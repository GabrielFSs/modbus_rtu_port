#ifndef _BSP_UART_H_
#define _BSP_UART_H_

#include "stm32f4xx_hal.h"

typedef enum {
    BSP_UART_1,
    BSP_UART_2,
    BSP_UART_3
} bsp_uart_id_t;

void bsp_uart_init(bsp_uart_id_t id);
void bsp_uart_deinit(bsp_uart_id_t id);

#endif /* _BSP_UART_H_ */
