#include "mb.h"
#include "mbport.h"
#include "mb_port.h"
#include "hal_uart.h"

static hal_uart_drv_t mb_uart = NULL;
static volatile uint8_t rx_byte;

/* ========================================== */
/* UART EVENT CALLBACK                       */
/* ========================================== */

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

    /* ================= RX ================= */
    if (event == UART_EVENT_RX_DONE && data && len > 0)
    {
        rx_byte = data[0];

        if (pxMBFrameCBByteReceived)
            pxMBFrameCBByteReceived();
    }

    /* ================= TX ================= */
    if (event == UART_EVENT_TX_DONE)
    {
        if (pxMBFrameCBTransmitterEmpty)
            pxMBFrameCBTransmitterEmpty();
    }
}

/* ========================================== */
/* SERIAL INIT                               */
/* ========================================== */

BOOL xMBPortSerialInit(UCHAR ucPort,
                       ULONG ulBaudRate,
                       UCHAR ucDataBits,
                       eMBParity eParity,
                       UCHAR ucStopBits)
{
    (void)ucPort;
    (void)ucDataBits;
    (void)ucStopBits;

    hal_uart_cfg_t cfg = {0};

    cfg.baudrate = ulBaudRate;
    cfg.databits = HAL_UART_DATABITS_8;
    cfg.stopbits = HAL_UART_STOPBIT_1;

    cfg.parity =
        (eParity == MB_PAR_NONE) ? HAL_UART_PARITY_NONE :
        (eParity == MB_PAR_EVEN) ? HAL_UART_PARITY_EVEN :
                                   HAL_UART_PARITY_ODD;

    cfg.comm_mode   = UART_MODE_INTERRUPT;
    cfg.duplex_mode = UART_DUPLEX_FULL;
    cfg.comm_control = UART_DIR_NONE;

    static uint8_t rx_buf[1];

    cfg.rx_mode        = UART_RX_MODE_LINEAR;
    cfg.rx_buffer      = rx_buf;
    cfg.rx_buffer_size = 1;

    cfg.rx_done_mode   = UART_RX_DONE_ON_LENGTH;
    cfg.rx_done_length = 1;

    mb_uart = hal_uart_open(HAL_UART_DEV_3, &cfg);
    if (!mb_uart)
        return FALSE;

    hal_uart_set_event_cb(mb_uart, mb_uart_event_cb, NULL);

    return TRUE;
}

/* ========================================== */
/* ENABLE / DISABLE RX & TX                  */
/* ========================================== */

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    if (xRxEnable)
        hal_uart_rx_enable(mb_uart);
    else
        hal_uart_rx_disable(mb_uart);

    if (xTxEnable)
        hal_uart_tx_it_enable(mb_uart);
    else
        hal_uart_tx_it_disable(mb_uart);
}

/* ========================================== */
/* SEND ONE BYTE                             */
/* ========================================== */

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    size_t written;

    return (hal_uart_write(mb_uart,
                           (uint8_t *)&ucByte,
                           1,
                           &written,
                           100) == UART_STATUS_OK);
}

/* ========================================== */
/* GET ONE BYTE                              */
/* ========================================== */

BOOL xMBPortSerialGetByte(CHAR *pucByte)
{
    if (!pucByte)
        return FALSE;

    *pucByte = (CHAR)rx_byte;
    return TRUE;
}
