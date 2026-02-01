#include "bsp.h"

void bsp_clock_init(void);
void bsp_gpio_init(void);

void bsp_init(void)
{
    bsp_clock_init();
    bsp_gpio_init();
}
