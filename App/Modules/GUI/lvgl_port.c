/**
 * Port LVGL: tick com hal_timer.
 * HAL_TIMER_0 (TIM2) = Modbus (manager + slave/master); usamos HAL_TIMER_1 (TIM5) para LVGL.
 */
#include "lvgl_port.h"
#include "hal_timer.h"
#include "lvgl.h"
#include <stddef.h>

#define LVGL_TICK_MS    5u
#define LVGL_TICK_TIMER HAL_TIMER_1

static hal_timer_drv_t tick_timer = NULL;

static void lvgl_tick_cb(hal_timer_drv_t t, void *ctx)
{
    (void)t;
    (void)ctx;
    lv_tick_inc(LVGL_TICK_MS);
}

bool lvgl_port_tick_init(void)
{
    hal_timer_cfg_t cfg = {
        .period     = LVGL_TICK_MS,
        .periodic    = true,
        .resolution  = HAL_TIMER_RESOLUTION_MS,
        .cb          = lvgl_tick_cb,
        .cb_ctx      = NULL,
    };

    tick_timer = hal_timer_open(LVGL_TICK_TIMER, &cfg);
    if (!tick_timer)
        return false;

    if (hal_timer_start(tick_timer) != HAL_TIMER_OK)
    {
        hal_timer_close(tick_timer);
        tick_timer = NULL;
        return false;
    }
    return true;
}

void lvgl_port_tick_deinit(void)
{
    if (tick_timer)
    {
        hal_timer_stop(tick_timer);
        hal_timer_close(tick_timer);
        tick_timer = NULL;
    }
}
