/**
 * Input LVGL → XPT2046 (read_cb pointer).
 * Chamar lvgl_input_init() após lv_display criado e BSP/HAL (SPI2, GPIO touch) inicializados.
 */
#ifndef LVGL_INPUT_H
#define LVGL_INPUT_H

#include <stdbool.h>

/** Cria e regista o lv_indev pointer com read_cb para o XPT2046.
 *  Retorna true se ok. */
bool lvgl_input_init(void);

#endif
