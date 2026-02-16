#include "hal_display.h"
#include "bsp_display_fsmc.h"
#include <stddef.h>

/* RS ligado em A16 */

#define LCD_CMD_ADDR  (*((volatile uint16_t *)0x60000000))
#define LCD_DATA_ADDR (*((volatile uint16_t *)0x60080000))  // A18

struct hal_display_drv_s
{
    int dummy;
};

static struct hal_display_drv_s display;

static void stm32_fsmc_init(void)
{
    bsp_display_fsmc_init();
}

static void stm32_fsmc_deinit(void)
{
    bsp_display_fsmc_deinit();
}

static hal_display_t stm32_fsmc_open(void)
{
    return &display;
}

static void stm32_fsmc_close(hal_display_t disp)
{
    (void)disp;
}

static hal_display_status_t stm32_fsmc_write_cmd(hal_display_t disp,
                                                 uint16_t cmd)
{
    (void)disp;
    LCD_CMD_ADDR = cmd;
    return HAL_DISPLAY_OK;
}

static hal_display_status_t stm32_fsmc_write_data(hal_display_t disp,
                                                  uint16_t data)
{
    (void)disp;
    LCD_DATA_ADDR = data;
    return HAL_DISPLAY_OK;
}

static hal_display_status_t stm32_fsmc_write_buffer(hal_display_t disp,
                                                    const uint16_t *data,
                                                    size_t len)
{
    (void)disp;

    for (size_t i = 0; i < len; i++)
        LCD_DATA_ADDR = data[i];

    return HAL_DISPLAY_OK;
}

hal_display_drv_imp_t HAL_DISPLAY_FSMC =
{
    .init        = stm32_fsmc_init,
    .deinit      = stm32_fsmc_deinit,
    .open        = stm32_fsmc_open,
    .close       = stm32_fsmc_close,
    .write_cmd   = stm32_fsmc_write_cmd,
    .write_data  = stm32_fsmc_write_data,
    .write_buffer= stm32_fsmc_write_buffer
};
