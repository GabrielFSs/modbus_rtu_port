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

/* ======================================================================
 * XPT2046 — Calibração + protocolo + referência LVGL read_cb
 * Painel: DevBox 3.2" ILI9341 240x320 (mesma placa STM32F407VET6 Black)
 * ======================================================================
 *
 * PINOS (confirmados por STM32_LVGL.ioc e Tetris STM32 project):
 *   SPI2:  PB13=SCK, PB14=MISO, PB15=MOSI  (BSP_SPI_2, AF5)
 *   T_CS:  PB12  (HAL_GPIO_TOUCH_CS,  active_high=true  no BSP)
 *   T_IRQ: PC5   (HAL_GPIO_TOUCH_TINT, active_high=false no BSP)
 *
 * VELOCIDADE SPI:
 *   PCLK1 = 42 MHz / 64 = 656 kHz  (datasheet recomenda < 2.5 MHz)
 *   CPOL=0, CPHA=0 (Mode 0), 8-bit, polling
 *
 * PROTOCOLO POR EIXO (24 clocks totais), Y LIDO ANTES de X:
 *   hal_spi_transmit(cmd, 1)              → 8 clocks, cmd no MOSI
 *   hal_spi_transmit_receive(zeros,r,2)   → 16 clocks, r[0]=MSB, r[1]=LSB
 *   raw16 = (r[0]<<8)|r[1]               → sem >>3 (igual XPT2046_touch.c)
 *   CS fica LOW durante todos os 16 samples do toque
 *
 * COMANDOS:  READ_Y=0x90, READ_X=0xD0
 *
 * T_PEN: hal_gpio_read(TOUCH_TINT) devolve true=tocado, false=solto.
 *   active_high=false já aplica inversão — NUNCA usar !level.
 *
 * CONVERSÃO raw16 → pixel (portrait 0°, USB em baixo):
 *   X INVERTIDO: cx = (X_LEFT - raw_x) * 240 / (X_LEFT - X_RIGHT)
 *   Y NORMAL:    cy = (raw_y - Y_TOP)   * 320 / (Y_BOTTOM - Y_TOP)
 * ====================================================================== */
#define XPT2046_CAL_X_LEFT    27970u   /* raw16 borda esquerda  → display X=0   */
#define XPT2046_CAL_X_RIGHT   2815u    /* raw16 borda direita   → display X=239 */
#define XPT2046_CAL_Y_TOP     2317u    /* raw16 borda superior  → display Y=0   */
#define XPT2046_CAL_Y_BOTTOM  29199u   /* raw16 borda inferior  → display Y=319 */

/* ======================================================================
 * REFERÊNCIA LVGL — read_cb do driver de input
 * ======================================================================
 * Registar em lvgl_touch_init():
 *
 *   static lv_indev_drv_t indev_drv;
 *   lv_indev_drv_init(&indev_drv);
 *   indev_drv.type    = LV_INDEV_TYPE_POINTER;
 *   indev_drv.read_cb = touch_read_cb;
 *   lv_indev_drv_register(&indev_drv);
 *
 * Implementação das 3 funções necessárias:
 *
 * [1] is_touched() — devolve true se dedo na tela:
 *   bool is_touched(void) {
 *       bool v = false;
 *       hal_gpio_read(touch_irq_drv, &v);
 *       return v;  // NÃO usar !v
 *   }
 *
 * [2] read_raw(rx, ry) — 16 samples com CS LOW, Y antes de X:
 *   bool read_raw(uint16_t *rx, uint16_t *ry) {
 *       if (!is_touched()) return false;
 *       uint32_t sx=0, sy=0; uint8_t n=0;
 *       const uint8_t zeros[2]={0,0}; uint8_t r[2];
 *       hal_gpio_write(cs, false);                  // CS LOW
 *       for (uint8_t i=0; i<16 && is_touched(); i++) {
 *           n++;
 *           hal_spi_transmit(spi, (uint8_t[]){0x90}, 1, 100);  // READ_Y
 *           hal_spi_transmit_receive(spi, zeros, r, 2, 100);
 *           sy += ((uint16_t)r[0]<<8)|r[1];
 *           hal_spi_transmit(spi, (uint8_t[]){0xD0}, 1, 100);  // READ_X
 *           hal_spi_transmit_receive(spi, zeros, r, 2, 100);
 *           sx += ((uint16_t)r[0]<<8)|r[1];
 *       }
 *       hal_gpio_write(cs, true);                   // CS HIGH
 *       if (n < 16) return false;
 *       *rx = (uint16_t)(sx/16); *ry = (uint16_t)(sy/16);
 *       return true;
 *   }
 *
 * [3] raw_to_display(rx, ry, cx, cy) — conversão com calibração:
 *   void raw_to_display(uint16_t rx, uint16_t ry,
 *                       int16_t *cx, int16_t *cy) {
 *       if (rx < XPT2046_CAL_X_RIGHT) rx = XPT2046_CAL_X_RIGHT;
 *       if (rx > XPT2046_CAL_X_LEFT)  rx = XPT2046_CAL_X_LEFT;
 *       if (ry < XPT2046_CAL_Y_TOP)   ry = XPT2046_CAL_Y_TOP;
 *       if (ry > XPT2046_CAL_Y_BOTTOM)ry = XPT2046_CAL_Y_BOTTOM;
 *       int32_t x = (int32_t)(XPT2046_CAL_X_LEFT - rx) * 240
 *                   / (int32_t)(XPT2046_CAL_X_LEFT - XPT2046_CAL_X_RIGHT);
 *       int32_t y = (int32_t)(ry - XPT2046_CAL_Y_TOP) * 320
 *                   / (int32_t)(XPT2046_CAL_Y_BOTTOM - XPT2046_CAL_Y_TOP);
 *       *cx = (int16_t)(x < 0 ? 0 : x > 239 ? 239 : x);
 *       *cy = (int16_t)(y < 0 ? 0 : y > 319 ? 319 : y);
 *   }
 *
 * [4] touch_read_cb (registar em LVGL como read_cb):
 *   void touch_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data) {
 *       static int16_t last_x=0, last_y=0;
 *       if (is_touched()) {
 *           uint16_t rx, ry;
 *           if (read_raw(&rx, &ry))
 *               raw_to_display(rx, ry, &last_x, &last_y);
 *           data->state = LV_INDEV_STATE_PRESSED;
 *       } else {
 *           data->state = LV_INDEV_STATE_RELEASED;
 *       }
 *       data->point.x = last_x;
 *       data->point.y = last_y;
 *   }
 * ====================================================================== */

#endif
