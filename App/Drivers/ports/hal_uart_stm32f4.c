#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_uart.h"
#include "hal_uart.h"
#include "bsp_uart.h"


struct hal_uart_drv_s
{
    hal_uart_dev_interface_t iface;

    UART_HandleTypeDef *huart;

    uart_mode_t mode;
    uart_duplex_t duplex;

    /* RX */
    bool rx_enabled;
    uint8_t rx_byte;

    uint8_t  *rx_buf;
    size_t    rx_buf_size;
    size_t    rx_len;
    
    uart_rx_mode_t rx_mode;

    /* RX circular only */
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

    hal_uart_baud_t baudrate;
    hal_uart_databits_t databits;
    hal_uart_dev_parity_t parity;
    hal_uart_dev_stopbit_t stopbits;
};

static struct hal_uart_drv_s uart_instances[HAL_UART_DEVS_N];

static void stm32_uart_init(void)
{
    /* Limpa estruturas */
    memset(uart_instances, 0, sizeof(uart_instances));

    /* Clocks globais necessários ao driver */
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* Não inicializa nenhuma UART aqui */
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

static hal_uart_drv_t stm32_uart_open(hal_uart_dev_interface_t interface, const hal_uart_cfg_t *cfg)
{
    struct hal_uart_drv_s *drv;
    UART_HandleTypeDef *huart;

    if (!cfg)
    {
        return NULL;
    }
        

    if (interface >= HAL_UART_DEVS_N)
    {
        return NULL;
    }

    drv = &uart_instances[interface];
    memset(drv, 0, sizeof(*drv));

    drv->iface = interface;
    drv->mode    = cfg->comm_mode;
    drv->duplex  = cfg->duplex_mode;
    drv->dir_mode = cfg->comm_control;
    drv->baudrate = cfg->baudrate;
    drv->databits = cfg->databits;
    drv->parity   = cfg->parity;
    drv->stopbits = cfg->stopbits;
    drv->rx_mode     = cfg->rx_mode;
    drv->rx_buf      = cfg->rx_buffer;
    drv->rx_buf_size = cfg->rx_buffer_size;

    drv->rx_len = 0;
    drv->rx_wr  = 0;
    drv->rx_rd  = 0;

    static UART_HandleTypeDef huart1;
    static UART_HandleTypeDef huart2;
    static UART_HandleTypeDef huart3;

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

    switch (cfg->databits)
    {
        case HAL_UART_DATABITS_8:
            huart->Init.WordLength = UART_WORDLENGTH_8B;
            break;

        case HAL_UART_DATABITS_9:
            huart->Init.WordLength = UART_WORDLENGTH_9B;
            break;

        case HAL_UART_DATABITS_7:
        default:
            return NULL; // configuração inválida para STM32F4
    }

    huart->Init.StopBits =
        (cfg->stopbits == HAL_UART_STOPBIT_2) ?
        UART_STOPBITS_2 : UART_STOPBITS_1;

    switch (cfg->parity)
    {
        case HAL_UART_PARITY_EVEN:
            huart->Init.Parity = UART_PARITY_EVEN;
            break;
        case HAL_UART_PARITY_ODD:
            huart->Init.Parity = UART_PARITY_ODD;
            break;
        default:
            huart->Init.Parity = UART_PARITY_NONE;
            break;
    }

    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart->Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(huart) != HAL_OK)
    {
        return NULL;
    }
    
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


    if (!dev)
    {
        return;
    }

    if (drv->huart)
    {
        /* Aborta recepção em andamento */
        HAL_UART_AbortReceive(drv->huart);

        /* Aborta transmissão em andamento */
        HAL_UART_AbortTransmit(drv->huart);

        HAL_UART_DeInit(drv->huart);
    }
    
    drv->huart = NULL;
    drv->rx_enabled = false;
    drv->tx_busy = false;

}

static uart_status_t stm32_uart_write(hal_uart_drv_t dev, const uint8_t *data, size_t len, size_t *written, uint32_t timeout_ms)
{
    struct hal_uart_drv_s *drv = (struct hal_uart_drv_s *)dev;


    if (!dev || !data || len == 0)
    {
        return UART_STATUS_ERROR;
    }
        

    drv = (struct hal_uart_drv_s *)dev;

    if (!drv->huart)
    {
        return UART_STATUS_ERROR;
    }
    
    if (drv->dir_mode == UART_DIR_GPIO)
    {
        HAL_GPIO_WritePin(drv->de_port, drv->de_pin, GPIO_PIN_SET);
    }

    if (drv->mode == UART_MODE_POLLING)
    {
        HAL_StatusTypeDef st;

        st = HAL_UART_Transmit(drv->huart, (uint8_t *)data, len, timeout_ms);

        if (st != HAL_OK)
        {
            return UART_STATUS_TIMEOUT;
        }
            
        if (written)
        {
            *written = len;
        }

        if (drv->dir_mode == UART_DIR_GPIO)
        {
            HAL_GPIO_WritePin(drv->de_port, drv->de_pin, GPIO_PIN_RESET);
        }

        return UART_STATUS_OK;
    }

    else if (drv->mode == UART_MODE_INTERRUPT)
    {
        drv->tx_busy = true;

        if (HAL_UART_Transmit_IT(drv->huart, (uint8_t *)data, len) != HAL_OK)
        {
            drv->tx_busy = false;
            return UART_STATUS_ERROR;
        }

        if (written)
        {
            *written = len;
        }
            
        return UART_STATUS_OK;
    }

    else if (drv->mode == UART_MODE_DMA)
    {
        drv->tx_busy = true;

        if (HAL_UART_Transmit_DMA(drv->huart, (uint8_t *)data, len) != HAL_OK)
        {
            drv->tx_busy = false;
            return UART_STATUS_ERROR;
        }

        if (written)
        {
            *written = len;
        }
            
        return UART_STATUS_OK;
    }

    return UART_STATUS_ERROR;

}

