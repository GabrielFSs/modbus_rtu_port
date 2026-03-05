/**
 * Display LVGL → ILI9341.
 * Requer: ili9341_port_init() já chamado; lv_init() já chamado.
 */
#include "lvgl_display.h"
#include "ili9341.h"
#include "lvgl.h"
#include <stddef.h>

#define BUF_PIXELS   (ILI9341_WIDTH * 32u)   /* 1/10 altura ≈ 32 linhas */
#define BUF_BYTES    (BUF_PIXELS * 2u)

static void flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    (void)disp;
    if (!px_map)
        return;
    uint32_t w = (uint32_t)(area->x2 - area->x1 + 1);
    uint32_t h = (uint32_t)(area->y2 - area->y1 + 1);
    ili9341_set_window((uint16_t)area->x1, (uint16_t)area->y1,
                       (uint16_t)area->x2, (uint16_t)area->y2);
    ili9341_write_pixels((const uint16_t *)px_map, w * h);
    lv_display_flush_ready(disp);
}

bool lvgl_display_init(void)
{
    static uint8_t buf1[BUF_BYTES];
    lv_display_t * disp = lv_display_create(ILI9341_WIDTH, ILI9341_HEIGHT);
    if (!disp)
        return false;

    lv_display_set_flush_cb(disp, flush_cb);
    lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
    lv_display_set_buffers(disp, buf1, NULL, BUF_BYTES,
                           LV_DISPLAY_RENDER_MODE_PARTIAL);
    lv_display_set_default(disp);
    return true;
}
