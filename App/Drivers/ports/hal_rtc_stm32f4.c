#include "hal_rtc.h"
#include "bsp_rtc.h"
#include "stm32f4xx_hal.h"
#include <string.h>

/* ===== HANDLE ===== */
static RTC_HandleTypeDef hrtc;

/* ===== INIT ===== */
static hal_rtc_status_t stm32_rtc_init(void)
{
    if (!bsp_rtc_init())
        return HAL_RTC_ERROR;

    hrtc.Instance = RTC;
    hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
    hrtc.Init.AsynchPrediv = 127;
    hrtc.Init.SynchPrediv  = 255;
    hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
    hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

    if (HAL_RTC_Init(&hrtc) != HAL_OK)
        return HAL_RTC_ERROR;

    return HAL_RTC_OK;
}

/* ===== SET TIME ===== */
static hal_rtc_status_t stm32_rtc_set_time(const hal_rtc_time_t *time)
{
    RTC_TimeTypeDef t = {0};

    if (!time)
        return HAL_RTC_ERROR;

    t.Hours   = time->hour;
    t.Minutes = time->min;
    t.Seconds = time->sec;

    if (HAL_RTC_SetTime(&hrtc, &t, RTC_FORMAT_BIN) != HAL_OK)
        return HAL_RTC_ERROR;

    return HAL_RTC_OK;
}

/* ===== GET TIME ===== */
static hal_rtc_status_t stm32_rtc_get_time(hal_rtc_time_t *time)
{
    RTC_TimeTypeDef t;

    if (!time)
        return HAL_RTC_ERROR;

    HAL_RTC_GetTime(&hrtc, &t, RTC_FORMAT_BIN);

    time->hour = t.Hours;
    time->min  = t.Minutes;
    time->sec  = t.Seconds;

    return HAL_RTC_OK;
}

/* ===== SET DATE ===== */
static hal_rtc_status_t stm32_rtc_set_date(const hal_rtc_date_t *date)
{
    RTC_DateTypeDef d = {0};

    if (!date)
        return HAL_RTC_ERROR;

    d.Date  = date->day;
    d.Month = date->month;
    d.Year  = (uint8_t)(date->year - 2000);
    d.WeekDay = RTC_WEEKDAY_MONDAY; /* opcional */

    if (HAL_RTC_SetDate(&hrtc, &d, RTC_FORMAT_BIN) != HAL_OK)
        return HAL_RTC_ERROR;

    return HAL_RTC_OK;
}

/* ===== GET DATE ===== */
static hal_rtc_status_t stm32_rtc_get_date(hal_rtc_date_t *date)
{
    RTC_DateTypeDef d;

    if (!date)
        return HAL_RTC_ERROR;

    HAL_RTC_GetDate(&hrtc, &d, RTC_FORMAT_BIN);

    date->day   = d.Date;
    date->month = d.Month;
    date->year  = 2000 + d.Year;

    return HAL_RTC_OK;
}

/* ===== DRIVER EXPORT ===== */
hal_rtc_drv_imp_t HAL_RTC_DRV =
{
    .init     = stm32_rtc_init,
    .set_time = stm32_rtc_set_time,
    .get_time = stm32_rtc_get_time,
    .set_date = stm32_rtc_set_date,
    .get_date = stm32_rtc_get_date
};
