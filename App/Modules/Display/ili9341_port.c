/**
 * Port ILI9341 → hal_display (STM32F4): liga o driver ili9341 ao HAL de display.
 */
#include "ili9341.h"
#include "hal_display.h"
#include "stm32f4xx_hal.h"

static hal_display_t disp;

/* ================= PORT FUNCTIONS ================= */

static void port_write_cmd(uint16_t cmd)
{
    hal_display_write_cmd(disp, cmd);
}

static void port_write_data(uint16_t data)
{
    hal_display_write_data(disp, data);
}

static void port_delay(uint32_t ms)
 {
    HAL_Delay(ms);
}

static void port_write_buffer(const uint16_t *buf, uint32_t len)
{
    if (buf && disp)
        hal_display_write_buffer(disp, buf, len);
}

/* ================= PORT INIT ================= */

void ili9341_port_init(void)
{
    hal_display_init();
    disp = hal_display_open();

    static ili9341_io_t io =
    {
        .write_cmd    = port_write_cmd,
        .write_data   = port_write_data,
        .write_buffer = port_write_buffer,
        .delay_ms     = port_delay
    };

    ili9341_init(&io);
}
