#include "app.h"
#include "hal_uart.h"
#include "hal_rtc.h"
#include "stm32f4xx_hal.h"

#include <stdio.h>
#include <string.h>

static hal_uart_drv_t uart;

/* ================= SETUP ================= */

void app_setup(void)
{
    hal_uart_init();
    hal_rtc_init();

    /* ================= UART CONFIG ================= */

    hal_uart_cfg_t ucfg =
    {
        .baudrate        = HAL_UART_BAUD_115200,
        .databits        = HAL_UART_DATABITS_8,
        .stopbits        = HAL_UART_STOPBIT_1,
        .parity          = HAL_UART_PARITY_NONE,
        .rx_buffer       = NULL,
        .rx_buffer_size  = 0,
        .rx_mode         = UART_RX_MODE_LINEAR,
        .comm_mode       = UART_MODE_POLLING,
        .duplex_mode     = UART_DUPLEX_FULL,
        .rx_done_mode    = UART_RX_DONE_NONE
    };

    uart = hal_uart_open(HAL_UART_DEV_3, &ucfg);

    /* ================= SETA DATA/HORA INICIAL ================= */

    hal_rtc_date_t date =
    {
        .day = 14,
        .month = 5,
        .year = 2025
    };

    hal_rtc_time_t time =
    {
        .hour = 12,
        .min  = 0,
        .sec  = 0
    };

    hal_rtc_set_date(&date);
    hal_rtc_set_time(&time);
}

/* ================= LOOP ================= */

void app_loop(void)
{
    static uint32_t last_tick = 0;

    if (HAL_GetTick() - last_tick >= 200)
    {
        last_tick = HAL_GetTick();

        hal_rtc_timestamp_t ts;

        hal_rtc_get_timestamp(&ts);

        char buffer[128];
        size_t written;

        int len = snprintf(buffer, sizeof(buffer),
                           "%02d/%02d/%04d  %02d:%02d:%02d.%03d\r\n",
                           ts.day,
                           ts.month,
                           ts.year,
                           ts.hour,
                           ts.min,
                           ts.sec,
                           ts.ms);

        hal_uart_write(uart, (uint8_t*)buffer, len, &written, 100);
    }
}
