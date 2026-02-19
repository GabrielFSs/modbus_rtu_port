#include "hal_storage.h"
#include "bsp_storage.h"
#include "ff.h"
#include <string.h>

/* ========================================================= */
/*               STM32 + FATFS IMPLEMENTATION                */
/* ========================================================= */

static FATFS fs;

/* ================= INIT ================= */

static hal_storage_status_t stm32_storage_init(void)
{
    if (bsp_storage_init() != BSP_STORAGE_OK)
        return HAL_STORAGE_ERROR;

    if (f_mount(&fs, "", 1) != FR_OK)
        return HAL_STORAGE_ERROR;

    return HAL_STORAGE_OK;
}

static hal_storage_status_t stm32_storage_deinit(void)
{
    f_mount(NULL, "", 1);
    bsp_storage_deinit();
    return HAL_STORAGE_OK;
}

/* ================= WRITE (OVERWRITE) ================= */

static hal_storage_status_t stm32_storage_write_file(
    const char *path,
    const uint8_t *data,
    size_t len)
{
    FIL fil;

    if (f_open(&fil, path, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
        return HAL_STORAGE_ERROR;

    UINT written;
    if (f_write(&fil, data, len, &written) != FR_OK || written != len)
    {
        f_close(&fil);
        return HAL_STORAGE_ERROR;
    }

    f_close(&fil);
    return HAL_STORAGE_OK;
}

/* ================= APPEND 🔥 ================= */

static hal_storage_status_t stm32_storage_append_file(
    const char *path,
    const uint8_t *data,
    size_t len)
{
    FIL fil;

    /* Abre criando se não existir */
    if (f_open(&fil, path, FA_OPEN_ALWAYS | FA_WRITE) != FR_OK)
        return HAL_STORAGE_ERROR;

    /* Move ponteiro para final */
    if (f_lseek(&fil, f_size(&fil)) != FR_OK)
    {
        f_close(&fil);
        return HAL_STORAGE_ERROR;
    }

    UINT written;
    if (f_write(&fil, data, len, &written) != FR_OK || written != len)
    {
        f_close(&fil);
        return HAL_STORAGE_ERROR;
    }

    f_close(&fil);
    return HAL_STORAGE_OK;
}

/* ================= READ ================= */

static hal_storage_status_t stm32_storage_read_file(
    const char *path,
    uint8_t *data,
    size_t maxlen,
    size_t *out_len)
{
    FIL fil;

    if (f_open(&fil, path, FA_READ) != FR_OK)
        return HAL_STORAGE_ERROR;

    UINT read;
    if (f_read(&fil, data, maxlen, &read) != FR_OK)
    {
        f_close(&fil);
        return HAL_STORAGE_ERROR;
    }

    if (out_len)
        *out_len = read;

    f_close(&fil);
    return HAL_STORAGE_OK;
}

/* ========================================================= */
/*                   DRIVER INSTANCE                         */
/* ========================================================= */

hal_storage_drv_imp_t HAL_STORAGE_DRV =
{
    .init        = stm32_storage_init,
    .deinit      = stm32_storage_deinit,
    .write_file  = stm32_storage_write_file,
    .read_file   = stm32_storage_read_file,
    .append_file = stm32_storage_append_file   /* 🔥 NOVO */
};
