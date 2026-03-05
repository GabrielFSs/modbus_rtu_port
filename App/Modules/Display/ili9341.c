#include "ili9341.h"

static ili9341_io_t io;

static inline void write_cmd(uint16_t cmd)
{
    io.write_cmd(cmd);
}

static inline void write_data(uint16_t data)
{
    io.write_data(data);
}

void ili9341_init(const ili9341_io_t *io_cfg)
{
    io = *io_cfg;

    write_cmd(0x01);
    io.delay_ms(10);

    write_cmd(0x11);
    io.delay_ms(120);

    write_cmd(0x3A);
    write_data(0x55);

    write_cmd(0x36);
    write_data(0x48);

    write_cmd(0x29);
    io.delay_ms(20);
}

void ili9341_set_rotation(uint8_t rotation)
{
    write_cmd(0x36);

    switch (rotation)
    {
        case 0: write_data(0x48); break;
        case 1: write_data(0x28); break;
        case 2: write_data(0x88); break;
        case 3: write_data(0xE8); break;
        default: write_data(0x48); break;
    }
}

void ili9341_set_window(uint16_t x0,
                        uint16_t y0,
                        uint16_t x1,
                        uint16_t y1)
{
    write_cmd(0x2A);
    write_data(x0 >> 8);
    write_data(x0 & 0xFF);
    write_data(x1 >> 8);
    write_data(x1 & 0xFF);

    write_cmd(0x2B);
    write_data(y0 >> 8);
    write_data(y0 & 0xFF);
    write_data(y1 >> 8);
    write_data(y1 & 0xFF);

    write_cmd(0x2C);
}

void ili9341_write_pixels(const uint16_t *pixels,
                          uint32_t len)
{
    if (io.write_buffer && pixels)
        io.write_buffer(pixels, len);
}

#define ILI9341_MAX_FILL_CHUNK  240u  /* linha completa 240px (teste; antes 320) */

void ili9341_fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    if (w == 0 || h == 0 || !io.write_buffer)
        return;

    ili9341_set_window(x, y, (uint16_t)(x + w - 1), (uint16_t)(y + h - 1));

    uint32_t total = (uint32_t)w * (uint32_t)h;
    static uint16_t line_buf[ILI9341_MAX_FILL_CHUNK];

    uint16_t fill_w = (w < ILI9341_MAX_FILL_CHUNK) ? w : ILI9341_MAX_FILL_CHUNK;
    for (uint16_t i = 0; i < fill_w; i++)
        line_buf[i] = color;

    uint32_t sent = 0;
    while (sent < total)
    {
        uint32_t chunk = total - sent;
        if (chunk > ILI9341_MAX_FILL_CHUNK)
            chunk = ILI9341_MAX_FILL_CHUNK;
        io.write_buffer(line_buf, chunk);
        sent += chunk;
    }
}
