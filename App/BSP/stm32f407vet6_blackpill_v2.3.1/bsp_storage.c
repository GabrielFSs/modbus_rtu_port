#include "bsp_storage.h"

SD_HandleTypeDef hsd;

/* ================= GPIO CONFIG ================= */

static void bsp_storage_gpio_init(void)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};

    /* PC8-12 = D0-D3 + CLK */
    gpio.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
               GPIO_PIN_11 | GPIO_PIN_12;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF12_SDIO;
    HAL_GPIO_Init(GPIOC, &gpio);

    /* PD2 = CMD */
    gpio.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOD, &gpio);
}

/* ================= INIT ================= */

bsp_storage_status_t bsp_storage_init(void)
{
    __HAL_RCC_SDIO_CLK_ENABLE();

    bsp_storage_gpio_init();

    /* SDIO Config */
    hsd.Instance = SDIO;
    hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    hsd.Init.BusWide = SDIO_BUS_WIDE_1B;   // começa 1-bit
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    hsd.Init.ClockDiv = 2; // inicialização segura

    if (HAL_SD_Init(&hsd) != HAL_OK)
        return BSP_STORAGE_ERROR;

    /* Troca para 4-bit */
    if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK)
        return BSP_STORAGE_ERROR;

    return BSP_STORAGE_OK;
}

/* ================= DEINIT ================= */

void bsp_storage_deinit(void)
{
    HAL_SD_DeInit(&hsd);
    __HAL_RCC_SDIO_CLK_DISABLE();
}

/* ================= READ ================= */

bsp_storage_status_t bsp_storage_read_blocks(
    uint8_t *data,
    uint32_t block,
    uint32_t count)
{
    if (HAL_SD_ReadBlocks(&hsd, data, block, count, HAL_MAX_DELAY) != HAL_OK)
        return BSP_STORAGE_ERROR;

    while (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER);

    return BSP_STORAGE_OK;
}

/* ================= WRITE ================= */

bsp_storage_status_t bsp_storage_write_blocks(
    const uint8_t *data,
    uint32_t block,
    uint32_t count)
{
    if (HAL_SD_WriteBlocks(&hsd, (uint8_t*)data, block, count, HAL_MAX_DELAY) != HAL_OK)
        return BSP_STORAGE_ERROR;

    while (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER);

    return BSP_STORAGE_OK;
}
