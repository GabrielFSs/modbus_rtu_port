#include "hal_storage.h"

static hal_storage_drv_imp_t *drv = &HAL_STORAGE_DRV;

hal_storage_status_t hal_storage_init(void)
{
    return drv->init();
}

hal_storage_status_t hal_storage_deinit(void)
{
    return drv->deinit();
}

hal_storage_status_t hal_storage_write_file(
    const char *path,
    const uint8_t *data,
    size_t len)
{
    return drv->write_file(path, data, len);
}

hal_storage_status_t hal_storage_read_file(
    const char *path,
    uint8_t *data,
    size_t maxlen,
    size_t *out_len)
{
    return drv->read_file(path, data, maxlen, out_len);
}

/* 🔥 NOVO WRAPPER */
hal_storage_status_t hal_storage_append_file(
    const char *path,
    const uint8_t *data,
    size_t len)
{
    return drv->append_file(path, data, len);
}
