#include "hal_time.h"

/* Driver externo */
extern const hal_time_driver_t HAL_TIME_DRIVER;

/* ================= STATIC DRIVER ================= */

static const hal_time_driver_t *time_drv = &HAL_TIME_DRIVER;

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
