#include "hal_spi.h"
#include "bsp_spi.h"

#include "stm32f4xx_hal.h"
#include <string.h>

/* ===== DRIVER STRUCT ===== */
struct hal_spi_drv_s
{
    SPI_HandleTypeDef *hspi;

    hal_spi_xfer_mode_t xfer_mode;

    hal_spi_event_cb_t cb;
    void *cb_ctx;
};

/* ===== SINGLE INSTANCE ===== */
static struct hal_spi_drv_s spi1_drv;
static SPI_HandleTypeDef hspi1;

/* ===== INIT ===== */
static void stm32_spi_init(void)
{
    memset(&spi1_drv, 0, sizeof(spi1_drv));
}

/* ===== DEINIT ===== */
static void stm32_spi_deinit(void)
{
    if (spi1_drv.hspi)
    {
        HAL_SPI_DeInit(spi1_drv.hspi);
        spi1_drv.hspi = NULL;
    }
}

/* ===== OPEN ===== */
static hal_spi_drv_t stm32_spi_open(hal_spi_id_t id,
                                    const hal_spi_cfg_t *cfg)
{
    if (!cfg || id != HAL_SPI_1)
        return NULL;

    bsp_spi_init(BSP_SPI_1);

    hspi1.Instance = SPI1;
    hspi1.Init.Mode = (cfg->mode == HAL_SPI_MODE_MASTER) ?
                      SPI_MODE_MASTER : SPI_MODE_SLAVE;

    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize =
        (cfg->datasize == HAL_SPI_DATASIZE_16BIT) ?
        SPI_DATASIZE_16BIT : SPI_DATASIZE_8BIT;

    hspi1.Init.CLKPolarity =
        (cfg->cpol == HAL_SPI_CPOL_HIGH) ?
        SPI_POLARITY_HIGH : SPI_POLARITY_LOW;

    hspi1.Init.CLKPhase =
        (cfg->cpha == HAL_SPI_CPHA_2EDGE) ?
        SPI_PHASE_2EDGE : SPI_PHASE_1EDGE;

    hspi1.Init.NSS = SPI_NSS_SOFT;

    hspi1.Init.FirstBit =
        (cfg->bit_order == HAL_SPI_LSB_FIRST) ?
        SPI_FIRSTBIT_LSB : SPI_FIRSTBIT_MSB;

    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 7;

    /* Baudrate prescaler (simplificado) */
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;

    if (HAL_SPI_Init(&hspi1) != HAL_OK)
        return NULL;

    spi1_drv.hspi = &hspi1;
    spi1_drv.xfer_mode = cfg->xfer_mode;
    spi1_drv.cb = cfg->cb;
    spi1_drv.cb_ctx = cfg->cb_ctx;

    return (hal_spi_drv_t)&spi1_drv;
}

/* ===== CLOSE ===== */
static void stm32_spi_close(hal_spi_drv_t spi)
{
    struct hal_spi_drv_s *drv = spi;

    if (!drv || !drv->hspi)
        return;

    HAL_SPI_DeInit(drv->hspi);
    bsp_spi_deinit(BSP_SPI_1);

    drv->hspi = NULL;
}

/* ===== TX ===== */
static hal_spi_status_t stm32_spi_tx(hal_spi_drv_t spi,
                                     const uint8_t *tx,
                                     size_t len,
                                     uint32_t timeout_ms)
{
    struct hal_spi_drv_s *drv = spi;

    if (!drv || !drv->hspi)
        return HAL_SPI_ERROR;

    if (HAL_SPI_Transmit(drv->hspi,
                          (uint8_t *)tx,
                          len,
                          timeout_ms) != HAL_OK)
        return HAL_SPI_TIMEOUT;

    return HAL_SPI_OK;
}

/* ===== RX ===== */
static hal_spi_status_t stm32_spi_rx(hal_spi_drv_t spi,
                                     uint8_t *rx,
                                     size_t len,
                                     uint32_t timeout_ms)
{
    struct hal_spi_drv_s *drv = spi;

    if (!drv || !drv->hspi)
        return HAL_SPI_ERROR;

    if (HAL_SPI_Receive(drv->hspi,
                         rx,
                         len,
                         timeout_ms) != HAL_OK)
        return HAL_SPI_TIMEOUT;

    return HAL_SPI_OK;
}

/* ===== TXRX ===== */
static hal_spi_status_t stm32_spi_txrx(hal_spi_drv_t spi,
                                       const uint8_t *tx,
                                       uint8_t *rx,
                                       size_t len,
                                       uint32_t timeout_ms)
{
    struct hal_spi_drv_s *drv = spi;

    if (!drv || !drv->hspi)
        return HAL_SPI_ERROR;

    if (HAL_SPI_TransmitReceive(drv->hspi,
                                (uint8_t *)tx,
                                rx,
                                len,
                                timeout_ms) != HAL_OK)
        return HAL_SPI_TIMEOUT;

    return HAL_SPI_OK;
}

/* ===== DRIVER EXPORT ===== */
hal_spi_drv_imp_t HAL_SPI_DRV =
{
    .init   = stm32_spi_init,
    .deinit = stm32_spi_deinit,
    .open   = stm32_spi_open,
    .close  = stm32_spi_close,
    .tx     = stm32_spi_tx,
    .rx     = stm32_spi_rx,
    .txrx   = stm32_spi_txrx
};
