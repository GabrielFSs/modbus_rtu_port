#include "main.h"
#include "Modules/Display/ili9341.h"

#include <stdint.h>

/* Cores RGB565: verde, azul, vermelho. */
#define COLOR_GREEN  0x07E0u
#define COLOR_BLUE   0x001Fu
#define COLOR_RED    0xF800u

#define INTERVAL_MS  5000u

void app_setup(void)
{
    ili9341_port_init();
    ili9341_set_rotation(0);

    /* Primeira cor já na inicialização (verde). */
    ili9341_fill_rect(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, COLOR_GREEN);
}

void app_loop(void)
{
    static uint32_t last_tick = 0u;
    static uint8_t  color_idx = 1u;  /* próxima cor = azul (verde já foi no setup) */

    uint32_t now = HAL_GetTick();
    if (now - last_tick < INTERVAL_MS)
        return;

    last_tick = now;

    uint16_t color;
    switch (color_idx)
    {
        case 0u: color = COLOR_GREEN; break;
        case 1u: color = COLOR_BLUE;  break;
        default: color = COLOR_RED;   break;
    }

    ili9341_fill_rect(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, color);

    color_idx++;
    if (color_idx >= 3u)
        color_idx = 0u;
}
