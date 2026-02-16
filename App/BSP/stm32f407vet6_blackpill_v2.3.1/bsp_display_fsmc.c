#include "bsp_display_fsmc.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_sram.h"
#include "stm32f4xx_ll_fsmc.h"


/* Base address Bank1 NOR/SRAM */
#define LCD_BASE  ((uint32_t)0x60000000)

static SRAM_HandleTypeDef hsram;

void bsp_display_fsmc_init(void)
{
    /* ================= CLOCKS ================= */

    __HAL_RCC_FSMC_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /* ================= GPIO CONFIG ================= */

    GPIO_InitTypeDef gpio = {0};

    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_NOPULL;
    gpio.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF12_FSMC;

    /* D0-D3 (PD14, PD15, PD0, PD1) */
    gpio.Pin = GPIO_PIN_0 | GPIO_PIN_1 |
               GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &gpio);

    /* D4-D7 (PE7-PE10) */
    gpio.Pin = GPIO_PIN_7 | GPIO_PIN_8 |
               GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOE, &gpio);

    /* D8-D15 (PE11-PE15 + PD8-PD10) */
    gpio.Pin = GPIO_PIN_11 | GPIO_PIN_12 |
               GPIO_PIN_13 | GPIO_PIN_14 |
               GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &gpio);

    gpio.Pin = GPIO_PIN_8 | GPIO_PIN_9 |
               GPIO_PIN_10;
    HAL_GPIO_Init(GPIOD, &gpio);

    /* RD, WR, CS */
    gpio.Pin = GPIO_PIN_4 | GPIO_PIN_5; // NOE, NWE
    HAL_GPIO_Init(GPIOD, &gpio);

    gpio.Pin = GPIO_PIN_7; // NE1
    HAL_GPIO_Init(GPIOD, &gpio);

    /* RS via A16 (PG0 geralmente) */
    gpio.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOG, &gpio);

    /* A18 = PD13 (RS) */
    gpio.Pin       = GPIO_PIN_13;
    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_NOPULL;
    gpio.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio.Alternate = GPIO_AF12_FSMC;

    HAL_GPIO_Init(GPIOD, &gpio);

    /* ================= FSMC CONFIG ================= */

    hsram.Instance = FSMC_NORSRAM_DEVICE;
    hsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;

    hsram.Init.NSBank             = FSMC_NORSRAM_BANK1;
    hsram.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;
    hsram.Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram.Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;
    hsram.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
    hsram.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
    hsram.Init.ExtendedMode       = FSMC_EXTENDED_MODE_DISABLE;
    hsram.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram.Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;

    FSMC_NORSRAM_TimingTypeDef timing = {0};

    timing.AddressSetupTime      = 2;
    timing.AddressHoldTime       = 1;
    timing.DataSetupTime         = 5;
    timing.BusTurnAroundDuration = 1;
    timing.CLKDivision           = 2;
    timing.DataLatency           = 2;
    timing.AccessMode            = FSMC_ACCESS_MODE_A;

    HAL_SRAM_Init(&hsram, &timing, NULL);
}

void bsp_display_fsmc_deinit(void)
{
    HAL_SRAM_DeInit(&hsram);
}
