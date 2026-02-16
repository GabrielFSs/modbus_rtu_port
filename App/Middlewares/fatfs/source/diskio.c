#include "ff.h"
#include "diskio.h"
#include "stm32f4xx_hal.h"

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
