#include "hal_crc.h"
#include "stm32f4xx_hal.h"
#include <string.h>

/* ===== DRIVER STRUCT ===== */
struct hal_crc_drv_s
{
    hal_crc_type_t type;
};

/* ===== HANDLE CRC HW ===== */
static CRC_HandleTypeDef hcrc;

/* ===== CRC16 SOFTWARE (MODBUS) ===== */
static uint16_t crc16_modbus(const uint8_t *data, size_t len)
{
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < len; i++)
    {
        crc ^= data[i];

        for (int j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }

    return crc;
}

/* ===== INIT ===== */
static void stm32_crc_init(void)
{
    __HAL_RCC_CRC_CLK_ENABLE();

    hcrc.Instance = CRC;

    if (HAL_CRC_Init(&hcrc) != HAL_OK)
    {
        while (1); /* erro crítico */
    }
}

/* ===== DEINIT ===== */
static void stm32_crc_deinit(void)
{
    HAL_CRC_DeInit(&hcrc);
    __HAL_RCC_CRC_CLK_DISABLE();
}

/* ===== OPEN ===== */
static hal_crc_drv_t stm32_crc_open(const hal_crc_cfg_t *cfg)
{
    if (!cfg)
        return NULL;

    static struct hal_crc_drv_s drv;

    drv.type = cfg->type;

    return &drv;
}

/* ===== CLOSE ===== */
static void stm32_crc_close(hal_crc_drv_t crc)
{
    (void)crc;
}

/* ===== COMPUTE ===== */
static uint32_t stm32_crc_compute(hal_crc_drv_t crc,
                                  const uint8_t *data,
                                  size_t len)
{
    if (!crc || !data || len == 0)
        return 0;

    /* ---------- CRC16 SOFTWARE ---------- */
    if (crc->type == HAL_CRC_16)
    {
        return (uint32_t)crc16_modbus(data, len);
    }

    /* ---------- CRC32 HARDWARE ---------- */
    size_t words = len / 4;
    size_t rem   = len % 4;
    uint32_t result;

    __HAL_CRC_DR_RESET(&hcrc);

    if (words)
    {
        result = HAL_CRC_Calculate(&hcrc,
                                   (uint32_t *)data,
                                   words);
    }
    else
    {
        result = HAL_CRC_Calculate(&hcrc,
                                   (uint32_t[]){0},
                                   0);
    }

    if (rem)
    {
        uint32_t last = 0;
        const uint8_t *b = data + (words * 4);

        for (size_t i = 0; i < rem; i++)
            last |= ((uint32_t)b[i]) << (8 * i);

        result = HAL_CRC_Accumulate(&hcrc, &last, 1);
    }

    return result;
}

/* ===== DRIVER EXPORT ===== */
hal_crc_drv_imp_t HAL_CRC_DRV =
{
    .init    = stm32_crc_init,
    .deinit  = stm32_crc_deinit,
    .open    = stm32_crc_open,
    .close   = stm32_crc_close,
    .compute = stm32_crc_compute
};
