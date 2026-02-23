#include "hal_time.h"
#include "stm32f4xx_hal.h"

static uint32_t stm32_get_ms(void)
{
    return HAL_GetTick();
}

static void stm32_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}

const hal_time_driver_t HAL_TIME_DRIVER =
{
    .get_ms   = stm32_get_ms,
    .delay_ms = stm32_delay_ms
};
