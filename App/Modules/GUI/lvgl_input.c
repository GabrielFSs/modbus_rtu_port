/**
 * Input LVGL → XPT2046 (toque).
 * Requer: hal_spi_init(), hal_gpio_init(); lv_display_default já criado.
 */
#include "lvgl_input.h"
#include "lvgl.h"
#include "bsp_spi.h"
#include "hal_spi.h"
#include "hal_gpio.h"
#include <stddef.h>

static hal_spi_drv_t  spi_drv  = NULL;
static hal_gpio_drv_t gpio_cs  = NULL;
static hal_gpio_drv_t gpio_irq = NULL;

static bool is_touched(void)
{
    if (!gpio_irq)
        return false;
    bool v = false;
    hal_gpio_read(gpio_irq, &v);
    return v;
}

static bool read_raw(uint16_t *rx, uint16_t *ry)
{
    if (!spi_drv || !gpio_cs || !rx || !ry || !is_touched())
        return false;

    const uint8_t zeros[2] = { 0, 0 };
    uint8_t r[2];
    uint32_t sx = 0, sy = 0;
    uint8_t n = 0;

    hal_gpio_write(gpio_cs, false);
    for (uint8_t i = 0; i < 16 && is_touched(); i++)
    {
        uint8_t cmd_y = 0x90u;
        hal_spi_transmit(spi_drv, &cmd_y, 1, 100);
        hal_spi_transmit_receive(spi_drv, (const uint8_t *)zeros, r, 2, 100);
        sy += (uint32_t)(((uint16_t)r[0] << 8) | (uint16_t)r[1]);

        uint8_t cmd_x = 0xD0u;
        hal_spi_transmit(spi_drv, &cmd_x, 1, 100);
        hal_spi_transmit_receive(spi_drv, (const uint8_t *)zeros, r, 2, 100);
        sx += (uint32_t)(((uint16_t)r[0] << 8) | (uint16_t)r[1]);
        n++;
    }
    hal_gpio_write(gpio_cs, true);

    if (n < 16)
        return false;
    *rx = (uint16_t)(sx / 16);
    *ry = (uint16_t)(sy / 16);
    return true;
}

static void raw_to_display(uint16_t rx, uint16_t ry, int16_t *cx, int16_t *cy)
{
    if (rx < XPT2046_CAL_X_RIGHT)  rx = XPT2046_CAL_X_RIGHT;
    if (rx > XPT2046_CAL_X_LEFT)   rx = XPT2046_CAL_X_LEFT;
    if (ry < XPT2046_CAL_Y_TOP)    ry = XPT2046_CAL_Y_TOP;
    if (ry > XPT2046_CAL_Y_BOTTOM) ry = XPT2046_CAL_Y_BOTTOM;

    int32_t x = (int32_t)(XPT2046_CAL_X_LEFT - rx) * 240
                / (int32_t)(XPT2046_CAL_X_LEFT - XPT2046_CAL_X_RIGHT);
    int32_t y = (int32_t)(ry - XPT2046_CAL_Y_TOP) * 320
                / (int32_t)(XPT2046_CAL_Y_BOTTOM - XPT2046_CAL_Y_TOP);

    *cx = (int16_t)(x < 0 ? 0 : x > 239 ? 239 : x);
    *cy = (int16_t)(y < 0 ? 0 : y > 319 ? 319 : y);
}

static void touch_read_cb(lv_indev_t * indev, lv_indev_data_t * data)
{
    (void)indev;
    static int16_t last_x = 0, last_y = 0;

    if (is_touched())
    {
        uint16_t rx, ry;
        if (read_raw(&rx, &ry))
            raw_to_display(rx, ry, &last_x, &last_y);
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
        data->state = LV_INDEV_STATE_RELEASED;

    data->point.x = last_x;
    data->point.y = last_y;
}

bool lvgl_input_init(void)
{
    hal_gpio_cfg_t gcfg = {
        .direction  = HAL_GPIO_OUTPUT,
        .pull       = HAL_GPIO_NOPULL,
        .out_type   = HAL_GPIO_PUSH_PULL,
        .irq_edge   = HAL_GPIO_IRQ_NONE,
    };
    gpio_cs = hal_gpio_open(HAL_GPIO_TOUCH_CS, &gcfg);
    if (!gpio_cs)
        return false;
    hal_gpio_write(gpio_cs, true);

    gcfg.direction = HAL_GPIO_INPUT;
    gpio_irq = hal_gpio_open(HAL_GPIO_TOUCH_TINT, &gcfg);
    if (!gpio_irq)
        return false;

    hal_spi_cfg_t scfg = {
        .mode      = HAL_SPI_MODE_MASTER,
        .cpol      = HAL_SPI_CPOL_LOW,
        .cpha      = HAL_SPI_CPHA_1EDGE,
        .bit_order = HAL_SPI_MSB_FIRST,
        .datasize  = HAL_SPI_DATASIZE_8BIT,
        .xfer_mode = HAL_SPI_XFER_POLLING,
        .baudrate_hz = 650000u,
        .cb        = NULL,
        .cb_ctx    = NULL,
    };
    spi_drv = hal_spi_open(HAL_SPI_2, &scfg);
    if (!spi_drv)
        return false;

    lv_indev_t * indev = lv_indev_create();
    if (!indev)
        return false;

    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touch_read_cb);
    lv_indev_set_display(indev, lv_display_get_default());
    return true;
}
