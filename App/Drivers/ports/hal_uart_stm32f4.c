#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#include "hal_uart.h"
#include "bsp_uart.h"

/* ========================================================= */
/* ================= DRIVER STRUCT ========================= */
/* ========================================================= */

struct hal_uart_drv_s
{
    hal_uart_dev_interface_t iface;
    UART_HandleTypeDef *huart;

    uart_mode_t   mode;
    uart_duplex_t duplex;

    /* RX */
    bool     rx_enabled;
    uint8_t  rx_byte;

    uint8_t *rx_buf;
    size_t   rx_buf_size;
    size_t   rx_len;
    uart_rx_mode_t rx_mode;

    /* RX DONE control */
    uart_rx_done_mode_t rx_done_mode;
    uint8_t  rx_done_char;
    uint16_t rx_done_length;
    bool     rx_done;

    /* RX timeout (external timer) */
    hal_uart_timer_start_fn_t timer_start;
    hal_uart_timer_stop_fn_t  timer_stop;
    void *timer_ctx;

    /* RX circular */
    size_t rx_wr;
    size_t rx_rd;

    /* TX */
    bool tx_busy;

    /* Callback */
    hal_uart_event_cb_t cb;
    void *cb_ctx;

    /* RS485 */
    uart_dir_ctrl_t dir_mode;
    GPIO_TypeDef *de_port;
    uint16_t de_pin;
};

static struct hal_uart_drv_s uart_instances[HAL_UART_DEVS_N];

/* ========================================================= */
/* ================= INIT / DEINIT ========================= */
/* ========================================================= */

static void stm32_uart_init(void)
{
    memset(uart_instances, 0, sizeof(uart_instances));
    __HAL_RCC_SYSCFG_CLK_ENABLE();
}

static void stm32_uart_deinit(void)
{
    for (int i = 0; i < HAL_UART_DEVS_N; i++)
    {
        if (uart_instances[i].huart)
        {
            HAL_UART_DeInit(uart_instances[i].huart);
            uart_instances[i].huart = NULL;
        }
    }
}

/* ========================================================= */
/* ================= OPEN / CLOSE ========================== */
/* ========================================================= */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

static hal_uart_drv_t stm32_uart_open(hal_uart_dev_interface_t interface,
                                      const hal_uart_cfg_t *cfg)
{
    struct hal_uart_drv_s *drv;
    UART_HandleTypeDef *huart;

    if (!cfg || interface >= HAL_UART_DEVS_N)
        return NULL;

    drv = &uart_instances[interface];
    memset(drv, 0, sizeof(*drv));

    drv->iface   = interface;
    drv->mode    = cfg->comm_mode;
    drv->duplex  = cfg->duplex_mode;
    drv->dir_mode = cfg->comm_control;

    drv->rx_mode      = cfg->rx_mode;
    drv->rx_buf       = cfg->rx_buffer;
    drv->rx_buf_size  = cfg->rx_buffer_size;

    drv->rx_done_mode   = cfg->rx_done_mode;
    drv->rx_done_char   = cfg->rx_done_char;
    drv->rx_done_length = cfg->rx_done_length;
    drv->rx_done        = false;

    drv->rx_len = 0;
    drv->rx_wr  = 0;
    drv->rx_rd  = 0;

    switch (interface)
    {
        case HAL_UART_DEV_1:
            huart = &huart1;
            huart->Instance = USART1;
            bsp_uart_init(BSP_UART_1);
            break;

        case HAL_UART_DEV_2:
            huart = &huart2;
            huart->Instance = USART2;
            bsp_uart_init(BSP_UART_2);
            break;

        case HAL_UART_DEV_3:
            huart = &huart3;
            huart->Instance = USART3;
            bsp_uart_init(BSP_UART_3);
            break;

        default:
            return NULL;
    }

    huart->Init.BaudRate = cfg->baudrate;

    if (cfg->databits == HAL_UART_DATABITS_8)
        huart->Init.WordLength = UART_WORDLENGTH_8B;
    else if (cfg->databits == HAL_UART_DATABITS_9)
        huart->Init.WordLength = UART_WORDLENGTH_9B;
    else
        return NULL;

    huart->Init.StopBits =
        (cfg->stopbits == HAL_UART_STOPBIT_2) ?
        UART_STOPBITS_2 : UART_STOPBITS_1;

    if (cfg->parity == HAL_UART_PARITY_EVEN)
        huart->Init.Parity = UART_PARITY_EVEN;
    else if (cfg->parity == HAL_UART_PARITY_ODD)
        huart->Init.Parity = UART_PARITY_ODD;
    else
        huart->Init.Parity = UART_PARITY_NONE;

    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(huart) != HAL_OK)
        return NULL;

    drv->huart = huart;

    if (drv->mode == UART_MODE_INTERRUPT)
    {
        drv->rx_enabled = true;
        HAL_UART_Receive_IT(drv->huart, &drv->rx_byte, 1);
    }

    return (hal_uart_drv_t)drv;
}

