#include "hal_crc.h"
#include "stm32f4xx_hal.h"

/* ===== HANDLE ===== */
static CRC_HandleTypeDef hcrc;

/* ===== INIT ===== */
static void stm32_crc_init(void)
{
    __HAL_RCC_CRC_CLK_ENABLE();

    hcrc.Instance = CRC;

    /* STM32F4:
     * - Polinômio fixo CRC-32 (0x04C11DB7)
     * - Input e output não refletidos
     * - Init value = 0xFFFFFFFF
     */
    if (HAL_CRC_Init(&hcrc) != HAL_OK)
    {
        /* erro crítico */
        while (1);
    }
}

/* ===== DEINIT ===== */
static void stm32_crc_deinit(void)
{
    HAL_CRC_DeInit(&hcrc);
    __HAL_RCC_CRC_CLK_DISABLE();
}

/* ===== CALC CRC32 ===== */
static uint32_t stm32_crc_compute(const void *data, size_t len)
{
    if (!data || len == 0)
        return 0;

    /* CRC do STM32 trabalha em palavras de 32 bits */
    size_t words = len / 4;
    size_t rem   = len % 4;

    uint32_t crc;

    __HAL_CRC_DR_RESET(&hcrc);

    /* Parte alinhada */
    if (words)
    {
        crc = HAL_CRC_Calculate(
            &hcrc,
            (uint32_t *)data,
            words
        );
    }
    else
    {
        crc = HAL_CRC_Calculate(&hcrc, (uint32_t[]){0}, 0);
    }

    /* Parte restante (bytes finais) */
    if (rem)
    {
        uint32_t last = 0;
        const uint8_t *b = (const uint8_t *)data + (words * 4);

        for (size_t i = 0; i < rem; i++)
            last |= ((uint32_t)b[i]) << (8 * i);

        crc = HAL_CRC_Accumulate(&hcrc, &last, 1);
    }

    return crc;
}

/* ===== DRIVER EXPORT ===== */
hal_crc_drv_imp_t HAL_CRC_DRV =
{
    .init    = stm32_crc_init,
    .deinit  = stm32_crc_deinit,
    .compute = stm32_crc_compute
};
