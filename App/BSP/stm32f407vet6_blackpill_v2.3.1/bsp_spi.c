#include "bsp_spi.h"
#include <stdbool.h>

void bsp_spi_init(bsp_spi_id_t id,
                  bool enable_irq,
                  bool enable_dma)
{
    if (id != BSP_SPI_1)
        return;

    /* ================= CLOCKS ================= */

    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* ================= GPIO CONFIG ================= */
    /* SPI1:
       PA5 = SCK
       PA6 = MISO
       PA7 = MOSI
    */

    GPIO_InitTypeDef gpio = {0};

    gpio.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_NOPULL;
    gpio.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF5_SPI1;

    HAL_GPIO_Init(GPIOA, &gpio);

    /* ================= INTERRUPT ================= */

    if (enable_irq)
    {
        HAL_NVIC_SetPriority(SPI1_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
    }

    /* ================= DMA (Preparado para futuro) ================= */

    if (enable_dma)
    {
        __HAL_RCC_DMA2_CLK_ENABLE();
        /* DMA config será feito dentro do hal_spi_stm32f4 */
    }
}

void bsp_spi_deinit(bsp_spi_id_t id)
{
    if (id != BSP_SPI_1)
        return;

    HAL_NVIC_DisableIRQ(SPI1_IRQn);

    HAL_GPIO_DeInit(GPIOA,
                    GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    __HAL_RCC_SPI1_CLK_DISABLE();
}
