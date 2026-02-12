#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include <stdbool.h>
#include <stdint.h>

#include "hal_gpio.h"
#include "stm32f4xx_hal.h"

/*
 * BSP GPIO
 *
 * Responsável por:
 *  - Mapear hal_gpio_id_t → GPIOx / PIN
 *  - Habilitar clocks
 *  - Configurar NVIC para EXTI
 */

/* Retorna porta e pino físico a partir do ID lógico */
bool bsp_gpio_get_pin(hal_gpio_id_t id,
                      GPIO_TypeDef **port,
                      uint16_t *pin);

bool bsp_gpio_get_active_level(hal_gpio_id_t id);

/* Habilita IRQ EXTI para o pino */
void bsp_gpio_enable_irq(uint16_t pin);

#endif /* BSP_GPIO_H */