static void stm32_uart_close(hal_uart_drv_t dev)
{
    struct hal_uart_drv_s *drv = (struct hal_uart_drv_s *)dev;

    if (!drv || !drv->huart)
        return;

    HAL_UART_AbortReceive(drv->huart);
    HAL_UART_AbortTransmit(drv->huart);
    HAL_UART_DeInit(drv->huart);

    drv->huart = NULL;
    drv->rx_enabled = false;
    drv->tx_busy = false;
}

/* ========================================================= */
/* ================= WRITE ================================= */
/* ========================================================= */

static uart_status_t stm32_uart_write(hal_uart_drv_t dev,
                                      const uint8_t *data,
                                      size_t len,
                                      size_t *written,
                                      uint32_t timeout_ms)
{
    struct hal_uart_drv_s *drv = (struct hal_uart_drv_s *)dev;

    if (!drv || !drv->huart || !data || len == 0)
        return UART_STATUS_ERROR;

    if (drv->mode == UART_MODE_POLLING)
    {
        if (HAL_UART_Transmit(drv->huart, (uint8_t *)data, len, timeout_ms) != HAL_OK)
            return UART_STATUS_TIMEOUT;
    }
    else
    {
        drv->tx_busy = true;
        if (HAL_UART_Transmit_IT(drv->huart, (uint8_t *)data, len) != HAL_OK)
        {
            drv->tx_busy = false;
            return UART_STATUS_ERROR;
        }
    }

    if (written)
        *written = len;

    return UART_STATUS_OK;
}

/* ========================================================= */
/* ================= READ (OPTIONAL) ======================= */
/* ========================================================= */

static uart_status_t stm32_uart_read(hal_uart_drv_t dev,
                                     uint8_t *data,
                                     size_t maxlen,
                                     size_t *out_len,
                                     uint32_t timeout_ms)
{
    struct hal_uart_drv_s *drv = (struct hal_uart_drv_s *)dev;
    size_t count = 0;

    if (!drv || !data || maxlen == 0)
        return UART_STATUS_ERROR;

    if (!drv->rx_done && drv->rx_done_mode != UART_RX_DONE_NONE)
        return UART_STATUS_TIMEOUT;

    if (drv->rx_len == 0)
        return UART_STATUS_TIMEOUT;

    count = (drv->rx_len < maxlen) ? drv->rx_len : maxlen;
    memcpy(data, drv->rx_buf, count);

    drv->rx_len = 0;
    drv->rx_done = false;

    if (out_len)
        *out_len = count;

    return UART_STATUS_OK;
}

/* ========================================================= */
/* ================= FLUSH ================================= */
/* ========================================================= */

static void stm32_uart_flush(hal_uart_drv_t dev)
{
    struct hal_uart_drv_s *drv = (struct hal_uart_drv_s *)dev;

    if (!drv)
        return;

    drv->rx_len = 0;
    drv->rx_wr  = 0;
    drv->rx_rd  = 0;
    drv->rx_done = false;
    drv->tx_busy = false;
}

/* ========================================================= */
/* ================= CALLBACK REG ========================== */
/* ========================================================= */

