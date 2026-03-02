#include "app.h"
#include "hal_uart.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"

/* ===== CONFIG ===== */
#define RX_BUF_SIZE 64
#define TX_PERIOD_MS 10000

#define RS485_DE_PORT GPIOB
#define RS485_DE_PIN  GPIO_PIN_12

#define RS485_RE_PORT GPIOB
#define RS485_RE_PIN  GPIO_PIN_13

static hal_uart_drv_t uart;
static uint8_t rx_buf[RX_BUF_SIZE];
static uint32_t last_tick;

/* ========================================================= */
/* ================= RS485 GPIO INIT ======================= */
/* ========================================================= */
static void rs485_gpio_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};

    gpio.Pin   = RS485_DE_PIN | RS485_RE_PIN;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOB, &gpio);

    /* Inicia em modo RX */
    HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RS485_RE_PORT, RS485_RE_PIN, GPIO_PIN_RESET);
}

/* ========================================================= */
/* ================= RS485 DIR CALLBACK ==================== */
/* ========================================================= */
static void rs485_dir_control(void *ctx, bool enable)
{
    (void)ctx;

    if (enable)
    {
        /* TX mode */
        HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(RS485_RE_PORT, RS485_RE_PIN, GPIO_PIN_SET);
    }
    else
    {
        /* RX mode */
        HAL_GPIO_WritePin(RS485_DE_PORT, RS485_DE_PIN, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(RS485_RE_PORT, RS485_RE_PIN, GPIO_PIN_RESET);
    }
}

/* ========================================================= */
/* ================= UART CONFIG =========================== */
/* ========================================================= */
static hal_uart_cfg_t uart_cfg =
{
    .baudrate = HAL_UART_BAUD_9600,
    .databits = HAL_UART_DATABITS_8,
    .stopbits = HAL_UART_STOPBIT_1,
    .parity   = HAL_UART_PARITY_NONE,

    .comm_mode   = UART_MODE_INTERRUPT,
    .duplex_mode = UART_DUPLEX_HALF,

    .comm_control = UART_DIR_GPIO,
    .dir_ctrl     = rs485_dir_control,
    .dir_ctrl_ctx = NULL,

    .rx_mode        = UART_RX_MODE_LINEAR,
    .rx_buffer      = rx_buf,
    .rx_buffer_size = sizeof(rx_buf),

    .tx_buffer      = NULL,
    .tx_buffer_size = 0,

    .rx_done_mode = UART_RX_DONE_NONE
};

/* ========================================================= */
/* ================= SETUP ================================= */
/* ========================================================= */
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

/* ========================================================= */
/* ================= LOOP ================================== */
/* ========================================================= */
void app_loop(void)
{
    uint32_t now = HAL_GetTick();

    if ((now - last_tick) >= TX_PERIOD_MS)
    {
        last_tick = now;

        size_t len = 0;
        uint8_t tmp[RX_BUF_SIZE];

        if (hal_uart_read(uart,
                          tmp,
                          sizeof(tmp),
                          &len,
                          0) == UART_STATUS_OK && len > 0)
        {
            size_t written;

            hal_uart_write(uart,
                           tmp,
                           len,
                           &written,
                           100);

            const uint8_t nl[] = "\r\n";
            hal_uart_write(uart, nl, 2, NULL, 100);
        }
    }
}
