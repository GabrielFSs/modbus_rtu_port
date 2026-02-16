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

#endif
