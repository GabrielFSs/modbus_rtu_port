#ifndef _HAL_SPI_H_
#define _HAL_SPI_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* ================= OPAQUE HANDLE ================= */

typedef struct hal_spi_drv_s *hal_spi_drv_t;

/* ================= SPI ID ================= */

typedef enum
{
    HAL_SPI_1 = 0,
    HAL_SPI_2,
    HAL_SPI_3,
    HAL_SPI_N
} hal_spi_id_t;

/* ================= MODE ================= */

typedef enum
{
    HAL_SPI_MODE_MASTER,
    HAL_SPI_MODE_SLAVE
} hal_spi_mode_t;

/* ================= CLOCK POLARITY ================= */

typedef enum
{
    HAL_SPI_CPOL_LOW,
    HAL_SPI_CPOL_HIGH
} hal_spi_cpol_t;

/* ================= CLOCK PHASE ================= */

typedef enum
{
    HAL_SPI_CPHA_1EDGE,
    HAL_SPI_CPHA_2EDGE
} hal_spi_cpha_t;

/* ================= BIT ORDER ================= */

typedef enum
{
    HAL_SPI_MSB_FIRST,
    HAL_SPI_LSB_FIRST
} hal_spi_bit_order_t;

/* ================= DATA SIZE ================= */

typedef enum
{
    HAL_SPI_DATASIZE_8BIT,
    HAL_SPI_DATASIZE_16BIT
} hal_spi_datasize_t;

/* ================= TRANSFER MODE ================= */

typedef enum
{
    HAL_SPI_XFER_POLLING,
    HAL_SPI_XFER_INTERRUPT,
    HAL_SPI_XFER_DMA
} hal_spi_xfer_mode_t;

/* ================= STATUS ================= */

typedef enum
{
    HAL_SPI_OK = 0,
    HAL_SPI_BUSY,
    HAL_SPI_TIMEOUT,
    HAL_SPI_ERROR
} hal_spi_status_t;

/* ================= EVENT ================= */

typedef enum
{
    HAL_SPI_EVENT_TX_DONE,
    HAL_SPI_EVENT_RX_DONE,
    HAL_SPI_EVENT_TXRX_DONE,
    HAL_SPI_EVENT_ERROR
} hal_spi_event_t;

/* ================= CALLBACK ================= */

typedef void (*hal_spi_event_cb_t)(hal_spi_drv_t spi,
                                   hal_spi_event_t event,
                                   hal_spi_status_t status,
                                   void *ctx);

/* ================= CONFIG ================= */

typedef struct
{
    hal_spi_mode_t       mode;
    hal_spi_cpol_t       cpol;
    hal_spi_cpha_t       cpha;
    hal_spi_bit_order_t  bit_order;
    hal_spi_datasize_t   datasize;
    hal_spi_xfer_mode_t  xfer_mode;

    uint32_t             baudrate_hz;

    hal_spi_event_cb_t   cb;
    void                *cb_ctx;
} hal_spi_cfg_t;

/* ================= DRIVER INTERFACE ================= */

typedef struct
{
    void (*init)(void);
    void (*deinit)(void);

    hal_spi_drv_t (*open)(hal_spi_id_t id,
                          const hal_spi_cfg_t *cfg);

    void (*close)(hal_spi_drv_t spi);

    hal_spi_status_t (*tx)(hal_spi_drv_t spi,
                           const uint8_t *tx,
                           size_t len,
                           uint32_t timeout_ms);

    hal_spi_status_t (*rx)(hal_spi_drv_t spi,
                           uint8_t *rx,
                           size_t len,
                           uint32_t timeout_ms);

    hal_spi_status_t (*txrx)(hal_spi_drv_t spi,
                             const uint8_t *tx,
                             uint8_t *rx,
                             size_t len,
                             uint32_t timeout_ms);

} hal_spi_drv_imp_t;

/* ================= DRIVER INSTANCE ================= */

extern hal_spi_drv_imp_t HAL_SPI_DRV;

/* ================= HAL API ================= */

void hal_spi_init(void);
void hal_spi_deinit(void);

hal_spi_drv_t hal_spi_open(hal_spi_id_t id,
                           const hal_spi_cfg_t *cfg);

void hal_spi_close(hal_spi_drv_t spi);

hal_spi_status_t hal_spi_transmit(hal_spi_drv_t spi,
                                  const uint8_t *tx,
                                  size_t len,
                                  uint32_t timeout_ms);

hal_spi_status_t hal_spi_receive(hal_spi_drv_t spi,
                                 uint8_t *rx,
                                 size_t len,
                                 uint32_t timeout_ms);

hal_spi_status_t hal_spi_transmit_receive(hal_spi_drv_t spi,
                                          const uint8_t *tx,
                                          uint8_t *rx,
                                          size_t len,
                                          uint32_t timeout_ms);

#endif
