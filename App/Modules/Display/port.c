#include "ili9341.h"
#include "hal_display.h"
#include "hal_time.h"

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
    hal_time_delay_ms(ms);
}

/* ================= PORT INIT ================= */

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
