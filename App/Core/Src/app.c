#include "app.h"
#include "hal_gpio.h"
#include "hal_timer.h"
#include <stddef.h>

/* ===== DEFINES ===== */
#define LEVEL_MIN       0
#define LEVEL_MAX       9
#define LEVEL_DEFAULT   5

/* período base em ms */
#define PERIOD_MIN_MS   100   // mais rápido
#define PERIOD_MAX_MS   1000  // mais lento

/* ===== GPIO HANDLES ===== */
static hal_gpio_drv_t led_red;
static hal_gpio_drv_t led_blue;

static hal_gpio_drv_t key1;
static hal_gpio_drv_t key2;
static hal_gpio_drv_t key3;

/* ===== TIMER HANDLE ===== */
static hal_timer_t blink_timer;

/* ===== STATE ===== */
static int level = LEVEL_DEFAULT;
static bool led_state = false;

/* ===== FORWARD DECL ===== */
static void update_timer(void);

/* ===== TIMER CALLBACK ===== */
static void timer_cb(hal_timer_t timer, void *ctx)
{
    (void)timer;
    (void)ctx;

    led_state = !led_state;

    hal_gpio_write(led_red,  led_state);
    hal_gpio_write(led_blue, led_state);
}

/* ===== GPIO CALLBACK ===== */
static void key_cb(hal_gpio_drv_t gpio, void *ctx)
{
    (void)ctx;

    if (gpio == key1)
    {
        if (level < LEVEL_MAX)
            level++;
    }
    else if (gpio == key2)
    {
        if (level > LEVEL_MIN)
            level--;
    }
    else if (gpio == key3)
    {
        level = LEVEL_DEFAULT;
    }

    update_timer();
}

/* ===== TIMER (RE)CONFIG ===== */
static void update_timer(void)
{
    if (blink_timer)
    {
        hal_timer_stop(blink_timer);
        hal_timer_close(blink_timer);
        blink_timer = NULL;
    }

    /* interpolação linear */
    uint32_t period =
        PERIOD_MAX_MS -
        ((PERIOD_MAX_MS - PERIOD_MIN_MS) * level) / LEVEL_MAX;

    hal_timer_cfg_t cfg =
    {
        .period     = period,
        .periodic   = true,
        .resolution = HAL_TIMER_RESOLUTION_MS,
        .cb         = timer_cb,
        .cb_ctx     = NULL
    };

    blink_timer = hal_timer_open(HAL_TIMER_0, &cfg);
    hal_timer_start(blink_timer);
}

/* ===== APP SETUP ===== */
void app_setup(void)
{
    hal_gpio_init();
    hal_timer_init();

    /* ===== LEDs ===== */
    led_red = hal_gpio_open(HAL_GPIO_1, &(hal_gpio_cfg_t){
        .direction = HAL_GPIO_OUTPUT,
        .pull      = HAL_GPIO_NOPULL,
        .out_type  = HAL_GPIO_PUSH_PULL,
        .irq_edge  = HAL_GPIO_IRQ_NONE
    });

    led_blue = hal_gpio_open(HAL_GPIO_2, &(hal_gpio_cfg_t){
        .direction = HAL_GPIO_OUTPUT,
        .pull      = HAL_GPIO_NOPULL,
        .out_type  = HAL_GPIO_PUSH_PULL,
        .irq_edge  = HAL_GPIO_IRQ_NONE
    });

    hal_gpio_write(led_red, false);
    hal_gpio_write(led_blue, false);

    /* ===== KEYS ===== */
    key1 = hal_gpio_open(HAL_GPIO_0, &(hal_gpio_cfg_t){
        .direction = HAL_GPIO_INPUT,
        .pull      = HAL_GPIO_PULLDOWN,
        .irq_edge  = HAL_GPIO_IRQ_RISING
    });

    key2 = hal_gpio_open(HAL_GPIO_3, &(hal_gpio_cfg_t){
        .direction = HAL_GPIO_INPUT,
        .pull      = HAL_GPIO_PULLUP,
        .irq_edge  = HAL_GPIO_IRQ_FALLING
    });

    key3 = hal_gpio_open(HAL_GPIO_4, &(hal_gpio_cfg_t){
        .direction = HAL_GPIO_INPUT,
        .pull      = HAL_GPIO_PULLUP,
        .irq_edge  = HAL_GPIO_IRQ_FALLING
    });

    hal_gpio_set_irq_cb(key1, key_cb, NULL);
    hal_gpio_set_irq_cb(key2, key_cb, NULL);
    hal_gpio_set_irq_cb(key3, key_cb, NULL);

    /* ===== START TIMER ===== */
    update_timer();
}

/* ===== APP LOOP ===== */
void app_loop(void)
{
    /* vazio: tudo por IRQ */
}
