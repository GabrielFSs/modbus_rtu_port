#include "hal_spi.h"

static hal_spi_drv_imp_t *drv = &HAL_SPI_DRV;

/* ================= INIT ================= */

void hal_spi_init(void)
{
    if (drv && drv->init)
        drv->init();
}

/* ================= DEINIT ================= */

void hal_spi_deinit(void)
{
    if (drv && drv->deinit)
        drv->deinit();
}

/* ================= OPEN ================= */

hal_spi_drv_t hal_spi_open(hal_spi_id_t id,
                           const hal_spi_cfg_t *cfg)
{
    if (!drv || !drv->open)
        return NULL;

    return drv->open(id, cfg);
}

/* ================= CLOSE ================= */

void hal_spi_close(hal_spi_drv_t spi)
{
    if (drv && drv->close)
        drv->close(spi);
}

/* ================= TX ================= */

hal_spi_status_t hal_spi_transmit(hal_spi_drv_t spi,
                                  const uint8_t *tx,
                                  size_t len,
                                  uint32_t timeout_ms)
{
    if (!drv || !drv->tx)
        return HAL_SPI_ERROR;

    return drv->tx(spi, tx, len, timeout_ms);
}

/* ================= RX ================= */

hal_spi_status_t hal_spi_receive(hal_spi_drv_t spi,
                                 uint8_t *rx,
                                 size_t len,
                                 uint32_t timeout_ms)
{
    if (!drv || !drv->rx)
        return HAL_SPI_ERROR;

    return drv->rx(spi, rx, len, timeout_ms);
}

/* ================= TXRX ================= */

hal_spi_status_t hal_spi_transmit_receive(hal_spi_drv_t spi,
                                          const uint8_t *tx,
                                          uint8_t *rx,
                                          size_t len,
                                          uint32_t timeout_ms)
{
    if (!drv || !drv->txrx)
        return HAL_SPI_ERROR;

    return drv->txrx(spi, tx, rx, len, timeout_ms);
}
