#ifndef _BSP_SPI_H_
#define _BSP_SPI_H_

#include "stm32f4xx_hal.h"

typedef enum
{
    BSP_SPI_1 = 0
} bsp_spi_id_t;

void bsp_spi_init(bsp_spi_id_t id);
void bsp_spi_deinit(bsp_spi_id_t id);

#endif /* _BSP_SPI_H_ */
