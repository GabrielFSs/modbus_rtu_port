#include "hal_spi.h"
#include "bsp_spi.h"
#include "stm32f4xx_hal.h"
#include <string.h>

/* ================= DRIVER STRUCT ================= */

struct hal_spi_drv_s
{
    SPI_HandleTypeDef *hspi;
    hal_spi_xfer_mode_t xfer_mode;

    hal_spi_event_cb_t cb;
    void *cb_ctx;
};

/* ================= INSTÂNCIAS ================= */

static struct hal_spi_drv_s spi1_drv;
static struct hal_spi_drv_s spi2_drv;
static SPI_HandleTypeDef hspi1;
static SPI_HandleTypeDef hspi2;

/* ================= AUXILIAR ================= */

static uint32_t spi_compute_prescaler(uint32_t pclk_hz, uint32_t target_hz)
{
    if (target_hz == 0)
        return SPI_BAUDRATEPRESCALER_256;

    uint32_t div = pclk_hz / target_hz;

    if (div <= 2)   return SPI_BAUDRATEPRESCALER_2;
    if (div <= 4)   return SPI_BAUDRATEPRESCALER_4;
    if (div <= 8)   return SPI_BAUDRATEPRESCALER_8;
    if (div <= 16)  return SPI_BAUDRATEPRESCALER_16;
    if (div <= 32)  return SPI_BAUDRATEPRESCALER_32;
    if (div <= 64)  return SPI_BAUDRATEPRESCALER_64;
    if (div <= 128) return SPI_BAUDRATEPRESCALER_128;

    return SPI_BAUDRATEPRESCALER_256;
}

/* ================= INIT ================= */

static void stm32_spi_init(void)
{
    memset(&spi1_drv, 0, sizeof(spi1_drv));
    memset(&spi2_drv, 0, sizeof(spi2_drv));
}

/* ================= DEINIT ================= */

static void stm32_spi_deinit(void)
{
    if (spi1_drv.hspi)
    {
        HAL_SPI_DeInit(spi1_drv.hspi);
        spi1_drv.hspi = NULL;
    }
    if (spi2_drv.hspi)
    {
        HAL_SPI_DeInit(spi2_drv.hspi);
        spi2_drv.hspi = NULL;
    }
}

/* ================= OPEN ================= */

static hal_spi_drv_t stm32_spi_open(hal_spi_id_t id,
                                    const hal_spi_cfg_t *cfg)
{
    if (!cfg || (id != HAL_SPI_1 && id != HAL_SPI_2))
        return NULL;

    bool use_irq = (cfg->xfer_mode == HAL_SPI_XFER_INTERRUPT);
    bool use_dma = (cfg->xfer_mode == HAL_SPI_XFER_DMA);

    if (id == HAL_SPI_1)
    {
        bsp_spi_init(BSP_SPI_1, use_irq, use_dma);

        hspi1.Instance = SPI1;
        hspi1.Init.Mode = (cfg->mode == HAL_SPI_MODE_MASTER) ? SPI_MODE_MASTER : SPI_MODE_SLAVE;
        hspi1.Init.Direction = SPI_DIRECTION_2LINES;
        hspi1.Init.DataSize = (cfg->datasize == HAL_SPI_DATASIZE_16BIT) ? SPI_DATASIZE_16BIT : SPI_DATASIZE_8BIT;
        hspi1.Init.CLKPolarity = (cfg->cpol == HAL_SPI_CPOL_HIGH) ? SPI_POLARITY_HIGH : SPI_POLARITY_LOW;
        hspi1.Init.CLKPhase = (cfg->cpha == HAL_SPI_CPHA_2EDGE) ? SPI_PHASE_2EDGE : SPI_PHASE_1EDGE;
        hspi1.Init.NSS = SPI_NSS_SOFT;
        hspi1.Init.FirstBit = (cfg->bit_order == HAL_SPI_LSB_FIRST) ? SPI_FIRSTBIT_LSB : SPI_FIRSTBIT_MSB;
        hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
        hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        hspi1.Init.CRCPolynomial = 7;
        hspi1.Init.BaudRatePrescaler = spi_compute_prescaler(HAL_RCC_GetPCLK2Freq(), cfg->baudrate_hz);

        if (HAL_SPI_Init(&hspi1) != HAL_OK)
            return NULL;

        spi1_drv.hspi     = &hspi1;
        spi1_drv.xfer_mode = cfg->xfer_mode;
        spi1_drv.cb       = cfg->cb;
        spi1_drv.cb_ctx   = cfg->cb_ctx;
        return &spi1_drv;
    }

    /* HAL_SPI_2: touch em muitos módulos (PB13/14/15) */
    bsp_spi_init(BSP_SPI_2, use_irq, use_dma);

    hspi2.Instance = SPI2;
    hspi2.Init.Mode = (cfg->mode == HAL_SPI_MODE_MASTER) ? SPI_MODE_MASTER : SPI_MODE_SLAVE;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = (cfg->datasize == HAL_SPI_DATASIZE_16BIT) ? SPI_DATASIZE_16BIT : SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity = (cfg->cpol == HAL_SPI_CPOL_HIGH) ? SPI_POLARITY_HIGH : SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = (cfg->cpha == HAL_SPI_CPHA_2EDGE) ? SPI_PHASE_2EDGE : SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;
    hspi2.Init.FirstBit = (cfg->bit_order == HAL_SPI_LSB_FIRST) ? SPI_FIRSTBIT_LSB : SPI_FIRSTBIT_MSB;
    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial = 7;
    hspi2.Init.BaudRatePrescaler = spi_compute_prescaler(HAL_RCC_GetPCLK1Freq(), cfg->baudrate_hz);

    if (HAL_SPI_Init(&hspi2) != HAL_OK)
        return NULL;

    spi2_drv.hspi     = &hspi2;
    spi2_drv.xfer_mode = cfg->xfer_mode;
    spi2_drv.cb       = cfg->cb;
    spi2_drv.cb_ctx   = cfg->cb_ctx;
    return &spi2_drv;
}

