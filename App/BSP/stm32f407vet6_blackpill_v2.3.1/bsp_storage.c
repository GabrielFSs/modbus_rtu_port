#include "bsp_storage.h"
#include "stm32f4xx_hal.h"

/* Se estiver usando SDIO */
extern SD_HandleTypeDef hsd;

bsp_storage_status_t bsp_storage_init(void)
{
    /* Clock SDIO */
    __HAL_RCC_SDIO_CLK_ENABLE();

    /* GPIO e NVIC normalmente já vêm do Cube */
    if (HAL_SD_Init(&hsd) != HAL_OK)
        return BSP_STORAGE_ERROR;

    return BSP_STORAGE_OK;
}

void bsp_storage_deinit(void)
{
    HAL_SD_DeInit(&hsd);
    __HAL_RCC_SDIO_CLK_DISABLE();
}