static uart_status_t stm32_uart_read(hal_uart_drv_t dev, uint8_t *data, size_t maxlen, size_t *out_len, uint32_t timeout_ms)
{
    struct hal_uart_drv_s *drv = (struct hal_uart_drv_s *)dev;

    if (!drv || !data || maxlen == 0)
        return UART_STATUS_ERROR;

    size_t count = 0;

    /* ---------- RX LINEAR (Modbus, frame) ---------- */
    if (drv->rx_mode == UART_RX_MODE_LINEAR)
    {
        if (drv->rx_len == 0)
            return UART_STATUS_TIMEOUT;

        count = (drv->rx_len < maxlen) ? drv->rx_len : maxlen;

        memcpy(data, drv->rx_buf, count);

        /* Consome tudo (frame já foi tratado) */
        drv->rx_len = 0;
    }

    /* ---------- RX CIRCULAR (CLI, stream) ---------- */
    else if (drv->rx_mode == UART_RX_MODE_CIRCULAR)
    {
        while ((drv->rx_rd != drv->rx_wr) && (count < maxlen))
        {
            data[count++] = drv->rx_buf[drv->rx_rd];
            drv->rx_rd = (drv->rx_rd + 1) % drv->rx_buf_size;
        }

        if (count == 0)
            return UART_STATUS_TIMEOUT;
    }

    else
    {
        return UART_STATUS_ERROR;
    }

    if (out_len)
        *out_len = count;

    return UART_STATUS_OK;
}

static void stm32_uart_flush(hal_uart_drv_t dev)
{
    struct hal_uart_drv_s *drv = (struct hal_uart_drv_s *)dev;

    if (!drv)
        return;

    /* RX */
    drv->rx_len = 0;
    drv->rx_wr  = 0;
    drv->rx_rd  = 0;

    /* TX */
    drv->tx_busy = false;
}

static void stm32_uart_set_event_cb(hal_uart_drv_t dev, hal_uart_event_cb_t cb, void *ctx)
{
    struct hal_uart_drv_s *drv = (struct hal_uart_drv_s *)dev;

    if (!drv)
        return;

    drv->cb = cb;
    drv->cb_ctx = ctx;
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    for (int i = 0; i < HAL_UART_DEVS_N; i++)
    {
        struct hal_uart_drv_s *drv = &uart_instances[i];

        if (drv->huart == huart)
        {
            drv->tx_busy = false;

            if (drv->dir_mode == UART_DIR_GPIO)
            {
                HAL_GPIO_WritePin(drv->de_port, drv->de_pin, GPIO_PIN_RESET);
            }

            if (drv->cb)
            {
                drv->cb((hal_uart_drv_t)drv, UART_EVENT_TX_DONE, UART_STATUS_OK, NULL, 0, drv->cb_ctx);
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

        if (drv->rx_len < drv->rx_buf_size)
        {
            drv->rx_buf[drv->rx_len++] = drv->rx_byte;

            /* ===== DETECTA ENTER (APP LOGIC) ===== */
            if (drv->rx_byte == '\n' || drv->rx_byte == '\r')
            {
                if (drv->cb)
                {
                    drv->cb((hal_uart_drv_t)drv,
                            UART_EVENT_RX_DONE,
                            UART_STATUS_OK,
                            drv->rx_buf,
                            drv->rx_len,
                            drv->cb_ctx);
                }
            }
        }

        /* Rearma RX */
        HAL_UART_Receive_IT(drv->huart, &drv->rx_byte, 1);
        break;
    }
}


void USART3_IRQHandler(void)
{
    for (int i = 0; i < HAL_UART_DEVS_N; i++)
    {
        if (uart_instances[i].huart &&
            uart_instances[i].huart->Instance == USART3)
        {
            HAL_UART_IRQHandler(uart_instances[i].huart);
            break;
        }
    }
}


hal_uart_drv_imp_t HAL_UART_DRV = {
    .init = stm32_uart_init,
    .deinit = stm32_uart_deinit,
    .open = stm32_uart_open,  
    .close = stm32_uart_close,
    .write = stm32_uart_write,
    .read = stm32_uart_read,
    .flush = stm32_uart_flush,
    .set_event_cb = stm32_uart_set_event_cb,
};

