#include "ff.h"
#include "diskio.h"
#include "stm32f4xx_hal.h"
#include "hal_rtc.h"

extern SD_HandleTypeDef hsd;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(BYTE pdrv)
{
    if (pdrv != 0)
        return STA_NOINIT;

    if (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER)
        return STA_NOINIT;

    return 0;
}

/*-----------------------------------------------------------------------*/
/* Initialize Drive                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv)
{
    if (pdrv != 0)
        return STA_NOINIT;

    if (HAL_SD_Init(&hsd) != HAL_OK)
        return STA_NOINIT;

    return 0;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv,
                  BYTE *buff,
                  LBA_t sector,
                  UINT count)
{
    if (pdrv != 0 || !buff || count == 0)
        return RES_PARERR;

    if (HAL_SD_ReadBlocks(&hsd,
                          buff,
                          sector,
                          count,
                          HAL_MAX_DELAY) != HAL_OK)
        return RES_ERROR;

    while (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER);

    return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write(BYTE pdrv,
                   const BYTE *buff,
                   LBA_t sector,
                   UINT count)
{
    if (pdrv != 0 || !buff || count == 0)
        return RES_PARERR;

    if (HAL_SD_WriteBlocks(&hsd,
                           (uint8_t*)buff,
                           sector,
                           count,
                           HAL_MAX_DELAY) != HAL_OK)
        return RES_ERROR;

    while (HAL_SD_GetCardState(&hsd) != HAL_SD_CARD_TRANSFER);

    return RES_OK;
}

#endif

/*-----------------------------------------------------------------------*/
/* IOCTL                                                                 */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    if (pdrv != 0)
        return RES_PARERR;

    switch (cmd)
    {
        case CTRL_SYNC:
            return RES_OK;

        case GET_SECTOR_COUNT:
        {
            HAL_SD_CardInfoTypeDef info;
            HAL_SD_GetCardInfo(&hsd, &info);
            *(DWORD*)buff = info.LogBlockNbr;
            return RES_OK;
        }

        case GET_SECTOR_SIZE:
        {
            HAL_SD_CardInfoTypeDef info;
            HAL_SD_GetCardInfo(&hsd, &info);
            *(WORD*)buff = info.LogBlockSize;
            return RES_OK;
        }

        case GET_BLOCK_SIZE:
            *(DWORD*)buff = 1;
            return RES_OK;
    }

    return RES_PARERR;
}

/*-----------------------------------------------------------------------*/
/* Get current time for FatFs (timestamp em ficheiros)                   */
/* Formato: (ano-1980)<<25 | mes<<21 | dia<<16 | hora<<11 | min<<5 | seg/2 */
/*-----------------------------------------------------------------------*/

#if !FF_FS_READONLY && !FF_FS_NORTC

DWORD get_fattime(void)
{
    hal_rtc_date_t date;
    hal_rtc_time_t time;
    DWORD t = 0;

    if (hal_rtc_get_date(&date) != HAL_RTC_OK ||
        hal_rtc_get_time(&time) != HAL_RTC_OK)
    {
        /* RTC não disponível: devolver data fixa (ex.: 01/01/2025 00:00:00) */
        return (DWORD)((2025 - 1980) << 25) | ((DWORD)1 << 21) | ((DWORD)1 << 16);
    }

    t  = (DWORD)(date.year >= 1980 ? date.year - 1980 : 0) << 25;
    t |= (DWORD)(date.month >= 1 && date.month <= 12 ? date.month : 1) << 21;
    t |= (DWORD)(date.day >= 1 && date.day <= 31 ? date.day : 1) << 16;
    t |= (DWORD)(time.hour <= 23 ? time.hour : 0) << 11;
    t |= (DWORD)(time.min <= 59 ? time.min : 0) << 5;
    t |= (DWORD)(time.sec <= 59 ? time.sec / 2 : 0);

    return t;
}

#endif
