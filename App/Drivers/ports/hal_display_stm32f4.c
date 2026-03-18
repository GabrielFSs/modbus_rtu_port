/**
 * Port display para STM32F4: seleção do driver (FSMC ou GPIO) em compile-time.
 * Fica em ports/ junto com hal_display_fsmc_stm32f4.c.
 */
#include "hal_display.h"

#if defined(DISPLAY_USE_FSMC)

extern hal_display_drv_imp_t HAL_DISPLAY_FSMC;
hal_display_drv_imp_t *HAL_DISPLAY_DRV = &HAL_DISPLAY_FSMC;

#elif defined(DISPLAY_USE_GPIO)

extern hal_display_drv_imp_t HAL_DISPLAY_GPIO;
hal_display_drv_imp_t *HAL_DISPLAY_DRV = &HAL_DISPLAY_GPIO;

#endif
