#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "port.h"

/* FreeModbus includes */
#include "mb.h"
#include "mbport.h"

/* HAL */
#include "hal_timer.h"

/* ================================
 * Timer Modbus (T3.5)
 * ================================ */
static hal_timer_drv_t mb_timer = NULL;
static uint32_t timeout_us = 0;

/* ================================
 * Callback do timer
 * ================================ */
static void mb_timer_cb(hal_timer_drv_t timer, void *ctx)
{
    (void)timer;
    (void)ctx;

    /* Notifica a stack Modbus */
    pxMBPortCBTimerExpired();
}

/* ================================
 * Inicializa timer T3.5
 * ================================ */
BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
    /* Cada unidade = 50us */
    timeout_us = (uint32_t)usTim1Timerout50us * 50;

    hal_timer_cfg_t cfg = {0};

    cfg.period = timeout_us;
    cfg.periodic  = false;           /* One-shot */
    cfg.cb        = mb_timer_cb;
    cfg.cb_ctx    = NULL;

    mb_timer = hal_timer_open(HAL_TIMER_0, &cfg);
    if (!mb_timer)
        return FALSE;

    return TRUE;
}

/* ================================
 * Habilita o timer (reinicia)
 * ================================ */
void vMBPortTimersEnable(void)
{
    if (!mb_timer)
        return;

    /* Reinicia o timer toda vez */
    hal_timer_stop(mb_timer);
    hal_timer_start(mb_timer);
}

/* ================================
 * Desabilita o timer
 * ================================ */
void vMBPortTimersDisable(void)
{
    if (!mb_timer)
        return;

    hal_timer_stop(mb_timer);
}
