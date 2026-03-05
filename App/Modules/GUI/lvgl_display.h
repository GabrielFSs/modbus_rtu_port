/**
 * Display LVGL → ILI9341 (flush_cb).
 * Chamar lvgl_display_init() após ili9341_port_init() e lv_init().
 */
#ifndef LVGL_DISPLAY_H
#define LVGL_DISPLAY_H

#include <stdbool.h>

/** Cria e regista o lv_display com flush_cb para o ILI9341.
 *  Retorna true se ok. */
bool lvgl_display_init(void);

#endif
