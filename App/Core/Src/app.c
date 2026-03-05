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
#include "hal_gpio.h"
#include "hal_spi.h"
#include "stm32f4xx_hal.h"

/* ========================================================================
 * Fluxo GUI ↔ Modbus
 * ========================================================================
 * - GUI é criada em app_setup() com lvgl_modbus_ui_create(NULL).
 * - O que o utilizador escreve/clica é tratado nos callbacks em lvgl_ui.c:
 *   • "Aplicar" (Barramento) → modbus_manager_start(pending_mode, &cfg).
 *   • "Enviar" (Master)      → mbm_add_request(&req); o resultado aparece no
 *     label via mbm_callback (OK/Timeout/Busy).
 *   • "Escrever" (Slave)     → escrita em mb_holding_regs[], mb_input_regs[],
 *     mb_coils[] (o master externo lê quando faz pedidos).
 * - O envio/receção real na UART é feito no app_loop() por
 *   modbus_manager_poll(): conforme o modo (Slave ou Master) chama
 *   eMBPoll() ou mbm_poll(). Só um deles é chamado em cada ciclo.
 * ======================================================================== */

/** Pode ser redefinido noutro ficheiro (ex.: vTaskDelay se usares FreeRTOS). */
__attribute__((weak)) void app_idle(void)
{
    __WFI();
}

void app_setup(void)
{
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
    uint32_t now = HAL_GetTick();

    if (now - last_run >= 5u)
    {
        modbus_manager_poll();
        mb_sniffer_process();
        lv_timer_handler();
        last_run = now;
    }
    app_idle();
}
