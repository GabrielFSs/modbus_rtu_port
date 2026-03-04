#include "hal_display.h"
#include "bsp_display_fsmc.h"
#include "stm32f4xx_hal.h"
#include <stddef.h>

/* RS (comando/dado) do display: no MCU PD13 = A18, PD11 = A16.
 * CMD = 0x60000000 (RS=0), DATA = 0x60080000 (A18=1).
 * Se o cabo do módulo usar A16 para RS, use DATA = 0x60010000. */
 #define LCD_CMD_ADDR   (*((volatile uint16_t *)0x60000000))
 #define LCD_DATA_ADDR  (*((volatile uint16_t *)0x60080000))

/* 0 = sem swap (cores certas aqui; 1 = cinza/vibra). Alguns módulos pedem 1 (big-endian). */
#define FSMC_LCD_BYTE_SWAP  0

#if FSMC_LCD_BYTE_SWAP
#define LCD_DATA_16(v)  ((uint16_t)(((v) >> 8) | ((v) << 8)))
#else
#define LCD_DATA_16(v)  ((uint16_t)(v))
#endif

/* 1 = pausa a cada 80 px (pode reduzir shift). 0 = fluxo contínuo (sugestão comum p/ ILI9341). */
#define FSMC_LCD_SEGMENT_SYNC    0
#define FSMC_LCD_SEGMENT_SYNC_MS 0   /* 0 = usa _US ou NOPs, 1 = 1 ms (só teste) */
#define FSMC_LCD_SEGMENT_SYNC_US 30  /* 30 us → ~30 fps; 60 us → ~15 fps */

#if FSMC_LCD_SEGMENT_SYNC
#define SEGMENT_PIXELS  80u
/* ~1 us por 42 iterações a 168 MHz (4 ciclos/iter). */
static void delay_us(uint32_t us)
{
    uint32_t n = us * 42u;
    while (n--) { __asm volatile ("nop"); }
}
#endif

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
    LCD_DATA_ADDR = LCD_DATA_16(data);
    return HAL_DISPLAY_OK;
}

static hal_display_status_t stm32_fsmc_write_buffer(hal_display_t disp,
                                                    const uint16_t *data,
                                                    size_t len)
{
    (void)disp;

    for (size_t i = 0; i < len; i++)
    {
        LCD_DATA_ADDR = LCD_DATA_16(data[i]);
#if FSMC_LCD_SEGMENT_SYNC
        if (((i + 1u) % SEGMENT_PIXELS) == 0u)
        {
#if FSMC_LCD_SEGMENT_SYNC_MS > 0
            HAL_Delay(FSMC_LCD_SEGMENT_SYNC_MS);
#elif FSMC_LCD_SEGMENT_SYNC_US > 0
            delay_us(FSMC_LCD_SEGMENT_SYNC_US);
#else
            __asm volatile ("nop"); __asm volatile ("nop"); __asm volatile ("nop");
            __asm volatile ("nop"); __asm volatile ("nop");
#endif
        }
#endif
    }

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
