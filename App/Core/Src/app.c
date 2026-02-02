#include "app.h"
#include "hal_uart.h"

#include "stm32f4xx_hal.h"


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ===== CONFIG ===== */
#define RX_BUF_SIZE 64
#define TX_PERIOD_MS 10000

static hal_uart_drv_t uart;
static uint8_t rx_buf[RX_BUF_SIZE];

static uint32_t last_tick;

/* UART config */
static hal_uart_cfg_t uart_cfg =
{
    .baudrate = HAL_UART_BAUD_9600,
    .databits = HAL_UART_DATABITS_8,
    .stopbits = HAL_UART_STOPBIT_1,
    .parity   = HAL_UART_PARITY_NONE,

    .comm_mode   = UART_MODE_INTERRUPT,   // <<< IMPORTANTE
    .duplex_mode = UART_DUPLEX_FULL,
    .comm_control = UART_DIR_NONE,

    .rx_mode        = UART_RX_MODE_LINEAR,
    .rx_buffer      = rx_buf,
    .rx_buffer_size = sizeof(rx_buf),

    .tx_buffer      = NULL,
    .tx_buffer_size = 0,
};

void app_setup(void)
{
    hal_uart_init();

    uart = hal_uart_open(HAL_UART_DEV_3, &uart_cfg);
    if (!uart)
    {
        while (1);
    }

    last_tick = HAL_GetTick();
}

void app_loop(void)
{
    uint32_t now = HAL_GetTick();

    if ((now - last_tick) >= TX_PERIOD_MS)
    {
        last_tick = now;

        size_t len;
        uint8_t tmp[RX_BUF_SIZE];

        /* Copia e consome o buffer RX */
        if (hal_uart_read(uart,
                          tmp,
                          sizeof(tmp),
                          &len,
                          0) == UART_STATUS_OK && len > 0)
        {
            size_t written;

            hal_uart_write(uart,
                           tmp,
                           len,
                           &written,
                           100);

            /* Nova linha só para visual */
            const uint8_t nl[] = "\r\n";
            hal_uart_write(uart, nl, 2, NULL, 100);
        }
    }
}
