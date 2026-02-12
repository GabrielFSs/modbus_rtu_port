#include "app.h"
#include "hal_uart.h"
#include "stm32f4xx_hal.h"

#include <string.h>
#include <stdio.h>

static hal_uart_drv_t uart;

/* ================= SETUP ================= */

void app_setup(void)
{
    hal_uart_init();

    hal_uart_cfg_t ucfg =
    {
        .baudrate = HAL_UART_BAUD_115200,
        .databits = HAL_UART_DATABITS_8,
        .stopbits = HAL_UART_STOPBIT_1,
        .parity   = HAL_UART_PARITY_NONE,

        .rx_buffer = NULL,
        .rx_buffer_size = 0,
        .rx_mode = UART_RX_MODE_LINEAR,

        .comm_mode = UART_MODE_POLLING,
        .duplex_mode = UART_DUPLEX_FULL,
        .rx_done_mode = UART_RX_DONE_NONE
    };

    uart = hal_uart_open(HAL_UART_DEV_3, &ucfg);
}

/* ================= LOOP ================= */

void app_loop(void)
{
    static uint32_t last_tick = 0;
    uint32_t now = HAL_GetTick();

    if ((now - last_tick) >= 5000)   // 5 segundos reais
    {
        last_tick = now;

        char msg[256];
        size_t written;

        uint32_t sysclk = HAL_RCC_GetSysClockFreq();
        uint32_t hclk   = HAL_RCC_GetHCLKFreq();
        uint32_t pclk1  = HAL_RCC_GetPCLK1Freq();

        /* Se quiser ainda ver os valores do TIM2 */
        uint32_t psc = TIM2->PSC;
        uint32_t arr = TIM2->ARR;

        int len = snprintf(msg, sizeof(msg),
            "\r\n=== CLOCK DEBUG ===\r\n"
            "SYSCLK = %lu\r\n"
            "HCLK   = %lu\r\n"
            "PCLK1  = %lu\r\n"
            "TIM2_PSC = %lu\r\n"
            "TIM2_ARR = %lu\r\n"
            "HAL_Tick = %lu\r\n"
            "====================\r\n",
            sysclk, hclk, pclk1, psc, arr, now);

        hal_uart_write(uart, (uint8_t*)msg, len, &written, 100);
    }
}
