#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "hal_uart.h"

static hal_uart_drv_imp_t *drv = &HAL_UART_DRV;

void hal_uart_init(void)
{
    if (drv->init)
        drv->init();
}

void hal_uart_deinit(void)
{
    if (drv->deinit)
        drv->deinit();
}

hal_uart_drv_t hal_uart_open(hal_uart_dev_interface_t interface, const hal_uart_cfg_t *cfg)
{
    return drv->open(interface, cfg);
}

void hal_uart_close(hal_uart_drv_t dev)
{
    drv->close(dev);
}

uart_status_t hal_uart_write(hal_uart_drv_t dev, const uint8_t *data, size_t len, size_t *written, uint32_t timeout_ms)
{
    return drv->write(dev, data, len, written, timeout_ms);
}

uart_status_t hal_uart_read(hal_uart_drv_t dev, uint8_t *data, size_t maxlen, size_t *read,uint32_t timeout_ms)
{
    return drv->read(dev, data, maxlen, read, timeout_ms);
}

void hal_uart_flush(hal_uart_drv_t dev)
{
    if (drv->flush)
        drv->flush(dev);
}

void hal_uart_set_event_cb(hal_uart_drv_t dev, hal_uart_event_cb_t cb, void *ctx)
{
    if (drv->set_event_cb)
        drv->set_event_cb(dev, cb, ctx);
}
