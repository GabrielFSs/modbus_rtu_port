/**
 * Port LVGL: tick via hal_timer (HAL_TIMER_1 = TIM5).
 * Display e input device ficam noutro módulo ou em app.
 */
#ifndef LVGL_PORT_H
#define LVGL_PORT_H

#include <stdbool.h>

/** Inicializa o tick do LVGL usando hal_timer (TIM5, 5 ms, periódico).
 *  Chamar após lv_init() e hal_timer_init().
 *  Retorna true se o timer foi arrancado. */
bool lvgl_port_tick_init(void);

/** Para o timer do tick (opcional, para poupar energia). */
void lvgl_port_tick_deinit(void);

#endif
