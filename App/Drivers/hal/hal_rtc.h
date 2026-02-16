#ifndef _HAL_RTC_H_
#define _HAL_RTC_H_

#include <stdint.h>
#include <stdbool.h>

/* ===== STATUS ===== */
typedef enum
{
    HAL_RTC_OK,
    HAL_RTC_ERROR
} hal_rtc_status_t;

/* ===== TIME ===== */
typedef struct
{
    uint8_t sec;
    uint8_t min;
    uint8_t hour;
} hal_rtc_time_t;

/* ===== DATE ===== */
typedef struct
{
    uint8_t day;
    uint8_t month;
    uint16_t year;
} hal_rtc_date_t;

/* ===== TIMESTAMP ===== */
typedef struct
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;

    uint8_t  hour;
    uint8_t  min;
    uint8_t  sec;

    uint16_t ms;
} hal_rtc_timestamp_t;

/* ===== DRIVER API ===== */
typedef struct
{
    hal_rtc_status_t (*init)(void);

    hal_rtc_status_t (*set_time)(const hal_rtc_time_t *time);
    hal_rtc_status_t (*get_time)(hal_rtc_time_t *time);

    hal_rtc_status_t (*set_date)(const hal_rtc_date_t *date);
    hal_rtc_status_t (*get_date)(hal_rtc_date_t *date);

    hal_rtc_status_t (*get_timestamp)(hal_rtc_timestamp_t *ts);

} hal_rtc_drv_imp_t;

/* ===== DRIVER INSTANCE ===== */
extern hal_rtc_drv_imp_t HAL_RTC_DRV;

/* ===== HAL API ===== */
hal_rtc_status_t hal_rtc_init(void);

hal_rtc_status_t hal_rtc_set_time(const hal_rtc_time_t *time);
hal_rtc_status_t hal_rtc_get_time(hal_rtc_time_t *time);

hal_rtc_status_t hal_rtc_set_date(const hal_rtc_date_t *date);
hal_rtc_status_t hal_rtc_get_date(hal_rtc_date_t *date);

hal_rtc_status_t hal_rtc_get_timestamp(hal_rtc_timestamp_t *ts);

#endif
