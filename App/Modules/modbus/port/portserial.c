#include "port.h"

/* FreeModbus includes */
#include "mb.h"
#include "mbport.h"

/* HAL */
#include "hal_uart.h"

/* ================================
 * UART usada pelo Modbus
 * ================================ */
static hal_uart_drv_t mb_uart = NULL;

/* ================================
 * Callback do HAL_UART
 * ================================ */
static void mb_uart_event_cb(hal_uart_drv_t dev,
                             uart_event_t event,
                             uart_status_t status,
                             const uint8_t *data,
                             size_t len,
                             void *ctx)
{
    (void)dev;
    (void)status;
    (void)ctx;

    switch (event)
    {
        case UART_EVENT_RX_DONE:
            /* Um byte recebido */
            pxMBFrameCBByteReceived();
            break;

        case UART_EVENT_TX_DONE:
            /* Pronto para enviar próximo byte */
            pxMBFrameCBTransmitterEmpty();
            break;

        default:
            break;
    }
}

/* ================================
 * Inicialização da UART para Modbus
 * ================================ */
BOOL xMBPortSerialInit(UCHAR ucPort,
                       ULONG ulBaudRate,
                       UCHAR ucDataBits,
                       eMBParity eParity)
{
    (void)ucPort; /* Você pode usar isso depois para múltiplas UARTs */

    hal_uart_cfg_t cfg = {0};

    cfg.baudrate  = (hal_uart_baud_t)ulBaudRate;
    cfg.databits  = (hal_uart_databits_t)ucDataBits;
    cfg.stopbits  = HAL_UART_STOPBIT_1;

    switch (eParity)
    {
        case MB_PAR_NONE:
            cfg.parity = HAL_UART_PARITY_NONE;
            break;
        case MB_PAR_EVEN:
            cfg.parity = HAL_UART_PARITY_EVEN;
            break;
        case MB_PAR_ODD:
            cfg.parity = HAL_UART_PARITY_ODD;
            break;
        default:
            return FALSE;
    }

    cfg.comm_mode   = UART_MODE_INTERRUPT;
    cfg.duplex_mode = UART_DUPLEX_FULL;
    cfg.comm_control = UART_DIR_NONE;

    static uint8_t rx_buf[256];

    cfg.rx_mode        = UART_RX_MODE_LINEAR;
    cfg.rx_buffer      = rx_buf;
    cfg.rx_buffer_size = sizeof(rx_buf);

    cfg.tx_buffer      = NULL;
    cfg.tx_buffer_size = 0;

    mb_uart = hal_uart_open(HAL_UART_DEV_3, &cfg);
    if (!mb_uart)
        return FALSE;

    hal_uart_set_event_cb(mb_uart, mb_uart_event_cb, NULL);

    return TRUE;
}

/* ================================
 * Habilita RX / TX
 * ================================ */
void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    /* RX e TX são controlados pelo HAL internamente.
       Aqui só garantimos que o driver está ativo. */

    (void)xRxEnable;
    (void)xTxEnable;
}

/* ================================
 * Envia 1 byte (TX)
 * ================================ */
BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    size_t written;

    if (!mb_uart)
        return FALSE;

    return (hal_uart_write(mb_uart,
                           (uint8_t *)&ucByte,
                           1,
                           &written,
                           0) == UART_STATUS_OK);
}

/* ================================
 * Lê 1 byte (RX)
 * ================================ */
BOOL xMBPortSerialGetByte(CHAR *pucByte)
{
    size_t read;

    if (!mb_uart || !pucByte)
        return FALSE;

    return (hal_uart_read(mb_uart,
                          (uint8_t *)pucByte,
                          1,
                          &read,
                          0) == UART_STATUS_OK);
}
