#include "bsp_spi.h"

void bsp_spi_init(bsp_spi_id_t id)
{
    if (id != BSP_SPI_1)
        return;

    /* Enable clocks */
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* PA5 = SCK, PA6 = MISO, PA7 = MOSI */
    GPIO_InitTypeDef gpio = {0};

    gpio.Pin       = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_NOPULL;
    gpio.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF5_SPI1;

    HAL_GPIO_Init(GPIOA, &gpio);

    /* NVIC (opcional – preparado) */
    HAL_NVIC_SetPriority(SPI1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
}

void bsp_spi_deinit(bsp_spi_id_t id)
{
    if (id != BSP_SPI_1)
        return;

    HAL_NVIC_DisableIRQ(SPI1_IRQn);
    __HAL_RCC_SPI1_CLK_DISABLE();
}
