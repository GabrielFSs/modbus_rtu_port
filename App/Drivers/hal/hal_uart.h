#ifndef _HAL_UART_H_
#define _HAL_UART_H_

#include <stdint.h>
#include <stddef.h>

typedef struct hal_uart_drv_s * hal_uart_drv_t;

/* ===================== UART CONFIG ===================== */

typedef enum
{
    HAL_UART_DEV_1,
    HAL_UART_DEV_2,
    HAL_UART_DEV_3,
    HAL_UART_DEVS_N
} hal_uart_dev_interface_t;

typedef enum
{
    HAL_UART_BAUD_9600   = 9600,
    HAL_UART_BAUD_19200 = 19200,
    HAL_UART_BAUD_38400 = 38400,
    HAL_UART_BAUD_57600 = 57600,
    HAL_UART_BAUD_115200 = 115200,
    HAL_UART_BAUD_230400 = 230400
} hal_uart_baud_t;

typedef enum
{
    HAL_UART_STOPBIT_1 = 0,
    HAL_UART_STOPBIT_1_5,
    HAL_UART_STOPBIT_2
} hal_uart_dev_stopbit_t;

typedef enum
{
    HAL_UART_DATABITS_7,
    HAL_UART_DATABITS_8,
    HAL_UART_DATABITS_9
} hal_uart_databits_t;

typedef enum
{
    HAL_UART_PARITY_NONE = 0,
    HAL_UART_PARITY_EVEN,
    HAL_UART_PARITY_ODD
} hal_uart_dev_parity_t;

typedef enum
{
    UART_DIR_NONE,
    UART_DIR_GPIO
} uart_dir_ctrl_t;

typedef enum
{
    UART_MODE_POLLING,
    UART_MODE_INTERRUPT,
    UART_MODE_DMA
} uart_mode_t;

typedef enum
{
    UART_DUPLEX_FULL,
    UART_DUPLEX_HALF
} uart_duplex_t;

/* ===================== STATUS / EVENTS ===================== */

typedef enum
{
    UART_STATUS_OK,
    UART_STATUS_TIMEOUT,
    UART_STATUS_BUFFER_FULL,
    UART_STATUS_ERROR,
    UART_STATUS_ABORTED
} uart_status_t;

typedef enum
{
    UART_EVENT_RX_DONE,
    UART_EVENT_RX_TIMEOUT,
    UART_EVENT_TX_DONE,
    UART_EVENT_ERROR
} uart_event_t;

/* ===================== RX MODES ===================== */

typedef enum
{
    UART_RX_MODE_LINEAR,
    UART_RX_MODE_CIRCULAR
} uart_rx_mode_t;

typedef enum
{
    UART_RX_DONE_NONE,
    UART_RX_DONE_ON_CHAR,
    UART_RX_DONE_ON_TIMEOUT,
    UART_RX_DONE_ON_LENGTH,
} uart_rx_done_mode_t;

/* ===================== UART CONFIG STRUCT ===================== */

typedef struct
{
    hal_uart_baud_t  baudrate;
    hal_uart_dev_stopbit_t  stopbits;
    hal_uart_dev_parity_t  parity;

    uint8_t  *rx_buffer;
    uint16_t  rx_buffer_size;
    uart_rx_mode_t rx_mode;

    uint8_t  *tx_buffer;
    uint16_t  tx_buffer_size;

    uart_dir_ctrl_t comm_control;
    uart_mode_t comm_mode;
    uart_duplex_t duplex_mode;
    hal_uart_databits_t databits;

    uart_rx_done_mode_t rx_done_mode;
    uint8_t  rx_done_char;
    uint16_t rx_done_length;

} hal_uart_cfg_t;

/* ===================== CALLBACK ===================== */

typedef void (*hal_uart_event_cb_t)(
    hal_uart_drv_t dev,
    uart_event_t event,
    uart_status_t status,
    const uint8_t *data,
    size_t len,
    void *ctx
);

/* ===================== TIMER ABSTRACTION ===================== */

typedef void (*hal_uart_timer_start_fn_t)(void *ctx);
typedef void (*hal_uart_timer_stop_fn_t)(void *ctx);

/* ===================== API ===================== */

void hal_uart_init(void);
void hal_uart_deinit(void);

hal_uart_drv_t hal_uart_open(hal_uart_dev_interface_t interface,
                             const hal_uart_cfg_t *cfg);

void hal_uart_close(hal_uart_drv_t dev);

uart_status_t hal_uart_write(hal_uart_drv_t dev,
                             const uint8_t *data,
                             size_t len,
                             size_t *written,
                             uint32_t timeout_ms);

uart_status_t hal_uart_read(hal_uart_drv_t dev,
                            uint8_t *data,
                            size_t maxlen,
                            size_t *read,
                            uint32_t timeout_ms);

void hal_uart_flush(hal_uart_drv_t dev);

void hal_uart_set_event_cb(hal_uart_drv_t dev,
                           hal_uart_event_cb_t cb,
                           void *ctx);

/* Timer injection (optional, only for RX_DONE_ON_TIMEOUT) */
void hal_uart_set_rx_timeout_timer(hal_uart_drv_t dev,
                                   hal_uart_timer_start_fn_t start,
                                   hal_uart_timer_stop_fn_t stop,
                                   void *ctx);

/* ===================== DRIVER IMP ===================== */

typedef struct hal_uart_drv_imp_s
{
    void (*init)(void);
    void (*deinit)(void);

    hal_uart_drv_t (*open)(hal_uart_dev_interface_t interface,
                           const hal_uart_cfg_t *cfg);
    void (*close)(hal_uart_drv_t dev);

    uart_status_t (*write)(hal_uart_drv_t dev,
                           const uint8_t *data,
                           size_t len,
                           size_t *written,
                           uint32_t timeout_ms);

    uart_status_t (*read)(hal_uart_drv_t dev,
                          uint8_t *data,
                          size_t maxlen,
                          size_t *read,
                          uint32_t timeout_ms);

    void (*flush)(hal_uart_drv_t dev);

    void (*set_event_cb)(hal_uart_drv_t dev,
                         hal_uart_event_cb_t cb,
                         void *ctx);

    void (*set_rx_timeout_timer)(hal_uart_drv_t dev,
                                     hal_uart_timer_start_fn_t start,
                                     hal_uart_timer_stop_fn_t stop,
                                     void *ctx);

} hal_uart_drv_imp_t;

extern hal_uart_drv_imp_t HAL_UART_DRV;

void hal_uart_set_rx_timeout_timer(hal_uart_drv_t dev,
                                   hal_uart_timer_start_fn_t start,
                                   hal_uart_timer_stop_fn_t stop,
                                   void *ctx);


#endif /* _HAL_UART_H_ */
