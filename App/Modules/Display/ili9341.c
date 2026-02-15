#include "ili9341.h"
#include "hal_display.h"
#include "stm32f4xx_hal.h"   /* apenas para HAL_Delay */

static hal_display_t disp;

/* ================= LOW LEVEL ================= */

static inline void write_cmd(uint16_t cmd)
{
    hal_display_write_cmd(disp, cmd);
}

static inline void write_data(uint16_t data)
{
    hal_display_write_data(disp, data);
}

/* ================= INIT ================= */

void ili9341_init(void)
{
    hal_display_init();
    disp = hal_display_open();

    /* Software reset */
    write_cmd(0x01);
    HAL_Delay(10);

    /* Sleep out */
    write_cmd(0x11);
    HAL_Delay(120);

    /* Pixel format = RGB565 */
    write_cmd(0x3A);
    write_data(0x55);

    /* Memory Access Control */
    write_cmd(0x36);
    write_data(0x48);   /* default orientation */

    /* Display ON */
    write_cmd(0x29);
    HAL_Delay(20);
}

/* ================= ROTATION ================= */

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

/* ================= SET WINDOW ================= */

void ili9341_set_window(uint16_t x0,
                        uint16_t y0,
                        uint16_t x1,
                        uint16_t y1)
{
    /* Column address set */
    write_cmd(0x2A);
    write_data(x0 >> 8);
    write_data(x0 & 0xFF);
    write_data(x1 >> 8);
    write_data(x1 & 0xFF);

    /* Page address set */
    write_cmd(0x2B);
    write_data(y0 >> 8);
    write_data(y0 & 0xFF);
    write_data(y1 >> 8);
    write_data(y1 & 0xFF);

    /* Memory write */
    write_cmd(0x2C);
}

/* ================= FILL SCREEN ================= */

void ili9341_fill_screen(uint16_t color)
{
    ili9341_set_window(0, 0,
                       ILI9341_WIDTH - 1,
                       ILI9341_HEIGHT - 1);

    for (uint32_t i = 0;
         i < (uint32_t)ILI9341_WIDTH * ILI9341_HEIGHT;
         i++)
    {
        write_data(color);
    }
}

/* ================= DRAW PIXEL ================= */

void ili9341_draw_pixel(uint16_t x,
                        uint16_t y,
                        uint16_t color)
{
    if (x >= ILI9341_WIDTH ||
        y >= ILI9341_HEIGHT)
        return;

    ili9341_set_window(x, y, x, y);
    write_data(color);
}
