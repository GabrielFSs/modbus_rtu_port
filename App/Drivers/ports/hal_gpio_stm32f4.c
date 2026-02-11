#include "hal_gpio.h"
#include "bsp_gpio.h"
#include "stm32f4xx_hal.h"

#include <string.h>
#include <stdbool.h>

/* ===== DRIVER INSTANCE ===== */
extern hal_gpio_drv_imp_t HAL_GPIO_DRV;

/* ===== GPIO DRIVER STRUCT ===== */
struct hal_gpio_drv_s
{
    hal_gpio_id_t id;

    bool active_high;

    GPIO_TypeDef *port;
    uint16_t pin;

    hal_gpio_cfg_t cfg;

    hal_gpio_irq_cb_t cb;
    void *cb_ctx;
};

/* ===== INSTANCES ===== */
static struct hal_gpio_drv_s gpio_instances[HAL_GPIO_N];

/* ===== INIT ===== */
static void stm32_gpio_init(void)
{
    memset(gpio_instances, 0, sizeof(gpio_instances));
}

static void stm32_gpio_deinit(void)
{
    /* Nada global */
}

/* ===== OPEN ===== */
static hal_gpio_drv_t stm32_gpio_open(hal_gpio_id_t id,
                                      const hal_gpio_cfg_t *cfg)
{
    if (!cfg || id >= HAL_GPIO_N)
        return NULL;

    struct hal_gpio_drv_s *drv = &gpio_instances[id];
    memset(drv, 0, sizeof(*drv));

    drv->id  = id;
    drv->cfg = *cfg;

    drv->active_high = bsp_gpio_get_active_level(id);

    if (!bsp_gpio_get_pin(id, &drv->port, &drv->pin))
        return NULL;

    GPIO_InitTypeDef gpio = {0};

    /* MODE */
    if (cfg->direction == HAL_GPIO_OUTPUT)
    {
        gpio.Mode = (cfg->out_type == HAL_GPIO_OPEN_DRAIN)
                  ? GPIO_MODE_OUTPUT_OD
                  : GPIO_MODE_OUTPUT_PP;
    }
    else
    {
        switch (cfg->irq_edge)
        {
            case HAL_GPIO_IRQ_RISING:
                gpio.Mode = GPIO_MODE_IT_RISING;
                break;
            case HAL_GPIO_IRQ_FALLING:
                gpio.Mode = GPIO_MODE_IT_FALLING;
                break;
            case HAL_GPIO_IRQ_BOTH:
                gpio.Mode = GPIO_MODE_IT_RISING_FALLING;
                break;
            default:
                gpio.Mode = GPIO_MODE_INPUT;
                break;
        }
    }

    /* PULL */
    switch (cfg->pull)
    {
        case HAL_GPIO_PULLUP:
            gpio.Pull = GPIO_PULLUP;
            break;
        case HAL_GPIO_PULLDOWN:
            gpio.Pull = GPIO_PULLDOWN;
            break;
        default:
            gpio.Pull = GPIO_NOPULL;
            break;
    }

    gpio.Pin   = drv->pin;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(drv->port, &gpio);

    if (cfg->irq_edge != HAL_GPIO_IRQ_NONE)
        bsp_gpio_enable_irq(drv->pin);

    return (hal_gpio_drv_t)drv;
}

/* ===== CLOSE ===== */
static void stm32_gpio_close(hal_gpio_drv_t gpio)
{
    if (!gpio)
        return;

    struct hal_gpio_drv_s *drv = (struct hal_gpio_drv_s *)gpio;

    HAL_GPIO_DeInit(drv->port, drv->pin);

    drv->cb = NULL;
    drv->cb_ctx = NULL;
}

/* ===== WRITE ===== */
static hal_gpio_status_t stm32_gpio_write(hal_gpio_drv_t gpio, bool value)
{
    if (!gpio)
        return HAL_GPIO_ERROR;

    struct hal_gpio_drv_s *drv = (struct hal_gpio_drv_s *)gpio;

    bool electrical = drv->active_high ? value : !value;

    HAL_GPIO_WritePin(
        drv->port,
        drv->pin,
        electrical ? GPIO_PIN_SET : GPIO_PIN_RESET
    );

    return HAL_GPIO_OK;
}

/* ===== READ ===== */
static hal_gpio_status_t stm32_gpio_read(hal_gpio_drv_t gpio, bool *value)
{
    if (!gpio || !value)
        return HAL_GPIO_ERROR;

    struct hal_gpio_drv_s *drv = (struct hal_gpio_drv_s *)gpio;

    bool electrical =
        (HAL_GPIO_ReadPin(drv->port, drv->pin) == GPIO_PIN_SET);

    /* converte para valor lógico */
    *value = drv->active_high ? electrical : !electrical;

    return HAL_GPIO_OK;
}

/* ===== IRQ CALLBACK ===== */
static void stm32_gpio_set_irq_cb(hal_gpio_drv_t gpio,
                                  hal_gpio_irq_cb_t cb,
                                  void *ctx)
{
    if (!gpio)
        return;

    struct hal_gpio_drv_s *drv = (struct hal_gpio_drv_s *)gpio;

    drv->cb = cb;
    drv->cb_ctx = ctx;
}

/* ===== EXTI DISPATCH ===== */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    for (int i = 0; i < HAL_GPIO_N; i++)
    {
        struct hal_gpio_drv_s *drv = &gpio_instances[i];

        if (drv->pin == GPIO_Pin && drv->cb)
            drv->cb((hal_gpio_drv_t)drv, drv->cb_ctx);
    }
}

/* ===== DRIVER VTABLE ===== */
hal_gpio_drv_imp_t HAL_GPIO_DRV =
{
    .init       = stm32_gpio_init,
    .deinit     = stm32_gpio_deinit,
    .open       = stm32_gpio_open,
    .close      = stm32_gpio_close,
    .write      = stm32_gpio_write,
    .read       = stm32_gpio_read,
    .set_irq_cb = stm32_gpio_set_irq_cb
};
