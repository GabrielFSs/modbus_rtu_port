#ifndef ILI9341_H
#define ILI9341_H

#include <stdint.h>
#include <stdbool.h>

/* ================= RESOLUTION ================= */

#define ILI9341_WIDTH   240
#define ILI9341_HEIGHT  320

/* ================= COLOR FORMAT ================= */
/* RGB565 */

#define ILI9341_COLOR_BLACK   0x0000
#define ILI9341_COLOR_WHITE   0xFFFF
#define ILI9341_COLOR_RED     0xF800
#define ILI9341_COLOR_GREEN   0x07E0
#define ILI9341_COLOR_BLUE    0x001F

/* ================= API ================= */

void ili9341_init(void);

void ili9341_set_rotation(uint8_t rotation);

void ili9341_fill_screen(uint16_t color);

void ili9341_draw_pixel(uint16_t x,
                        uint16_t y,
                        uint16_t color);

void ili9341_set_window(uint16_t x0,
                        uint16_t y0,
                        uint16_t x1,
                        uint16_t y1);

#endif
