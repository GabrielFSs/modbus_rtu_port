#include "bsp_rtc.h"
#include "stm32f4xx_hal.h"

bool bsp_rtc_init(void)
{
    /* Habilita PWR e acesso ao Backup Domain */
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();

    /* Ativa LSE */
    __HAL_RCC_LSE_CONFIG(RCC_LSE_ON);

    /* Aguarda estabilizar */
    while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSERDY) == RESET);

    /* Seleciona LSE como clock do RTC */
    __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);

    /* Habilita RTC */
    __HAL_RCC_RTC_ENABLE();

    return true;
}
