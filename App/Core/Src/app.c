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

#define VALUE_LOW    500
#define VALUE_HIGH   2000

#define DEBOUNCE_MS  200

/* ========================================================= */
/* ===================== HANDLES =========================== */
/* ========================================================= */

static hal_gpio_drv_t led_red  = NULL;
static hal_gpio_drv_t led_blue = NULL;

static hal_gpio_drv_t key1 = NULL;
static hal_gpio_drv_t key2 = NULL;
static hal_gpio_drv_t key3 = NULL;

/* Debounce */
static uint32_t last_press_key1 = 0;
static uint32_t last_press_key2 = 0;
static uint32_t last_press_key3 = 0;

/* ========================================================= */
/* ===================== HELPERS =========================== */
/* ========================================================= */

static void send_read_request(void)
{
    if (!mbm_is_enabled() || mbm_is_busy())
        return;

    mbm_request_t req = {0};

    req.slave_id  = SLAVE_ID;
    req.function  = 0x03;
    req.address   = MB_REG_ADDR;
    req.quantity  = 1;
    req.timeout_ms = 200;
    req.retries    = 2;

    req.callback = NULL;  // leitura simples, LED será atualizado no write callback

    mbm_add_request(&req);
}

/* ========================================================= */
/* ===================== MODBUS CALLBACK =================== */
/* ========================================================= */

static void modbus_cb(uint8_t *response,
                      uint16_t length,
                      mbm_status_t status)
{
    if (status != MBM_ERR_OK)
        return;

    if (!response || length < 5)
        return;

    /* ===== Leitura Holding Register ===== */
    if (response[1] == 0x03)
    {
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

    /* ===== Escrita concluída ===== */
    else if (response[1] == 0x06)
    {
        /* Após escrever, faz leitura controlada */
        send_read_request();
    }
}

/* ========================================================= */
/* ===================== BUTTON CALLBACK =================== */
/* ========================================================= */

static void button_cb(hal_gpio_drv_t gpio, void *ctx)
{
    (void)ctx;

    uint32_t now = hal_time_ms();

    if (!mbm_is_enabled())
        return;

    if (mbm_is_busy())
        return;

    mbm_request_t req = {0};

    req.slave_id   = SLAVE_ID;
    req.address    = MB_REG_ADDR;
    req.timeout_ms = 200;
    req.retries    = 2;
    req.callback   = modbus_cb;

    /* KEY1 → Leitura */
    if (gpio == key1)
    {
        if ((now - last_press_key1) < DEBOUNCE_MS)
            return;

        last_press_key1 = now;

        req.function = 0x03;
        req.quantity = 1;
    }

    /* KEY2 → Escreve valor baixo */
    else if (gpio == key2)
    {
        if ((now - last_press_key2) < DEBOUNCE_MS)
            return;

        last_press_key2 = now;

        req.function = 0x06;
        req.quantity = VALUE_LOW;
    }

    /* KEY3 → Escreve valor alto */
    else if (gpio == key3)
    {
        if ((now - last_press_key3) < DEBOUNCE_MS)
            return;

        last_press_key3 = now;

        req.function = 0x06;
        req.quantity = VALUE_HIGH;
    }
    else
        return;

    mbm_add_request(&req);
}

/* ========================================================= */
/* ===================== SETUP ============================= */
/* ========================================================= */

void app_setup(void)
{
    hal_gpio_init();
    hal_uart_init();
    hal_timer_init();
    hal_crc_init();
    hal_time_stm32f4_init();

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

    hal_gpio_cfg_t key_rising_cfg =
    {
        .direction = HAL_GPIO_INPUT,
        .pull      = HAL_GPIO_PULLDOWN,
        .irq_edge  = HAL_GPIO_IRQ_RISING
    };

    hal_gpio_cfg_t key_falling_cfg =
    {
        .direction = HAL_GPIO_INPUT,
        .pull      = HAL_GPIO_PULLUP,
        .irq_edge  = HAL_GPIO_IRQ_FALLING
    };

    key1 = hal_gpio_open(HAL_GPIO_0, &key_rising_cfg);
    key2 = hal_gpio_open(HAL_GPIO_3, &key_falling_cfg);
    key3 = hal_gpio_open(HAL_GPIO_4, &key_falling_cfg);

    if (key1) hal_gpio_set_irq_cb(key1, button_cb, NULL);
    if (key2) hal_gpio_set_irq_cb(key2, button_cb, NULL);
    if (key3) hal_gpio_set_irq_cb(key3, button_cb, NULL);

    mbm_serial_cfg_t serial_cfg =
    {
        .baudrate = 9600,
        .databits = HAL_UART_DATABITS_8,
        .parity   = HAL_UART_PARITY_NONE,
        .stopbits = HAL_UART_STOPBIT_1,
        .uart     = HAL_UART_DEV_3
    };

    mbm_port_open(&serial_cfg);
    mbm_enable();
}

/* ========================================================= */
/* ===================== LOOP ============================== */
/* ========================================================= */

void app_loop(void)
{
    mbm_poll();
}
