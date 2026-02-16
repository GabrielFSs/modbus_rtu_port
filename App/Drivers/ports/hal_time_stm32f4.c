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

/* ================= DRIVER INSTANCE ================= */

static const hal_time_driver_t stm32_time_driver =
{
    .get_ms   = stm32_get_ms,
    .delay_ms = stm32_delay_ms
};

/* ================= PUBLIC INIT ================= */

void hal_time_stm32f4_init(void)
{
    hal_time_init(&stm32_time_driver);
}
