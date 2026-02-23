#include "main.h"
#include "hal_uart.h"

static hal_uart_drv_t uart;
static uint8_t rx_buffer[1];

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

    static uint8_t tx_byte;

    if (event == UART_EVENT_RX_DONE && data && len > 0)
    {
        tx_byte = data[0];
        hal_uart_write(uart, &tx_byte, 1, NULL, 100);
    }
}

void app_setup(void)
{
    hal_uart_init();

    hal_uart_cfg_t cfg = {0};

    cfg.baudrate = HAL_UART_BAUD_9600;
    cfg.databits = HAL_UART_DATABITS_8;
    cfg.stopbits = HAL_UART_STOPBIT_1;
    cfg.parity   = HAL_UART_PARITY_NONE;

    cfg.comm_mode   = UART_MODE_INTERRUPT;
    cfg.duplex_mode = UART_DUPLEX_FULL;

    cfg.rx_mode        = UART_RX_MODE_LINEAR;
    cfg.rx_buffer      = rx_buffer;
    cfg.rx_buffer_size = 1;

    cfg.rx_done_mode   = UART_RX_DONE_ON_LENGTH;
    cfg.rx_done_length = 1;

    uart = hal_uart_open(HAL_UART_DEV_3, &cfg);

    hal_uart_set_event_cb(uart, uart_event_cb, NULL);

    hal_uart_rx_enable(uart);
}

void app_loop(void)
{
}
