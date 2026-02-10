#include "app.h"
#include "hal_uart.h"
#include "hal_crc.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "stm32f4xx_hal.h"

/* ===== CONFIG ===== */
#define TX_PERIOD_MS 10000

static hal_uart_drv_t uart;
static hal_crc_drv_t  crc;

static uint32_t last_tick;

/* ===== UART CONFIG (POLLING – IGUAL AO FUNCIONAL) ===== */
static hal_uart_cfg_t uart_cfg =
{
    .baudrate = HAL_UART_BAUD_9600,
    .databits = HAL_UART_DATABITS_8,
    .stopbits = HAL_UART_STOPBIT_1,
    .parity   = HAL_UART_PARITY_NONE,

    .comm_mode    = UART_MODE_POLLING,   // <<< IMPORTANTE
    .duplex_mode  = UART_DUPLEX_FULL,
    .comm_control = UART_DIR_NONE,

    .rx_mode        = UART_RX_MODE_LINEAR,
    .rx_buffer      = NULL,
    .rx_buffer_size = 0,

    .tx_buffer      = NULL,
    .tx_buffer_size = 0,
};

/* ===== CRC16 SOFTWARE (MODBUS) ===== */
static hal_crc_cfg_t crc_cfg =
{
    .type        = HAL_CRC_16,
    .poly        = HAL_CRC_POLY_MODBUS,
    .init_value  = 0xFFFF,
    .xor_out     = 0,
    .reflect_in  = true,
    .reflect_out = true
};

/* ===== DADO FIXO ===== */
static const uint8_t test_data[] = "123456789";

/* ===== SETUP ===== */
void app_setup(void)
{
    hal_uart_init();
    hal_crc_init();

    uart = hal_uart_open(HAL_UART_DEV_3, &uart_cfg);
    if (!uart)
        while (1);

    crc = hal_crc_open(&crc_cfg);
    if (!crc)
        while (1);

    const char *msg =
        "\r\n=== CRC16 INTERNAL TEST ===\r\n"
        "Data: \"123456789\"\r\n"
        "Expected CRC16: 0x4B37\r\n\r\n";

    size_t written;
    hal_uart_write(uart,
                   (uint8_t *)msg,
                   strlen(msg),
                   &written,
                   100);

    last_tick = HAL_GetTick();
}

/* ===== LOOP ===== */
void app_loop(void)
{
    if ((HAL_GetTick() - last_tick) < TX_PERIOD_MS)
        return;

    last_tick = HAL_GetTick();

    uint16_t crc16 = (uint16_t)hal_crc_compute(
        crc,
        test_data,
        sizeof(test_data) - 1   // sem o '\0'
    );

    char out[64];
    int n = snprintf(out, sizeof(out),
                     "CRC16 result: 0x%04X\r\n",
                     crc16);

    size_t written;
    hal_uart_write(uart,
                   (uint8_t *)out,
                   n,
                   &written,
                   100);
}
