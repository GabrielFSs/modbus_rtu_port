#include "main.h"
#include "hal_uart.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"

/* ===== RS485 PINS ===== */
#define RS485_DE_PORT GPIOB
#define RS485_DE_PIN  GPIO_PIN_12

#define RS485_RE_PORT GPIOB
#define RS485_RE_PIN  GPIO_PIN_13

/* ===== UART ===== */
static hal_uart_drv_t uart;

/* ===== RS485 GPIO INIT ===== */
static void rs485_gpio_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};

    gpio.Pin   = RS485_DE_PIN | RS485_RE_PIN;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOB, &gpio);

    /* Começa em RX */
    HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RS485_RE_PORT, RS485_RE_PIN, GPIO_PIN_RESET);
}

/* ===== RS485 DIR CALLBACK ===== */
static void rs485_dir_control(void *ctx, bool enable)
{
    (void)ctx;

    if (enable)
    {
        /* TX */
        HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(RS485_RE_PORT, RS485_RE_PIN, GPIO_PIN_SET);
    }
    else
    {
        /* RX */
        HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RS485_RE_PORT, RS485_RE_PIN, GPIO_PIN_RESET);
    }
}

/* ===== UART CONFIG ===== */
static hal_uart_cfg_t uart_cfg =
{
    .baudrate = HAL_UART_BAUD_9600,
    .databits = HAL_UART_DATABITS_8,
    .stopbits = HAL_UART_STOPBIT_1,
    .parity   = HAL_UART_PARITY_NONE,

    .comm_mode   = UART_MODE_POLLING,
    .duplex_mode = UART_DUPLEX_HALF,

    .comm_control = UART_DIR_GPIO,
    .dir_ctrl     = rs485_dir_control,
    .dir_ctrl_ctx = NULL,

    .rx_mode        = UART_RX_MODE_LINEAR,
    .rx_buffer      = NULL,
    .rx_buffer_size = 0,

    .tx_buffer      = NULL,
    .tx_buffer_size = 0,

    .rx_done_mode = UART_RX_DONE_NONE
};

/* ===== VARIÁVEIS ===== */
static uint32_t last_tick = 0;
static const uint8_t msg[] = "Teste RS485\r\n";

void app_setup(void)
{
    rs485_gpio_init();

    hal_uart_init();

    uart = hal_uart_open(HAL_UART_DEV_3, &uart_cfg);

    if (!uart)
    {
        while (1);
    }

    last_tick = HAL_GetTick();
}

void app_loop(void)
{
    if ((HAL_GetTick() - last_tick) >= 1000)
    {
        last_tick = HAL_GetTick();

        size_t written;

        hal_uart_write(uart,
                       msg,
                       sizeof(msg) - 1,
                       &written,
                       100);
    }
}
