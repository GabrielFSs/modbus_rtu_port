#include "mb.h"
#include "mbport.h"
#include "mb_port.h"
#include "hal_timer.h"
#include "stddef.h"
#include "mb_sniffer.h"

static hal_timer_drv_t mb_timer = NULL;
static hal_timer_drv_t mb_timer_tx = NULL;
static uint32_t timeout_ms_val = 1;
static mb_port_tx_complete_cb_t tx_complete_cb = NULL;

static void mb_timer_cb(hal_timer_drv_t timer, void *ctx)
{
    (void)timer;
    (void)ctx;

    if (pxMBPortCBTimerExpired)
    {
        mb_sniffer_rx_timeout();
        pxMBPortCBTimerExpired();
    }
}

static void mb_timer_tx_cb(hal_timer_drv_t timer, void *ctx)
{
    (void)timer;
    (void)ctx;

    if (tx_complete_cb)
        tx_complete_cb();
}

void mb_port_timer_set_tx_complete_cb(mb_port_tx_complete_cb_t cb)
{
    tx_complete_cb = cb;
}

void mb_port_timer_restart_tx(void)
{
    if (!mb_timer_tx)
        return;

    hal_timer_stop(mb_timer_tx);
    hal_timer_start(mb_timer_tx);
}

BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
    /* converter para milissegundos */
    timeout_ms_val = (usTim1Timerout50us * 50) / 1000;

    if (timeout_ms_val == 0)
        timeout_ms_val = 1;

    hal_timer_cfg_t cfg = {0};

    cfg.period   = timeout_ms_val;
    cfg.periodic = false;
    cfg.cb       = mb_timer_cb;
    cfg.cb_ctx   = NULL;

    mb_timer = hal_timer_open(HAL_TIMER_0, &cfg);
    if (!mb_timer)
        return FALSE;

    /* Timer TX para sniffer: 3.5T após último byte TX = frame completo */
    cfg.cb = mb_timer_tx_cb;
    mb_timer_tx = hal_timer_open(HAL_TIMER_1, &cfg);

    return TRUE;
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