static void stm32_uart_set_event_cb(hal_uart_drv_t dev,
                                    hal_uart_event_cb_t cb,
                                    void *ctx)
{
    struct hal_uart_drv_s *drv = (struct hal_uart_drv_s *)dev;
    if (!drv)
        return;

    drv->cb = cb;
    drv->cb_ctx = ctx;
}

/* ========================================================= */
/* ================= HAL CALLBACKS ========================= */
/* ========================================================= */

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < HAL_UART_DEVS_N; i++)
    {
        if (uart_instances[i].huart == huart)
        {
            uart_instances[i].tx_busy = false;

            if (uart_instances[i].cb)
            {
                uart_instances[i].cb((hal_uart_drv_t)&uart_instances[i],
                                      UART_EVENT_TX_DONE,
                                      UART_STATUS_OK,
                                      NULL, 0,
                                      uart_instances[i].cb_ctx);
            }
            break;
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < HAL_UART_DEVS_N; i++)
    {
        struct hal_uart_drv_s *drv = &uart_instances[i];

        if (drv->huart != huart || !drv->rx_enabled)
            continue;

        /* Store byte */
        if (drv->rx_mode == UART_RX_MODE_LINEAR)
        {
            if (drv->rx_len < drv->rx_buf_size)
                drv->rx_buf[drv->rx_len++] = drv->rx_byte;
        }
        else
        {
            size_t next = (drv->rx_wr + 1) % drv->rx_buf_size;
            if (next != drv->rx_rd)
            {
                drv->rx_buf[drv->rx_wr] = drv->rx_byte;
                drv->rx_wr = next;
            }
        }

        /* RX DONE conditions */
        if (drv->rx_done_mode == UART_RX_DONE_ON_CHAR &&
            drv->rx_byte == drv->rx_done_char)
        {
            if (drv->rx_len > 0)
                drv->rx_len--;  // remove terminador

            if (drv->cb)
            {
                drv->cb((hal_uart_drv_t)drv,
                        UART_EVENT_RX_DONE,
                        UART_STATUS_OK,
                        drv->rx_buf,
                        drv->rx_len,
                        drv->cb_ctx);
            }

            drv->rx_len = 0;
        }

        if (drv->rx_done_mode == UART_RX_DONE_ON_LENGTH &&
            drv->rx_len >= drv->rx_done_length)
        {
            drv->rx_done = true;
            if (drv->timer_stop)
                drv->timer_stop(drv->timer_ctx);
        }

        /* Restart timeout timer */
        if (drv->rx_done_mode == UART_RX_DONE_ON_TIMEOUT &&
            drv->timer_start)
        {
            drv->timer_stop(drv->timer_ctx);
            drv->timer_start(drv->timer_ctx);
        }

        HAL_UART_Receive_IT(drv->huart, &drv->rx_byte, 1);
        break;
    }
}

static void stm32_uart_set_rx_timeout_timer(hal_uart_drv_t dev,
                                            hal_uart_timer_start_fn_t start,
                                            hal_uart_timer_stop_fn_t stop,
                                            void *ctx)
{
    struct hal_uart_drv_s *drv = (struct hal_uart_drv_s *)dev;
    if (!drv) return;

    drv->timer_start = start;
    drv->timer_stop  = stop;
    drv->timer_ctx   = ctx;
}


/* ========================================================= */
/* ================= IRQ HANDLERS ========================== */
/* ========================================================= */

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart1);
}

void USART2_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart2);
}

void USART3_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart3);
}


/* ========================================================= */
/* ================= DRIVER EXPORT ========================= */
/* ========================================================= */

hal_uart_drv_imp_t HAL_UART_DRV = {
    .init = stm32_uart_init,
    .deinit = stm32_uart_deinit,
    .open = stm32_uart_open,
    .close = stm32_uart_close,
    .write = stm32_uart_write,
    .read = stm32_uart_read,
    .flush = stm32_uart_flush,
	.set_rx_timeout_timer = stm32_uart_set_rx_timeout_timer,
    .set_event_cb = stm32_uart_set_event_cb,
};
