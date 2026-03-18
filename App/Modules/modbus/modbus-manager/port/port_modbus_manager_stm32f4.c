#include "port_modbus_manager.h"

#include "hal_uart.h"
#include "hal_timer.h"
#include "hal_crc.h"
#include "hal_time.h"
#include "hal_gpio.h"

#include <stddef.h>

static hal_uart_drv_t  uart  = NULL;
static hal_timer_drv_t timer = NULL;
static hal_crc_drv_t   crc   = NULL;

static hal_gpio_drv_t gpio_de = NULL;
static hal_gpio_drv_t gpio_re = NULL;

static mbm_uart_rx_cb_t       rx_cb    = NULL;
static mbm_timer_timeout_cb_t timer_cb = NULL;

/* ================= RS485 DE/RE CONTROL =================
 * BSP: RS485_DE=PE4, RS485_RE=PE3 (evita conflito com T_CS e SPI2) */

static void rs485_dir_ctrl(void *ctx, bool enable)
{
    (void)ctx;

    if (!gpio_de || !gpio_re)
        return;

    if (enable)
    {
        hal_gpio_write(gpio_de, true);
        hal_gpio_write(gpio_re, true);
    }
    else
    {
        hal_gpio_write(gpio_de, false);
        hal_gpio_write(gpio_re, false);
    }
}

/* ================= UART CALLBACK ================= */

static void uart_event_cb(hal_uart_drv_t dev,
                          uart_event_t event,
                          uart_status_t status,
                          const uint8_t *data,
                          size_t len,
                          void *ctx)
{
    (void)dev;
    (void)status;
    (void)ctx;

    if (event == UART_EVENT_RX_DONE && data && len > 0 && rx_cb)
        rx_cb(data, len);
}

/* ================= TIMER CALLBACK ================= */

static void timer_event_cb(hal_timer_drv_t t, void *ctx)
{
    (void)t;
    (void)ctx;

    if (timer_cb)
        timer_cb();
}

static void port_deinit(void);

/* ================= INIT ================= */

static bool port_init(const modbus_serial_cfg_t *cfg)
{
    if (!cfg)
        return false;

    port_deinit();

    static uint8_t rx_buf[1];

    hal_uart_cfg_t ucfg = {0};

    ucfg.baudrate = (hal_uart_baud_t)cfg->baudrate;

    switch (cfg->databits)
    {
        case 9:  ucfg.databits = HAL_UART_DATABITS_9; break;
        default: ucfg.databits = HAL_UART_DATABITS_8; break;
    }

    ucfg.stopbits = (cfg->stopbits == 2)
                    ? HAL_UART_STOPBIT_2
                    : HAL_UART_STOPBIT_1;

    switch (cfg->parity)
    {
        case 1:  ucfg.parity = HAL_UART_PARITY_EVEN; break;
        case 2:  ucfg.parity = HAL_UART_PARITY_ODD;  break;
        default: ucfg.parity = HAL_UART_PARITY_NONE;  break;
    }

    ucfg.comm_mode   = UART_MODE_INTERRUPT;

    /* DEV3 = RS485 com controle DE/RE (PE4, PE3 via BSP) */
    hal_uart_dev_interface_t dev = (cfg->uart_dev == 0)
                                    ? HAL_UART_DEV_2
                                    : HAL_UART_DEV_3;
    bool use_rs485 = (dev == HAL_UART_DEV_3);

    if (use_rs485)
    {
        hal_gpio_cfg_t gpio_cfg = {
            .direction = HAL_GPIO_OUTPUT,
            .pull      = HAL_GPIO_NOPULL,
            .out_type  = HAL_GPIO_PUSH_PULL,
            .irq_edge  = HAL_GPIO_IRQ_NONE
        };
        gpio_de = hal_gpio_open(HAL_GPIO_RS485_DE, &gpio_cfg);
        gpio_re = hal_gpio_open(HAL_GPIO_RS485_RE, &gpio_cfg);
        if (!gpio_de || !gpio_re)
            return false;
        hal_gpio_write(gpio_de, false);
        hal_gpio_write(gpio_re, false);

        ucfg.duplex_mode   = UART_DUPLEX_HALF;
        ucfg.comm_control  = UART_DIR_GPIO;
        ucfg.dir_ctrl      = rs485_dir_ctrl;
        ucfg.dir_ctrl_ctx  = NULL;
    }
    else
    {
        ucfg.duplex_mode   = UART_DUPLEX_FULL;
        ucfg.comm_control  = UART_DIR_NONE;
    }

    ucfg.rx_mode        = UART_RX_MODE_LINEAR;
    ucfg.rx_buffer      = rx_buf;
    ucfg.rx_buffer_size = sizeof(rx_buf);

    ucfg.rx_done_mode   = UART_RX_DONE_ON_LENGTH;
    ucfg.rx_done_length = 1;

    uart = hal_uart_open(dev, &ucfg);
    if (!uart)
    {
        if (use_rs485)
        {
            if (gpio_de) { hal_gpio_close(gpio_de); gpio_de = NULL; }
            if (gpio_re) { hal_gpio_close(gpio_re); gpio_re = NULL; }
        }
        return false;
    }

    hal_uart_set_event_cb(uart, uart_event_cb, NULL);
    hal_uart_rx_enable(uart);

    /* CRC16 Modbus para modo Master */
    hal_crc_cfg_t crc_cfg = {
        .type        = HAL_CRC_16,
        .poly        = HAL_CRC_POLY_MODBUS,
        .init_value  = 0xFFFF,
        .xor_out     = 0x0000,
        .reflect_in  = true,
        .reflect_out = true
    };
    crc = hal_crc_open(&crc_cfg);
    if (!crc)
    {
        hal_uart_close(uart);
        uart = NULL;
        if (use_rs485)
        {
            if (gpio_de) { hal_gpio_close(gpio_de); gpio_de = NULL; }
            if (gpio_re) { hal_gpio_close(gpio_re); gpio_re = NULL; }
        }
        return false;
    }

    /* ===== T3.5 (in milliseconds for the HAL timer) ===== */

    uint32_t bits_per_char = 1 + cfg->databits
                           + (cfg->parity ? 1 : 0)
                           + cfg->stopbits;
    uint32_t t35_ms = (bits_per_char * 35UL * 1000UL)
                    / (cfg->baudrate * 10UL);
    if (t35_ms == 0)
        t35_ms = 1;

    hal_timer_cfg_t tcfg = {0};

    tcfg.period    = t35_ms;
    tcfg.periodic  = false;
    tcfg.cb        = timer_event_cb;
    tcfg.cb_ctx    = NULL;

    timer = hal_timer_open(HAL_TIMER_0, &tcfg);
    if (!timer)
    {
        hal_crc_close(crc);
        crc = NULL;
        hal_uart_close(uart);
        uart = NULL;
        if (use_rs485)
        {
            if (gpio_de) { hal_gpio_close(gpio_de); gpio_de = NULL; }
            if (gpio_re) { hal_gpio_close(gpio_re); gpio_re = NULL; }
        }
        return false;
    }

    return true;
}

