#include "app.h"

#include "hal_crc.h"
#include "hal_uart.h"

#include <string.h>
#include <stdio.h>

/* ===== CONFIG ===== */
#define RX_BUF_SIZE     64
#define CRC_INPUT_LEN   10   /* quantidade de bytes esperados */

/* ===== HANDLES ===== */
static hal_crc_drv_t crc;
static hal_uart_drv_t uart;

/* ===== BUFFERS ===== */
static uint8_t rx_buf[RX_BUF_SIZE];
static size_t rx_len = 0;

/* ===== CRC CONFIG ===== */
static const hal_crc_cfg_t crc_cfg =
{
    .type        = HAL_CRC_32,
    .poly        = HAL_CRC_POLY_DEFAULT,
    .init_value  = 0xFFFFFFFF,
    .xor_out     = 0,
    .reflect_in  = false,
    .reflect_out = false
};

/* ===== SETUP ===== */
void app_setup(void)
{
    /* CRC */
    hal_crc_init();
    crc = hal_crc_open(&crc_cfg);

    /* UART */
    static uint8_t uart_rx_buffer[RX_BUF_SIZE];

    hal_uart_cfg_t uart_cfg =
    {
        .baudrate        = HAL_UART_BAUD_115200,
        .databits        = HAL_UART_DATABITS_8,
        .parity          = HAL_UART_PARITY_NONE,
        .stopbits        = HAL_UART_STOPBIT_1,

        .comm_mode       = UART_MODE_INTERRUPT,
        .duplex_mode     = UART_DUPLEX_FULL,

        .rx_mode         = UART_RX_MODE_CIRCULAR,
        .rx_buffer       = uart_rx_buffer,
        .rx_buffer_size  = sizeof(uart_rx_buffer),

        .tx_buffer       = NULL,
        .tx_buffer_size  = 0,

        .comm_control    = UART_DIR_NONE,
    };

    uart = hal_uart_open(HAL_UART_DEV_3, &uart_cfg);

    /* Mensagem inicial */
    const char *msg =
        "\r\n=== CRC HAL TEST ===\r\n"
        "Send 10 bytes to calculate CRC32\r\n";

    size_t written;
    hal_uart_write(uart,
                   (uint8_t *)msg,
                   strlen(msg),
                   &written,
                   1000);
}

/* ===== LOOP ===== */
void app_loop(void)
{
    uint8_t tmp[16];
    size_t read;

    /* Lê o que tiver disponível */
    if (hal_uart_read(uart, tmp, sizeof(tmp), &read, 0) != UART_STATUS_OK)
        return;

    for (size_t i = 0; i < read; i++)
    {
        if (rx_len < RX_BUF_SIZE)
            rx_buf[rx_len++] = tmp[i];

        /* Quando atingir o tamanho esperado */
        if (rx_len == CRC_INPUT_LEN)
        {
            uint32_t result = hal_crc_compute(crc, rx_buf, rx_len);

            char tx[64];
            snprintf(tx, sizeof(tx),
                     "\r\nCRC32 = 0x%08lX\r\n",
                     result);

            size_t written;
            hal_uart_write(uart,
                           (uint8_t *)tx,
                           strlen(tx),
                           &written,
                           1000);

            /* Reset para próxima mensagem */
            rx_len = 0;
            memset(rx_buf, 0, sizeof(rx_buf));
        }
    }
}