/* ================= CLOSE ================= */

static void stm32_spi_close(hal_spi_drv_t spi)
{
    struct hal_spi_drv_s *drv = spi;

    if (!drv || !drv->hspi)
        return;

    HAL_SPI_DeInit(drv->hspi);
    if (drv == &spi1_drv)
        bsp_spi_deinit(BSP_SPI_1);
    else if (drv == &spi2_drv)
        bsp_spi_deinit(BSP_SPI_2);
    drv->hspi = NULL;
}

/* ================= TX ================= */

static hal_spi_status_t stm32_spi_tx(hal_spi_drv_t spi,
                                     const uint8_t *tx,
                                     size_t len,
                                     uint32_t timeout_ms)
{
    struct hal_spi_drv_s *drv = spi;

    if (!drv || !drv->hspi || !tx || len == 0)
        return HAL_SPI_ERROR;

    HAL_StatusTypeDef ret;

    switch (drv->xfer_mode)
    {
        case HAL_SPI_XFER_POLLING:
            ret = HAL_SPI_Transmit(drv->hspi,
                                   (uint8_t *)tx,
                                   len,
                                   timeout_ms);
            break;

        case HAL_SPI_XFER_INTERRUPT:
            ret = HAL_SPI_Transmit_IT(drv->hspi,
                                      (uint8_t *)tx,
                                      len);
            break;

        case HAL_SPI_XFER_DMA:
            ret = HAL_SPI_Transmit_DMA(drv->hspi,
                                       (uint8_t *)tx,
                                       len);
            break;

        default:
            return HAL_SPI_ERROR;
    }

    return (ret == HAL_OK) ? HAL_SPI_OK : HAL_SPI_ERROR;
}

/* ================= RX ================= */

static hal_spi_status_t stm32_spi_rx(hal_spi_drv_t spi,
                                     uint8_t *rx,
                                     size_t len,
                                     uint32_t timeout_ms)
{
    struct hal_spi_drv_s *drv = spi;

    if (!drv || !drv->hspi || !rx || len == 0)
        return HAL_SPI_ERROR;

    HAL_StatusTypeDef ret;

    switch (drv->xfer_mode)
    {
        case HAL_SPI_XFER_POLLING:
            ret = HAL_SPI_Receive(drv->hspi,
                                  rx,
                                  len,
                                  timeout_ms);
            break;

        case HAL_SPI_XFER_INTERRUPT:
            ret = HAL_SPI_Receive_IT(drv->hspi,
                                     rx,
                                     len);
            break;

        case HAL_SPI_XFER_DMA:
            ret = HAL_SPI_Receive_DMA(drv->hspi,
                                      rx,
                                      len);
            break;

        default:
            return HAL_SPI_ERROR;
    }

    return (ret == HAL_OK) ? HAL_SPI_OK : HAL_SPI_ERROR;
}

/* ================= TXRX ================= */

static hal_spi_status_t stm32_spi_txrx(hal_spi_drv_t spi,
                                       const uint8_t *tx,
                                       uint8_t *rx,
                                       size_t len,
                                       uint32_t timeout_ms)
{
    struct hal_spi_drv_s *drv = spi;

    if (!drv || !drv->hspi || !tx || !rx || len == 0)
        return HAL_SPI_ERROR;

    HAL_StatusTypeDef ret;

    switch (drv->xfer_mode)
    {
        case HAL_SPI_XFER_POLLING:
            ret = HAL_SPI_TransmitReceive(drv->hspi,
                                          (uint8_t *)tx,
                                          rx,
                                          len,
                                          timeout_ms);
            break;

        case HAL_SPI_XFER_INTERRUPT:
            ret = HAL_SPI_TransmitReceive_IT(drv->hspi,
                                             (uint8_t *)tx,
                                             rx,
                                             len);
            break;

        case HAL_SPI_XFER_DMA:
            ret = HAL_SPI_TransmitReceive_DMA(drv->hspi,
                                              (uint8_t *)tx,
                                              rx,
                                              len);
            break;

        default:
            return HAL_SPI_ERROR;
    }

    return (ret == HAL_OK) ? HAL_SPI_OK : HAL_SPI_ERROR;
}

/* ================= DRIVER EXPORT ================= */

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