/* ================= DEINIT ================= */

static void port_deinit(void)
{
    if (timer)
    {
        hal_timer_stop(timer);
        hal_timer_close(timer);
        timer = NULL;
    }

    if (crc)
    {
        hal_crc_close(crc);
        crc = NULL;
    }

    if (uart)
    {
        hal_uart_close(uart);
        uart = NULL;
    }

    if (gpio_de)
    {
        hal_gpio_close(gpio_de);
        gpio_de = NULL;
    }

    if (gpio_re)
    {
        hal_gpio_close(gpio_re);
        gpio_re = NULL;
    }

    rx_cb    = NULL;
    timer_cb = NULL;
}

/* ================= CALLBACK SET ================= */

static void port_set_uart_rx_callback(mbm_uart_rx_cb_t cb)
{
    rx_cb = cb;
}

static void port_set_timer_callback(mbm_timer_timeout_cb_t cb)
{
    timer_cb = cb;
}

/* ================= TIMER RESTART ================= */

static void port_timer_restart(void)
{
    if (timer)
    {
        hal_timer_stop(timer);
        hal_timer_start(timer);
    }
}

/* ================= MASTER (mbm) HELPERS ================= */

static void port_uart_send(uint8_t *data, uint16_t len)
{
    if (!uart || !data || len == 0)
        return;

    /* TX bloqueante: envia frame completo sem usar TxCpltCallback,
     * evita travamentos e garante que RX está pronto para resposta. */
    hal_uart_write_blocking(uart, data, len, 1000);
}

static uint16_t port_crc16(uint8_t *data, uint16_t len)
{
    if (!crc || !data)
        return 0;
    return (uint16_t)hal_crc_compute(crc, data, len);
}

static uint32_t port_get_time_ms(void)
{
    return hal_time_ms();
}

/* ================= EXPORT ================= */

const modbus_manager_port_t modbus_manager_port =
{
    .init                 = port_init,
    .deinit               = port_deinit,
    .set_uart_rx_callback = port_set_uart_rx_callback,
    .set_timer_callback   = port_set_timer_callback,
    .timer_restart        = port_timer_restart,
    .uart_send            = port_uart_send,
    .crc16                = port_crc16,
    .get_time_ms          = port_get_time_ms
};
