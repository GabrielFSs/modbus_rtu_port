#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "hal_timer.h"

/* Driver ativo */
static hal_timer_drv_imp_t *drv = &HAL_TIMER_DRV;

void hal_timer_init(void)
{
    if (drv && drv->init)
    {
        drv->init();
    }
}

void hal_timer_deinit(void)
{
    if (drv && drv->deinit)
    {
        drv->deinit();
    }
}

hal_timer_drv_t hal_timer_open(hal_timer_id_t id,
                           const hal_timer_cfg_t *cfg)
{
    if (!drv || !drv->open || !cfg)
        return NULL;

    return drv->open(id, cfg);
}

void hal_timer_close(hal_timer_drv_t timer)
{
    if (!drv || !drv->close || !timer)
        return;

    drv->close(timer);
}

hal_timer_status_t hal_timer_start(hal_timer_drv_t timer)
{
    if (!drv || !drv->start || !timer)
        return HAL_TIMER_ERROR;

    return drv->start(timer);
}

hal_timer_status_t hal_timer_stop(hal_timer_drv_t timer)
{
    if (!drv || !drv->stop || !timer)
        return HAL_TIMER_ERROR;

    return drv->stop(timer);
}

hal_timer_status_t hal_timer_reset(hal_timer_drv_t timer)
{
    if (!drv || !drv->reset || !timer)
        return HAL_TIMER_ERROR;

    return drv->reset(timer);
}

bool hal_timer_is_running(hal_timer_drv_t timer)
{
    if (!drv || !drv->is_running || !timer)
        return false;

    return drv->is_running(timer);
}
