#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

typedef enum
{
    BSP_SPI_1 = 0,
    BSP_SPI_2,
    BSP_SPI_3
} bsp_spi_id_t;

void bsp_spi_init(bsp_spi_id_t id,
                  bool enable_irq,
                  bool enable_dma);

void bsp_spi_deinit(bsp_spi_id_t id);

#endif
