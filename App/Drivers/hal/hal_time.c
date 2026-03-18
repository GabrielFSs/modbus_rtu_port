#include "hal_time.h"

/* ================= STATIC DRIVER ================= */

/* A implementação port específica (ex.: `ports/hal_time_stm32f4.c`)
 * deve exportar `HAL_TIME_DRV`. */
extern const hal_time_driver_t HAL_TIME_DRV;

static const hal_time_driver_t *time_drv = &HAL_TIME_DRV;

/* ================= INIT ================= */

void hal_time_init(void)
{
    /* no-op: time_drv já aponta para HAL_TIME_DRV */
}

/* ================= GET TIME ================= */

uint32_t hal_time_ms(void)
{
    if (time_drv && time_drv->get_ms)
        return time_drv->get_ms();

    return 0;
}

/* ================= DELAY ================= */

void hal_time_delay_ms(uint32_t ms)
{
    if (time_drv && time_drv->delay_ms)
        time_drv->delay_ms(ms);
}
