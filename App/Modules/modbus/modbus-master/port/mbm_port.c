#include "mbm_port.h"
#include "hal_uart.h"
#include "hal_timer.h"
#include "hal_crc.h"
#include "hal_time.h"
#include "hal_gpio.h"

/* ============================================================ */
/* RS485 POLICY (UART3 = RS485)                                */
/* ============================================================ */

#define RS485_DE_GPIO   HAL_GPIO_3   /* ajuste conforme seu BSP */
#define RS485_RE_GPIO   HAL_GPIO_4   /* ajuste conforme seu BSP */

static hal_gpio_drv_t rs485_de = NULL;
static hal_gpio_drv_t rs485_re = NULL;
static bool rs485_active = false;

static void rs485_init(void)
{
    if (rs485_active)
        return;

    hal_gpio_cfg_t cfg =
    {
        .direction = HAL_GPIO_OUTPUT,
        .pull      = HAL_GPIO_NOPULL,
        .out_type  = HAL_GPIO_PUSH_PULL,
        .irq_edge  = HAL_GPIO_IRQ_NONE
    };

    rs485_de = hal_gpio_open(RS485_DE_GPIO, &cfg);
    rs485_re = hal_gpio_open(RS485_RE_GPIO, &cfg);

    if (rs485_de) hal_gpio_write(rs485_de, false);
    if (rs485_re) hal_gpio_write(rs485_re, false);

    rs485_active = true;
}

static void rs485_deinit(void)
{
    if (!rs485_active)
        return;

    if (rs485_de)
    {
        hal_gpio_close(rs485_de);
        rs485_de = NULL;
    }

    if (rs485_re)
    {
        hal_gpio_close(rs485_re);
        rs485_re = NULL;
    }

    rs485_active = false;
}

static void rs485_dir_control(void *ctx, bool enable)
{
    (void)ctx;

    if (!rs485_active)
        return;

    if (enable)
    {
        hal_gpio_write(rs485_de, true);
        hal_gpio_write(rs485_re, true);
    }
    else
    {
        hal_gpio_write(rs485_de, false);
        hal_gpio_write(rs485_re, false);
    }
}

/* ============================================================ */
/* HANDLES                                                      */
/* ============================================================ */

static hal_uart_drv_t  uart  = NULL;
static hal_timer_drv_t timer = NULL;
static hal_crc_drv_t   crc   = NULL;

static uint8_t rx_byte;

/* ============================================================ */
/* UART CALLBACK                                               */
/* ============================================================ */

static void uart_event_cb(hal_uart_drv_t dev,
                          uart_event_t event,
                          uart_status_t status,
                          const uint8_t *data,
                          size_t len,
                          void *ctx)
{
    (void)dev;
    (void)ctx;
    (void)status;

    if (event == UART_EVENT_RX_DONE && data && len > 0)
    {
        rx_byte = data[0];
        mbm_rx_byte(rx_byte);
    }
}

/* ============================================================ */
/* TIMER CALLBACK (3.5 CHAR)                                   */
/* ============================================================ */

static void timer_cb(hal_timer_drv_t t, void *ctx)
{
    (void)t;
    (void)ctx;
    mbm_frame_timeout();
}

/* ============================================================ */
/* PORT WRAPPERS                                               */
/* ============================================================ */

static void port_uart_send(uint8_t *data, uint16_t len)
{
    size_t written;
    hal_uart_write(uart, data, len, &written, 1000);
}

static uint16_t port_crc16(uint8_t *data, uint16_t len)
{
    return (uint16_t)hal_crc_compute(crc, data, len);
}

static uint32_t port_time_ms(void)
{
    return hal_time_ms();
}

static void port_timer_start(void)
{
    if (!timer) return;

    hal_timer_stop(timer);
    hal_timer_start(timer);
}

/* ============================================================ */
/* PORT STRUCT                                                 */
/* ============================================================ */

static mbm_port_t port =
{
    .uart_send          = port_uart_send,
    .crc16              = port_crc16,
    .get_time_ms        = port_time_ms,
    .timer_start_35char = port_timer_start
};

