#include "ff.h"
#include "hal_rtc.h"

/* =========================================================
   FatFS requires this function if FF_FS_NORTC == 0
   It returns packed date/time for FAT
   ========================================================= */
DWORD get_fattime(void)
{
    hal_rtc_timestamp_t ts;

    if (hal_rtc_get_timestamp(&ts) != HAL_RTC_OK)
    {
        /* If RTC fails, return default date:
           01/01/2024 00:00:00 */
        return  ((DWORD)(44) << 25)  /* Year 2024 - 1980 = 44 */
              | ((DWORD)(1)  << 21)
              | ((DWORD)(1)  << 16);
    }

    uint16_t year = ts.year;

    if (year < 1980)
        year = 1980;

    year -= 1980;

    return  ((DWORD)(year)      << 25)
          | ((DWORD)(ts.month)  << 21)
          | ((DWORD)(ts.day)    << 16)
          | ((DWORD)(ts.hour)   << 11)
          | ((DWORD)(ts.min)    << 5)
          | ((DWORD)(ts.sec / 2));
}
