#include "app.h"
#include "hal_uart.h"
#include "hal_gpio.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"

/* ===== CONFIG ===== */
#define RX_BUF_SIZE   64
#define TX_PERIOD_MS  10000

/* ===== GLOBALS ===== */
static hal_uart_drv_t uart;
static uint8_t rx_buf[RX_BUF_SIZE];
static uint32_t last_tick;

static hal_gpio_drv_t rs485_de;
static hal_gpio_drv_t rs485_re;

/* ========================================================= */
/* ================= RS485 DIR CALLBACK ==================== */
/* ========================================================= */

void rs485_dir_control(void *ctx, hal_uart_dir_t dir)
{
    if (dir == HAL_UART_DIR_TX)
    {
        hal_gpio_write(rs485_de, true);
        hal_gpio_write(rs485_re, true);
    }
    else
    {
        hal_gpio_write(rs485_de, false);
        hal_gpio_write(rs485_re, false);
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

    .comm_control = UART_DIR_EXTERNAL,
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
    /* ===== GPIO INIT ===== */
    hal_gpio_init();

    hal_gpio_cfg_t gpio_out =
    {
        .direction = HAL_GPIO_OUTPUT,
        .pull      = HAL_GPIO_NOPULL,
        .out_type  = HAL_GPIO_PUSH_PULL,
        .irq_edge  = HAL_GPIO_IRQ_NONE
    };

    rs485_de = hal_gpio_open(HAL_GPIO_RS485_DE, &gpio_out);
    rs485_re = hal_gpio_open(HAL_GPIO_RS485_RE, &gpio_out);

    if (!rs485_de || !rs485_re)
    {
        while (1);
    }

    /* inicia RS485 em RX */
    hal_gpio_write(rs485_de, false);
    hal_gpio_write(rs485_re, false);

    /* ===== UART INIT ===== */
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
