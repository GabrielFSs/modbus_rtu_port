#include "app.h"
#include "modbus_manager.h"
#include "mb_sniffer.h"
#include "lvgl.h"
#include "lvgl_port.h"
#include "lvgl_display.h"
#include "lvgl_input.h"
#include "lvgl_ui.h"
#include "ili9341.h"
#include "hal_timer.h"
#include "hal_time.h"
#include "hal_gpio.h"
#include "hal_spi.h"


/** Pode ser redefinido noutro ficheiro (ex.: vTaskDelay se usares FreeRTOS). */
__attribute__((weak)) void app_idle(void)
{
    /* WFI é uma instrução da CPU (não depende do HAL). */
    __asm volatile ("wfi");
}

void app_setup(void)
{
    hal_time_init();  /* Necessário para hal_time_ms() usado pelo Modbus (scheduler, timeouts) */
    hal_timer_init();
    hal_gpio_init();
    hal_spi_init();

    modbus_manager_init();
    mb_sniffer_init();
    mb_sniffer_enable(1);

    lv_init();
    if (!lvgl_port_tick_init())
        return;

    ili9341_port_init();
    if (!lvgl_display_init())
        return;

    if (!lvgl_input_init())
        return;

    lvgl_modbus_ui_create(NULL);
}

void app_loop(void)
{
    static uint32_t last_run = 0;
    uint32_t now = hal_time_ms();

    if (now - last_run >= 5u)
    {
        modbus_manager_poll();
        mb_sniffer_process();
        lv_timer_handler();
        last_run = now;
    }
    app_idle();
}
