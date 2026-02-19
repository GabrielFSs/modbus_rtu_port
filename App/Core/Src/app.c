#include "hal_timer.h"
#include "hal_storage.h"
#include "hal_rtc.h"

#include <stdio.h>
#include <string.h>

static hal_timer_drv_t s_timer;

/* ============================================================
 * TIMER CALLBACK (5s)
 * ============================================================*/
static void timer_cb(hal_timer_drv_t timer, void *ctx)
{
    (void)timer;
    (void)ctx;

    hal_rtc_timestamp_t ts;

    if (hal_rtc_get_timestamp(&ts) != HAL_RTC_OK)
        return;

    /* Formata timestamp completo */
    char line[128];

    int len = snprintf(line,
                       sizeof(line),
                       "[%04u-%02u-%02u %02u:%02u:%02u.%03u] Timer tick 5s\r\n",
                       ts.year,
                       ts.month,
                       ts.day,
                       ts.hour,
                       ts.min,
                       ts.sec,
                       ts.ms);

    if (len <= 0)
        return;

    /* 🔥 APPEND no log */
    hal_storage_append_file("debug.log",
                            (uint8_t*)line,
                            len);
}

/* ============================================================
 * APP SETUP
 * ============================================================*/
void app_setup(void)
{
    hal_storage_init();
    hal_rtc_init();
    hal_timer_init();

    hal_timer_cfg_t cfg =
    {
        .period     = 5000,
        .periodic   = true,
        .resolution = HAL_TIMER_RESOLUTION_MS,
        .cb         = timer_cb,
        .cb_ctx     = NULL
    };

    s_timer = hal_timer_open(HAL_TIMER_0, &cfg);

    if (s_timer)
        hal_timer_start(s_timer);
}

/* ============================================================
 * APP LOOP
 * ============================================================*/
void app_loop(void)
{
    /* Tudo interrupt-driven */
}
