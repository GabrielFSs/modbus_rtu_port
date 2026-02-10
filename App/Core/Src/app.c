#include "main.h"
#include "hal_uart.h"

#include <stdint.h>
#include <stddef.h>

/* ===== UART CONFIG ===== */
static hal_uart_drv_t uart;

static hal_uart_cfg_t uart_cfg =
{
    .baudrate = HAL_UART_BAUD_9600,
    .databits = HAL_UART_DATABITS_8,
    .stopbits = HAL_UART_STOPBIT_1,
    .parity   = HAL_UART_PARITY_NONE,

    .comm_mode    = UART_MODE_POLLING,   // TX simples
    .duplex_mode  = UART_DUPLEX_FULL,
    .comm_control = UART_DIR_NONE,

    .rx_mode        = UART_RX_MODE_LINEAR,
    .rx_buffer      = NULL,
    .rx_buffer_size = 0,

    .tx_buffer      = NULL,
    .tx_buffer_size = 0,
};

/* ===== VARIÁVEIS QUE PRECISAM VIVER NO LOOP ===== */
static uint32_t last_tick = 0;
static const uint8_t msg[] = "TeSTE\n";

void app_setup(void)
{
    hal_uart_init();

    uart = hal_uart_open(HAL_UART_DEV_3, &uart_cfg);

    if (!uart)
    {
        /* Falha crítica */
        while (1);
    }

    last_tick = HAL_GetTick();

}

void app_loop(void)
{
    if ((HAL_GetTick() - last_tick) >= 1000)
    {
        last_tick = HAL_GetTick();

        size_t written;
        hal_uart_write(uart,
                       msg,
                       sizeof(msg) - 1,
                       &written,
                       100);
    }
}
