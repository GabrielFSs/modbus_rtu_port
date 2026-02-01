#include "stm32f4xx.h"

void bsp_gpio_init(void)
{
    /* Enable GPIO clocks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    /* Example: LED PA5 */
    GPIOA->MODER &= ~(3 << (5 * 2));
    GPIOA->MODER |=  (1 << (5 * 2));   /* Output */

    GPIOA->OTYPER &= ~(1 << 5);
    GPIOA->OSPEEDR |= (3 << (5 * 2));
}
