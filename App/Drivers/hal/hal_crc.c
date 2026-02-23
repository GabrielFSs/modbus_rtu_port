#include "hal_crc.h"

/* ===== DRIVER ===== */
static hal_crc_drv_imp_t *drv = &HAL_CRC_DRV;

/* ===== API ===== */
void hal_crc_init(void)
{
    if (drv->init)
        drv->init();
}

void hal_crc_deinit(void)
{
    if (drv->deinit)
        drv->deinit();
}

hal_crc_drv_t hal_crc_open(const hal_crc_cfg_t *cfg)
{
    if (!cfg || !drv->open)
        return NULL;

    return drv->open(cfg);
}

void hal_crc_close(hal_crc_drv_t crc)
{
    if (drv->close)
        drv->close(crc);
}

uint32_t hal_crc_compute(hal_crc_drv_t crc,
                         const uint8_t *data,
                         size_t len)
{
    if (!crc || !data || len == 0)
        return 0;

    return drv->compute(crc, data, len);
}
