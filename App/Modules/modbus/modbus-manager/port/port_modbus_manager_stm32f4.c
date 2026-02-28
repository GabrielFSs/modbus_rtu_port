#include "port_modbus_manager.h"

#include "hal_uart.h"
#include "hal_timer.h"

#include <stddef.h>

static hal_uart_drv_t  uart  = NULL;
static hal_timer_drv_t timer = NULL;

static mbm_uart_rx_cb_t       rx_cb    = NULL;
static mbm_timer_timeout_cb_t timer_cb = NULL;

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

    if (!rx_cb)
        return;

    if (event == UART_EVENT_RX_DONE && data && len > 0)
    {
        rx_cb(data, len);
    }
}

/* ================= TIMER CALLBACK ================= */

static void timer_event_cb(hal_timer_drv_t t, void *ctx)
{
    (void)t;
    (void)ctx;

    if (timer_cb)
        timer_cb();
}

/* ================= INIT ================= */

static bool port_init(const modbus_serial_cfg_t *cfg)
{
    if (!cfg)
        return false;

    static uint8_t rx_buffer[256];

    hal_uart_cfg_t ucfg = {0};

    ucfg.baudrate = (hal_uart_baud_t)cfg->baudrate;
    ucfg.databits = (hal_uart_databits_t)cfg->databits;
    ucfg.stopbits = (hal_uart_dev_stopbit_t)cfg->stopbits;
    ucfg.parity   = (hal_uart_dev_parity_t)cfg->parity;

    ucfg.comm_mode   = UART_MODE_INTERRUPT;
    ucfg.duplex_mode = UART_DUPLEX_FULL;
    ucfg.comm_control = UART_DIR_NONE;

    ucfg.rx_mode        = UART_RX_MODE_LINEAR;
    ucfg.rx_buffer      = rx_buffer;
    ucfg.rx_buffer_size = sizeof(rx_buffer);

    ucfg.rx_done_mode = UART_RX_DONE_ON_TIMEOUT;

    uart = hal_uart_open(HAL_UART_DEV_3, &ucfg);
    if (!uart)
        return false;

    hal_uart_set_event_cb(uart, uart_event_cb, NULL);

    /* ===== T3.5 ===== */

    uint32_t t35_us = (35000000UL) / cfg->baudrate;
    if (t35_us < 100)
        t35_us = 100;

    hal_timer_cfg_t tcfg = {0};

    tcfg.period = t35_us;
    tcfg.periodic  = false;
    tcfg.cb        = timer_event_cb;
    tcfg.cb_ctx    = NULL;

    timer = hal_timer_open(HAL_TIMER_0, &tcfg);
    if (!timer)
        return false;

    hal_uart_set_rx_timeout_timer(
        uart,
        (hal_uart_timer_start_fn_t)hal_timer_start,
        (hal_uart_timer_stop_fn_t)hal_timer_stop,
        timer
    );

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

    if (uart)
    {
        hal_uart_close(uart);
        uart = NULL;
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

/* ================= EXPORT ================= */

const modbus_manager_port_t modbus_manager_port =
{
    .init                 = port_init,
    .deinit               = port_deinit,
    .set_uart_rx_callback = port_set_uart_rx_callback,
    .set_timer_callback   = port_set_timer_callback,
    .timer_restart        = port_timer_restart
};
