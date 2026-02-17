#include "mb_master.h"

#include "hal_uart.h"
#include "hal_timer.h"
#include "hal_crc.h"
#include "hal_time.h"

/* ============================================================ */
/* HANDLES */
/* ============================================================ */

static hal_uart_drv_t  uart;
static hal_timer_drv_t timer;
static hal_crc_drv_t   crc;

/* ============================================================ */
/* UART EVENT CALLBACK */
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
        for (size_t i = 0; i < len; i++)
        {
            mbm_rx_byte(data[i]);
        }
    }
}

/* ============================================================ */
/* TIMER CALLBACK (3.5 CHAR SILENCE) */
/* ============================================================ */

static void timer_cb(hal_timer_drv_t t,
                     void *ctx)
{
    (void)t;
    (void)ctx;

    mbm_frame_timeout();
}

/* ============================================================ */
/* PORT FUNCTIONS */
/* ============================================================ */

static void port_uart_send(uint8_t *data,
                           uint16_t len)
{
    size_t written = 0;

    hal_uart_write(uart,
                   data,
                   len,
                   &written,
                   1000);
}

static uint16_t port_crc16(uint8_t *data,
                           uint16_t len)
{
    return (uint16_t)hal_crc_compute(crc,
                                     data,
                                     len);
}

static uint32_t port_time_ms(void)
{
    return hal_time_ms();
}

static void port_timer_start(void)
{
    hal_timer_stop(timer);
    hal_timer_start(timer);
}

/* ============================================================ */
/* INIT */
/* ============================================================ */

void mbm_port_init(hal_uart_drv_t uart_handle,
                   uint32_t baudrate)
{
    uart = uart_handle;

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

    /* ================= UART ================= */

    hal_uart_set_event_cb(uart,
                          uart_event_cb,
                          NULL);

    /* ================= 3.5 CHAR TIMER ================= */

    /*
       1 char ≈ 10 bits
       3.5 chars ≈ 35 bits
       tempo(ms) = (35 / baudrate) * 1000
    */

    uint32_t timeout_ms =
        (35UL * 1000UL) / baudrate;

    if (timeout_ms == 0)
        timeout_ms = 1;

    hal_timer_cfg_t cfg =
    {
        .period = timeout_ms,
        .cb     = timer_cb
    };

    timer = hal_timer_open(HAL_TIMER_0, &cfg);

    /* ================= PORT STRUCT ================= */

    static mbm_port_t port =
    {
        .uart_send          = port_uart_send,
        .crc16              = port_crc16,
        .get_time_ms        = port_time_ms,
        .timer_start_35char = port_timer_start
    };

    mbm_init(&port);
}
