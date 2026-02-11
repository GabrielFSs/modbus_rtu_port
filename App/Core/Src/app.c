#include "app.h"
#include "hal_uart.h"
#include "hal_timer.h"
#include <string.h>
#include <stdio.h>

#define RX_BUF_SIZE 64

static hal_uart_drv_t uart;
static hal_timer_t timer;
static uint8_t rx_buf[RX_BUF_SIZE];

/* ================= TIMER CALLBACK ================= */

static void timer_cb(hal_timer_t t, void *ctx)
{
    hal_uart_drv_t dev = (hal_uart_drv_t)ctx;

    const char *msg = "Timeout 5s\r\n";
    size_t written;

    hal_uart_write(dev, (uint8_t*)msg, strlen(msg), &written, 100);

    /* NÃO reinicia o timer aqui */
}


/* ================= SETUP ================= */

void app_setup(void)
{
    hal_uart_init();
    hal_timer_init();

    hal_uart_cfg_t ucfg =
    {
        .baudrate = HAL_UART_BAUD_9600,
        .databits = HAL_UART_DATABITS_8,
        .stopbits = HAL_UART_STOPBIT_1,
        .parity   = HAL_UART_PARITY_NONE,

        .rx_buffer = rx_buf,
        .rx_buffer_size = sizeof(rx_buf),
        .rx_mode = UART_RX_MODE_LINEAR,

        .comm_mode = UART_MODE_INTERRUPT,
        .duplex_mode = UART_DUPLEX_FULL,

        /* Agora usamos timeout puro */
        .rx_done_mode = UART_RX_DONE_ON_TIMEOUT
    };

    uart = hal_uart_open(HAL_UART_DEV_3, &ucfg);

    /* TIMER CONFIG */
    hal_timer_cfg_t tcfg =
    {
        .period = 5000,
        .resolution = HAL_TIMER_RESOLUTION_MS,
        .periodic = false,
        .cb = timer_cb,
        .cb_ctx = uart
    };

    timer = hal_timer_open(HAL_TIMER_0, &tcfg);

    /* Conecta UART ao timer */
    hal_uart_set_rx_timeout_timer(
        uart,
        (hal_uart_timer_start_fn_t)hal_timer_start,
        (hal_uart_timer_stop_fn_t)hal_timer_stop,
        timer
    );
}


void app_loop(void)
{
}
