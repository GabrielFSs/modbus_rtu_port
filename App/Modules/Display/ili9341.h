#ifndef ILI9341_H
#define ILI9341_H

#include <stdint.h>

#define ILI9341_WIDTH   240
#define ILI9341_HEIGHT  320

typedef struct
{
    void (*write_cmd)(uint16_t cmd);
    void (*write_data)(uint16_t data);
    void (*write_buffer)(const uint16_t *buf, uint32_t len);
    void (*delay_ms)(uint32_t ms);
} ili9341_io_t;

void ili9341_init(const ili9341_io_t *io);

void ili9341_set_rotation(uint8_t rotation);

void ili9341_set_window(uint16_t x0,
                        uint16_t y0,
                        uint16_t x1,
                        uint16_t y1);

void ili9341_write_pixels(const uint16_t *pixels,
                          uint32_t len);

/** Preenche retângulo com cor RGB565. Requer write_buffer no io. */
void ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/** Inicializa o port (HAL display + ILI9341). Chamar antes de qualquer outra função. */
void ili9341_port_init(void);

/* ======================================================================
 * REFERÊNCIA LVGL — flush_cb do driver de display
 * ======================================================================
 * Registar em lvgl_init():
 *
 *   static lv_disp_draw_buf_t draw_buf;
 *   static lv_color_t buf1[ILI9341_WIDTH * ILI9341_HEIGHT / 10];
 *   lv_disp_draw_buf_init(&draw_buf, buf1, NULL,
 *                         ILI9341_WIDTH * ILI9341_HEIGHT / 10);
 *
 *   static lv_disp_drv_t drv;
 *   lv_disp_drv_init(&drv);
 *   drv.flush_cb = tft_flush_cb;
 *   drv.draw_buf = &draw_buf;
 *   drv.hor_res  = ILI9341_WIDTH;
 *   drv.ver_res  = ILI9341_HEIGHT;
 *   lv_disp_drv_register(&drv);
 *
 * Implementação da flush_cb:
 *
 *   void tft_flush_cb(lv_disp_drv_t *drv,
 *                     const lv_area_t *area,
 *                     lv_color_t *buf)
 *   {
 *       ili9341_set_window(area->x1, area->y1, area->x2, area->y2);
 *       uint32_t n = (area->x2 - area->x1 + 1u)
 *                  * (area->y2 - area->y1 + 1u);
 *       ili9341_write_pixels((const uint16_t *)buf, n);
 *       lv_disp_flush_ready(drv);
 *   }
 *
 * Nota DMA (opcional, muito mais rápido):
 *   Em vez de ili9341_write_pixels, usar DMA mem-to-mem para escrever
 *   directamente no endereço FSMC 0x60080000:
 *     HAL_DMA_Start(&hdma_m2m, (uint32_t)buf, 0x60080000, n);
 *     HAL_DMA_PollForTransfer(&hdma_m2m, HAL_DMA_FULL_TRANSFER, 1000);
 *   Ver projecto de referência STM32_LVGL (lcd_lvgl.c: tft_lvgl_draw_bitmap).
 *
 * Tick LVGL:
 *   Chamar lv_tick_inc(5) numa interrupção de timer a cada 5 ms.
 *   Chamar lv_timer_handler() no loop principal a cada ~5 ms.
 * ====================================================================== */

#endif
