#include "app.h"
#include "hal_gpio.h"

#include <stdbool.h>
#include <stddef.h>

/* ===== HANDLES ===== */

/* LEDs */
static hal_gpio_drv_t led_red  = NULL;
static hal_gpio_drv_t led_blue = NULL;

/* Botões */
static hal_gpio_drv_t key1 = NULL;
static hal_gpio_drv_t key2 = NULL;
static hal_gpio_drv_t key3 = NULL;

/* ===== ESTADOS ===== */

static bool red_state  = false;
static bool blue_state = false;

/* ========================================================= */
/* ===================== CALLBACK ========================== */
/* ========================================================= */

static void button_cb(hal_gpio_drv_t gpio, void *ctx)
{
    (void)ctx;

    if (gpio == key1)
    {
        red_state = !red_state;
        hal_gpio_write(led_red, red_state);
    }
    else if (gpio == key2)
    {
        blue_state = !blue_state;
        hal_gpio_write(led_blue, blue_state);
    }
    else if (gpio == key3)
    {
        red_state  = !red_state;
        blue_state = !blue_state;

        hal_gpio_write(led_red,  red_state);
        hal_gpio_write(led_blue, blue_state);
    }
}

/* ========================================================= */
/* ===================== SETUP ============================= */
/* ========================================================= */

void app_setup(void)
{
    hal_gpio_init();

    /* ===== LEDs ===== */

    hal_gpio_cfg_t led_cfg =
    {
        .direction = HAL_GPIO_OUTPUT,
        .pull      = HAL_GPIO_NOPULL,
        .out_type  = HAL_GPIO_PUSH_PULL,
        .irq_edge  = HAL_GPIO_IRQ_NONE
    };

    led_red  = hal_gpio_open(HAL_GPIO_1, &led_cfg);
    led_blue = hal_gpio_open(HAL_GPIO_2, &led_cfg);

    /* Inicialmente desligados */
    if (led_red)  hal_gpio_write(led_red,  false);
    if (led_blue) hal_gpio_write(led_blue, false);

    /* ===== BOTÕES ===== */

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

    /* ===== CALLBACKS ===== */

    if (key1) hal_gpio_set_irq_cb(key1, button_cb, NULL);
    if (key2) hal_gpio_set_irq_cb(key2, button_cb, NULL);
    if (key3) hal_gpio_set_irq_cb(key3, button_cb, NULL);
}

/* ========================================================= */
/* ===================== LOOP ============================== */
/* ========================================================= */

void app_loop(void)
{
    /* Tudo por interrupção */
}
