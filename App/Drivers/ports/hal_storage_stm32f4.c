#include "hal_storage.h"
#include "bsp_storage.h"

#include "ff.h"   // FATFS
#include <string.h>

/* ===== CONTEXTO ===== */
struct hal_storage_drv_s
{
    FATFS fs;
    bool mounted;
};

static struct hal_storage_drv_s storage;

/* ===== INIT ===== */
static hal_storage_status_t stm32_storage_init(void)
{
    if (bsp_storage_init() != BSP_STORAGE_OK)
        return HAL_STORAGE_ERROR;

    FRESULT res = f_mount(&storage.fs, "", 1);
    if (res != FR_OK)
        return HAL_STORAGE_ERROR;

    storage.mounted = true;
    return HAL_STORAGE_OK;
}

/* ===== DEINIT ===== */
static void stm32_storage_deinit(void)
{
    if (storage.mounted)
    {
        f_mount(NULL, "", 0);
        storage.mounted = false;
    }

    bsp_storage_deinit();
}

/* ===== WRITE FILE ===== */
static hal_storage_status_t stm32_storage_write(const char *path,
                                                const uint8_t *data,
                                                size_t len)
{
    if (!storage.mounted || !path || !data || len == 0)
        return HAL_STORAGE_ERROR;

    FIL file;
    UINT bw;

    if (f_open(&file, path, FA_OPEN_APPEND | FA_WRITE) != FR_OK)
        return HAL_STORAGE_ERROR;

    if (f_write(&file, data, len, &bw) != FR_OK || bw != len)
    {
        f_close(&file);
        return HAL_STORAGE_ERROR;
    }

    f_sync(&file);
    f_close(&file);

    return HAL_STORAGE_OK;
}

/* ===== READ FILE ===== */
static hal_storage_status_t stm32_storage_read(const char *path,
                                               uint8_t *data,
                                               size_t maxlen,
                                               size_t *out_len)
{
    if (!storage.mounted || !path || !data)
        return HAL_STORAGE_ERROR;

    FIL file;
    UINT br;

    if (f_open(&file, path, FA_READ) != FR_OK)
        return HAL_STORAGE_ERROR;

    if (f_read(&file, data, maxlen, &br) != FR_OK)
    {
        f_close(&file);
        return HAL_STORAGE_ERROR;
    }

    f_close(&file);

    if (out_len)
        *out_len = br;

    return HAL_STORAGE_OK;
}

/* ===== DRIVER EXPORT ===== */
hal_storage_drv_imp_t HAL_STORAGE_DRV =
{
    .init   = stm32_storage_init,
    .deinit = stm32_storage_deinit,
    .write  = stm32_storage_write,
    .read   = stm32_storage_read,
};
