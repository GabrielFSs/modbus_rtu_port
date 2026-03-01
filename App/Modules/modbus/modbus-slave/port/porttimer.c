#include "mb.h"
#include "mbport.h"
#include "mb_port.h"
#include "hal_timer.h"
#include "stddef.h"

static hal_timer_drv_t mb_timer = NULL;
static uint32_t timeout_us = 0;

static void mb_timer_cb(hal_timer_drv_t timer, void *ctx)
{
    (void)timer;
    (void)ctx;

    if (pxMBPortCBTimerExpired)
        pxMBPortCBTimerExpired();
}

BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
    /* converter para milissegundos */
    uint32_t timeout_ms = (usTim1Timerout50us * 50) / 1000;

    if (timeout_ms == 0)
        timeout_ms = 1;

    hal_timer_cfg_t cfg = {0};

    cfg.period   = timeout_ms;
    cfg.periodic = false;
    cfg.cb       = mb_timer_cb;
    cfg.cb_ctx   = NULL;

    mb_timer = hal_timer_open(HAL_TIMER_0, &cfg);

    return (mb_timer != NULL);
}

void vMBPortTimersEnable(void)
{
    if (!mb_timer)
        return;

    hal_timer_stop(mb_timer);
    hal_timer_start(mb_timer);
}

void vMBPortTimersDisable(void)
{
    if (mb_timer)
        hal_timer_stop(mb_timer);
}
