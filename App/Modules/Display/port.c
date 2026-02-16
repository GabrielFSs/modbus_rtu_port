#include <hal_time.h>
#include "stm32f4xx_hal.h"
#include "ili9341.h"

static hal_display_t disp;

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

void ili9341_port_init(void)
{
    hal_display_init();
    disp = hal_display_open();

    static ili9341_io_t io =
    {
        .write_cmd  = port_write_cmd,
        .write_data = port_write_data,
        .delay_ms   = port_delay
    };

    ili9341_init(&io);
}