/* ============================================================ */
/* AUX: CALCULAR 3.5 CHAR REAL                                 */
/* ============================================================ */

static uint32_t calculate_35char_timeout_ms(const mbm_serial_cfg_t *cfg)
{
    uint32_t bits_per_char = 1; /* start */

    bits_per_char += cfg->databits;

    if (cfg->parity != HAL_UART_PARITY_NONE)
        bits_per_char += 1;

    bits_per_char += (cfg->stopbits == HAL_UART_STOPBIT_2) ? 2 : 1;

    /* 3.5 chars */
    uint32_t total_bits = bits_per_char * 35UL / 10UL;  /* 3.5 * bits */

    uint32_t timeout_ms = (total_bits * 1000UL) / cfg->baudrate;

    if (timeout_ms == 0)
        timeout_ms = 1;

    return timeout_ms;
}

/* ============================================================ */
/* OPEN                                                         */
/* ============================================================ */

mbm_status_t mbm_port_open(const mbm_serial_cfg_t *cfg)
{
    if (!cfg)
        return MBM_ERR_INVALID;

    if (uart)
        mbm_port_close();

    /* ================= RS485 POLICY ================= */

    bool use_rs485 = (cfg->uart == HAL_UART_DEV_3);

    if (use_rs485)
        rs485_init();

    /* ================= UART ================= */

    hal_uart_cfg_t uart_cfg = {0};

    uart_cfg.baudrate = cfg->baudrate;
    uart_cfg.databits = cfg->databits;
    uart_cfg.parity   = cfg->parity;
    uart_cfg.stopbits = cfg->stopbits;

    uart_cfg.comm_mode = UART_MODE_INTERRUPT;

    if (use_rs485)
    {
        uart_cfg.duplex_mode  = UART_DUPLEX_HALF;
        uart_cfg.comm_control = UART_DIR_GPIO;
        uart_cfg.dir_ctrl     = rs485_dir_control;
        uart_cfg.dir_ctrl_ctx = NULL;
    }
    else
    {
        uart_cfg.duplex_mode  = UART_DUPLEX_FULL;
        uart_cfg.comm_control = UART_DIR_NONE;
    }

    uart_cfg.rx_mode        = UART_RX_MODE_LINEAR;
    uart_cfg.rx_buffer      = &rx_byte;
    uart_cfg.rx_buffer_size = 1;
    uart_cfg.rx_done_mode   = UART_RX_DONE_ON_LENGTH;
    uart_cfg.rx_done_length = 1;

    uart = hal_uart_open(cfg->uart, &uart_cfg);
    if (!uart)
        return MBM_ERR_BUSY;

    hal_uart_set_event_cb(uart, uart_event_cb, NULL);
    hal_uart_rx_enable(uart);

    /* ================= CRC ================= */

    hal_crc_cfg_t crc_cfg =
    {
        .type        = HAL_CRC_16,
        .poly        = HAL_CRC_POLY_MODBUS,
        .init_value  = 0xFFFF,
        .xor_out     = 0x0000,
        .reflect_in  = true,
        .reflect_out = true
    };

    crc = hal_crc_open(&crc_cfg);

    /* ================= TIMER 3.5 CHAR ================= */

    uint32_t timeout_ms = calculate_35char_timeout_ms(cfg);

    hal_timer_cfg_t timer_cfg =
    {
        .period   = timeout_ms,
        .periodic = false,
        .cb       = timer_cb,
        .cb_ctx   = NULL
    };

    timer = hal_timer_open(HAL_TIMER_0, &timer_cfg);

    mbm_init(&port);

    return MBM_ERR_OK;
}

/* ============================================================ */
/* CLOSE                                                        */
/* ============================================================ */

mbm_status_t mbm_port_close(void)
{
    if (uart)
    {
        hal_uart_close(uart);
        uart = NULL;
    }

    if (timer)
    {
        hal_timer_close(timer);
        timer = NULL;
    }

    if (crc)
    {
        hal_crc_close(crc);
        crc = NULL;
    }

    rs485_deinit();

    return MBM_ERR_OK;
}
