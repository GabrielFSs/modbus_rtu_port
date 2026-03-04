#include "bsp_uart.h"
#include "stm32f4xx_hal.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void bsp_uart_init(bsp_uart_id_t id)
{
    if (id == BSP_UART_3)
    {
        /* Clock */
        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /* GPIO PB10 = TX, PB11 = RX */
        GPIO_InitTypeDef gpio = {0};

        gpio.Pin = GPIO_PIN_10 | GPIO_PIN_11;
        gpio.Mode = GPIO_MODE_AF_PP;
        gpio.Pull = GPIO_PULLUP;
        gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio.Alternate = GPIO_AF7_USART3;

        HAL_GPIO_Init(GPIOB, &gpio);

        /* NVIC */
        HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
    }
}

void bsp_uart_deinit(bsp_uart_id_t id)
{
    if (id == BSP_UART_3)
    {
        HAL_NVIC_DisableIRQ(USART3_IRQn);
        __HAL_RCC_USART3_CLK_DISABLE();
    }
}
