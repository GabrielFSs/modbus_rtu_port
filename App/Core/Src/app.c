#include "app.h"
#include "hal_gpio.h"
#include <stdbool.h>
#include <stddef.h>

/* LEDs */
static hal_gpio_drv_t led_red;
static hal_gpio_drv_t led_blue;

/* Botões */
static hal_gpio_drv_t key1;
static hal_gpio_drv_t key2;
static hal_gpio_drv_t key3;

/* Estados dos LEDs */
static bool red_state = false;
static bool blue_state = false;

/* ===== CALLBACK ===== */
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

        hal_gpio_write(led_red, red_state);
        hal_gpio_write(led_blue, blue_state);
    }
}

void app_setup(void)
{
    hal_gpio_init();

    /* ===== LEDs ===== */
    led_red = hal_gpio_open(HAL_GPIO_1, &(hal_gpio_cfg_t){
        .direction = HAL_GPIO_OUTPUT,
        .pull = HAL_GPIO_NOPULL,
        .out_type = HAL_GPIO_PUSH_PULL,
        .irq_edge = HAL_GPIO_IRQ_NONE
    });

    led_blue = hal_gpio_open(HAL_GPIO_2, &(hal_gpio_cfg_t){
        .direction = HAL_GPIO_OUTPUT,
        .pull = HAL_GPIO_NOPULL,
        .out_type = HAL_GPIO_PUSH_PULL,
        .irq_edge = HAL_GPIO_IRQ_NONE
    });

    /* Inicialmente desligados */
    hal_gpio_write(led_red, red_state);
    hal_gpio_write(led_blue, blue_state);

    /* ===== BOTÕES ===== */
    key1 = hal_gpio_open(HAL_GPIO_0, &(hal_gpio_cfg_t){
        .direction = HAL_GPIO_INPUT,
        .pull = HAL_GPIO_PULLDOWN,
        .irq_edge = HAL_GPIO_IRQ_RISING
    });

    key2 = hal_gpio_open(HAL_GPIO_3, &(hal_gpio_cfg_t){
        .direction = HAL_GPIO_INPUT,
        .pull = HAL_GPIO_PULLUP,
        .irq_edge = HAL_GPIO_IRQ_FALLING
    });

    key3 = hal_gpio_open(HAL_GPIO_4, &(hal_gpio_cfg_t){
        .direction = HAL_GPIO_INPUT,
        .pull = HAL_GPIO_PULLUP,
        .irq_edge = HAL_GPIO_IRQ_FALLING
    });

    /* Callbacks */
    hal_gpio_set_irq_cb(key1, button_cb, NULL);
    hal_gpio_set_irq_cb(key2, button_cb, NULL);
    hal_gpio_set_irq_cb(key3, button_cb, NULL);
}

void app_loop(void)
{
    /* Tudo por interrupção */
}
