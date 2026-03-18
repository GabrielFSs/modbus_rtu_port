#include "hal_time.h"
#include "stm32f4xx_hal.h"

/* ================= LOW LEVEL ================= */

static uint32_t stm32_get_ms(void)
{
    return HAL_GetTick();
}

static void stm32_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

/* ================= DRIVER INSTANCE (export) =================
 * App/Drivers/hal/hal_time.c consome `HAL_TIME_DRV` diretamente.
 * Isso elimina a necessidade de `hal_time_stm32f4_init()` e de qualquer
 * header específico no app.
 */
const hal_time_driver_t HAL_TIME_DRV =
{
    .get_ms   = stm32_get_ms,
    .delay_ms = stm32_delay_ms
};
