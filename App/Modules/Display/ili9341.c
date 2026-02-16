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
    io.write_buffer(pixels, len);
}
