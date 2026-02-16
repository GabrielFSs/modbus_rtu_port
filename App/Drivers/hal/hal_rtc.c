#include "hal_rtc.h"

static hal_rtc_drv_imp_t *drv = &HAL_RTC_DRV;

hal_rtc_status_t hal_rtc_init(void)
{
    if (drv->init)
        return drv->init();

    return HAL_RTC_ERROR;
}

hal_rtc_status_t hal_rtc_set_time(const hal_rtc_time_t *time)
{
    if (drv->set_time)
        return drv->set_time(time);

    return HAL_RTC_ERROR;
}

hal_rtc_status_t hal_rtc_get_time(hal_rtc_time_t *time)
{
    if (drv->get_time)
        return drv->get_time(time);

    return HAL_RTC_ERROR;
}

hal_rtc_status_t hal_rtc_set_date(const hal_rtc_date_t *date)
{
    if (drv->set_date)
        return drv->set_date(date);

    return HAL_RTC_ERROR;
}

hal_rtc_status_t hal_rtc_get_date(hal_rtc_date_t *date)
{
    if (drv->get_date)
        return drv->get_date(date);

    return HAL_RTC_ERROR;
}

hal_rtc_status_t (*get_timestamp)(hal_rtc_timestamp_t *ts);

hal_rtc_status_t hal_rtc_get_timestamp(hal_rtc_timestamp_t *ts)
{
    if (drv->get_timestamp)
        return drv->get_timestamp(ts);

    return HAL_RTC_ERROR;
}

