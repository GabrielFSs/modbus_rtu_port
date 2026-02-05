#include "hal_storage.h"

static hal_storage_drv_imp_t *drv = &HAL_STORAGE_DRV;

hal_storage_status_t hal_storage_init(void)
{
    if (!drv || !drv->init)
        return HAL_STORAGE_ERROR;

    return drv->init();
}

hal_storage_status_t hal_storage_open(
    hal_storage_t *dev,
    const hal_storage_cfg_t *cfg)
{
    if (!drv || !drv->open)
        return HAL_STORAGE_ERROR;

    return drv->open(dev, cfg);
}

void hal_storage_close(hal_storage_t dev)
{
    if (drv && drv->close)
        drv->close(dev);
}

hal_storage_status_t hal_storage_read(
    hal_storage_t dev,
    uint32_t block,
    uint8_t *data,
    uint32_t count)
{
    if (!drv || !drv->read)
        return HAL_STORAGE_ERROR;

    return drv->read(dev, block, data, count);
}

hal_storage_status_t hal_storage_write(
    hal_storage_t dev,
    uint32_t block,
    const uint8_t *data,
    uint32_t count)
{
    if (!drv || !drv->write)
        return HAL_STORAGE_ERROR;

    return drv->write(dev, block, data, count);
}

hal_storage_status_t hal_storage_erase(
    hal_storage_t dev,
    uint32_t block,
    uint32_t count)
{
    if (!drv || !drv->erase)
        return HAL_STORAGE_ERROR;

    return drv->erase(dev, block, count);
}

hal_storage_status_t hal_storage_sync(hal_storage_t dev)
{
    if (!drv || !drv->sync)
        return HAL_STORAGE_ERROR;

    return drv->sync(dev);
}
