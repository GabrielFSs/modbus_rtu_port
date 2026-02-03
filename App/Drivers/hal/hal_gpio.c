#include "hal_gpio.h"

/* Ponteiro para a implementação concreta (PORT) */
static hal_gpio_drv_imp_t *drv = &HAL_GPIO_DRV;

/* ===== INIT / DEINIT ===== */

void hal_gpio_init(void)
{
    if (drv->init)
        drv->init();
}

void hal_gpio_deinit(void)
{
    if (drv->deinit)
        drv->deinit();
}

/* ===== OPEN / CLOSE ===== */

hal_gpio_drv_t hal_gpio_open(hal_gpio_id_t id, const hal_gpio_cfg_t *cfg)
{
    if (!drv->open)
        return NULL;

    return drv->open(id, cfg);
}

void hal_gpio_close(hal_gpio_drv_t gpio)
{
    if (!drv->close)
        return;

    drv->close(gpio);
}

/* ===== READ / WRITE ===== */

hal_gpio_status_t hal_gpio_write(hal_gpio_drv_t gpio, bool value)
{
    if (!drv->write)
        return HAL_GPIO_ERROR;

    return drv->write(gpio, value);
}

hal_gpio_status_t hal_gpio_read(hal_gpio_drv_t gpio, bool *value)
{
    if (!drv->read || !value)
        return HAL_GPIO_ERROR;

    return drv->read(gpio, value);
}

/* ===== IRQ CALLBACK ===== */

void hal_gpio_set_irq_cb(hal_gpio_drv_t gpio,
                         hal_gpio_irq_cb_t cb,
                         void *ctx)
{
    if (!drv->set_irq_cb)
        return;

    drv->set_irq_cb(gpio, cb, ctx);
}
