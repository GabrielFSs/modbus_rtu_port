#include "app.h"
#include "hal_uart.h"
#include <string.h>
#include <stdio.h>

#define RX_BUF_SIZE 64

static hal_uart_drv_t uart;
static uint8_t rx_buf[RX_BUF_SIZE];

/* ================= CALLBACK ================= */

static void uart_cb(hal_uart_drv_t dev,
                    uart_event_t event,
                    uart_status_t status,
                    const uint8_t *data,
                    size_t len,
                    void *ctx)
{
    if (event == UART_EVENT_RX_DONE && len > 0)
    {
        char out[128];

        int n = snprintf(out, sizeof(out),
                         "Mensagem recebida: %.*s\r\n",
                         (int)len, data);

        size_t written;
        hal_uart_write(dev, (uint8_t*)out, n, &written, 100);
    }
}

/* ================= SETUP ================= */

void app_setup(void)
{
    hal_uart_init();

    hal_uart_cfg_t cfg =
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

        .rx_done_mode = UART_RX_DONE_ON_CHAR,
        .rx_done_char = '\r'
    };

    uart = hal_uart_open(HAL_UART_DEV_3, &cfg);

    if (uart == NULL)
    {
        while (1); // trava se falhar
    }

    hal_uart_set_event_cb(uart, uart_cb, NULL);

    /* Mensagem de boot */
    const char *boot = "UART3 pronta\r\n";
    size_t written;
    hal_uart_write(uart, (uint8_t*)boot, strlen(boot), &written, 100);
}

/* ================= LOOP ================= */

void app_loop(void)
{
    /* Nada aqui */
}
