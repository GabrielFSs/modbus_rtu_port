#include "app.h"
#include "hal_uart.h"
#include "stm32f4xx_hal.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ===== CONFIG ===== */
#define RX_BUF_SIZE 256

static hal_uart_drv_t uart;
static uint8_t rx_buf[RX_BUF_SIZE];

/* ===== UART CONFIG ===== */
static hal_uart_cfg_t uart_cfg =
{
    .baudrate = HAL_UART_BAUD_9600,
    .databits = HAL_UART_DATABITS_8,
    .stopbits = HAL_UART_STOPBIT_1,
    .parity   = HAL_UART_PARITY_NONE,

    .comm_mode    = UART_MODE_INTERRUPT,
    .duplex_mode  = UART_DUPLEX_FULL,
    .comm_control = UART_DIR_NONE,

    .rx_mode        = UART_RX_MODE_LINEAR,
    .rx_buffer      = rx_buf,
    .rx_buffer_size = sizeof(rx_buf),

    .tx_buffer      = NULL,
    .tx_buffer_size = 0,
};

/* ===== EVENT CALLBACK (ISR CONTEXT) ===== */
static void uart_event_cb(hal_uart_drv_t dev,
                          uart_event_t event,
                          uart_status_t status,
                          const uint8_t *data,
                          size_t len,
                          void *ctx)
{
    (void)ctx;
    (void)status;

    if (event != UART_EVENT_RX_DONE)
        return;

    /* Ecoa tudo o que foi recebido */
    size_t written;
    hal_uart_write(dev,
                   data,
                   len,
                   &written,
                   100);

    /* Nova linha para terminal */
    const uint8_t nl[] = "\r\n";
    hal_uart_write(dev, nl, 2, NULL, 100);

    /* Limpa RX para próxima mensagem */
    hal_uart_flush(dev);
}

/* ===== APP SETUP ===== */
void app_setup(void)
{
    hal_uart_init();

    uart = hal_uart_open(HAL_UART_DEV_3, &uart_cfg);
    if (!uart)
    {
        while (1);
    }

    hal_uart_set_event_cb(uart, uart_event_cb, NULL);
}

/* ===== ISR-ONLY ===== */
void app_loop(void)
{
    /* Nada aqui */
}
