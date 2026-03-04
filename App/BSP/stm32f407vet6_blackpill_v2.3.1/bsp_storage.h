#ifndef _BSP_STORAGE_H_
#define _BSP_STORAGE_H_

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

typedef enum
{
    BSP_STORAGE_OK = 0,
    BSP_STORAGE_ERROR
} bsp_storage_status_t;

/* Inicializa SDIO + SD Card */
bsp_storage_status_t bsp_storage_init(void);

/* Desliga SDIO */
void bsp_storage_deinit(void);

/* Acesso em bloco (usado pelo diskio.c depois) */
bsp_storage_status_t bsp_storage_read_blocks(
    uint8_t *data,
    uint32_t block,
    uint32_t count);

bsp_storage_status_t bsp_storage_write_blocks(
    const uint8_t *data,
    uint32_t block,
    uint32_t count);

#endif
