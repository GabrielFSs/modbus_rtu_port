#include "app.h"

#include "hal_gpio.h"
#include "hal_uart.h"
#include "hal_timer.h"
#include "hal_crc.h"
#include "hal_time.h"
#include "hal_time_stm32f4.h"

#include "mbm.h"
#include "mbm_port.h"

/* ========================================================= */
/* ===================== DEFINES =========================== */
/* ========================================================= */

#define SLAVE_ID     2
#define MB_REG_ADDR  0
#define MB_REG_QTY   1

/* ========================================================= */
/* ===================== HANDLES =========================== */
/* ========================================================= */

static hal_gpio_drv_t led_red  = NULL;
static hal_gpio_drv_t led_blue = NULL;
static hal_gpio_drv_t key1     = NULL;

/* ========================================================= */
/* ===================== MODBUS CALLBACK =================== */
/* ========================================================= */

static void modbus_cb(uint8_t *response,
                      uint16_t length,
                      mbm_status_t status)
{
    if (status != MBM_ERR_OK)
        return;

    if (length < 5)
        return;

    if (response[1] != 0x04)
        return;

    uint16_t value =
        ((uint16_t)response[3] << 8) |
         (uint16_t)response[4];

    if (value > 1000)
    {
        hal_gpio_write(led_red,  true);
        hal_gpio_write(led_blue, false);
    }
    else
    {
        hal_gpio_write(led_red,  false);
        hal_gpio_write(led_blue, true);
    }
}

/* ========================================================= */
/* ===================== BUTTON IRQ ======================== */
/* ========================================================= */

static void button_cb(hal_gpio_drv_t gpio, void *ctx)
{
    (void)ctx;

    if (gpio != key1)
        return;

    mbm_request_t req = {0};

    req.slave_id  = SLAVE_ID;
    req.function  = 0x04;
    req.address   = MB_REG_ADDR;
    req.quantity  = MB_REG_QTY;
    req.job_id    = 1;

    req.timeout_ms = 200;
    req.retries    = 2;

    req.callback = modbus_cb;

    mbm_add_request(&req);
}

/* ========================================================= */
/* ===================== SETUP ============================= */
/* ========================================================= */

void app_setup(void)
{
    /* ================= HAL INIT ================= */

    hal_gpio_init();
    hal_uart_init();
    hal_timer_init();
    hal_crc_init();

    hal_time_stm32f4_init();

    /* ================= LED CONFIG ================= */

    hal_gpio_cfg_t led_cfg =
    {
        .direction = HAL_GPIO_OUTPUT,
        .pull      = HAL_GPIO_NOPULL,
        .out_type  = HAL_GPIO_PUSH_PULL,
        .irq_edge  = HAL_GPIO_IRQ_NONE
    };

    led_red  = hal_gpio_open(HAL_GPIO_1, &led_cfg);
    led_blue = hal_gpio_open(HAL_GPIO_2, &led_cfg);

    hal_gpio_write(led_red,  false);
    hal_gpio_write(led_blue, false);

    /* ================= BUTTON CONFIG ================= */

    hal_gpio_cfg_t key_rising_cfg =
        {
            .direction = HAL_GPIO_INPUT,
            .pull      = HAL_GPIO_PULLDOWN,
            .irq_edge  = HAL_GPIO_IRQ_RISING
        };

    key1 = hal_gpio_open(HAL_GPIO_0, &key_rising_cfg);

    if (key1)
        hal_gpio_set_irq_cb(key1, button_cb, NULL);

    /* ================= MODBUS PORT OPEN ================= */

    mbm_serial_cfg_t serial_cfg =
    {
        .baudrate = 115200,
        .databits = HAL_UART_DATABITS_8,
        .parity   = HAL_UART_PARITY_NONE,
        .stopbits = HAL_UART_STOPBIT_1,
        .uart     = HAL_UART_DEV_3
    };

    mbm_port_open(&serial_cfg);

    /* Habilita o core */
    mbm_enable();
}

/* ========================================================= */
/* ===================== LOOP ============================== */
/* ========================================================= */

void app_loop(void)
{
    mbm_poll();
}
