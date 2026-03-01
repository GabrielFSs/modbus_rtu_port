#include "mb.h"
#include "mbport.h"
#include "mb_port.h"
#include "hal_uart.h"

/* ========================================================= */
/* VARIÁVEIS GLOBAIS                                         */
/* ========================================================= */

static hal_uart_drv_t mb_uart = NULL;

/* RX precisa armazenar o último byte recebido */
static volatile uint8_t rx_byte;

/* ⚠️ TX byte precisa ser estático (NÃO pode ser local!) */
static volatile uint8_t tx_byte;

/* Buffer RX físico (1 byte) */
static uint8_t rx_buf[1];


/* ========================================================= */
/* UART EVENT CALLBACK                                       */
/* ========================================================= */

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
        {
            pxMBFrameCBByteReceived();
        }
    }

    /* ================= TX ================= */
    if (event == UART_EVENT_TX_DONE)
    {
        if (pxMBFrameCBTransmitterEmpty)
        {
            pxMBFrameCBTransmitterEmpty();
        }
    }
}


/* ========================================================= */
/* SERIAL INIT                                               */
/* ========================================================= */

BOOL xMBPortSerialInit(UCHAR ucPort,
                       ULONG ulBaudRate,
                       UCHAR ucDataBits,
                       eMBParity eParity,
                       UCHAR ucStopBits)
{
    hal_uart_cfg_t cfg = {0};
    hal_uart_dev_interface_t dev;

    /* ================= MAPEAR PORT ================= */

    switch (ucPort)
    {
        case 0: dev = HAL_UART_DEV_1; break;
        case 1: dev = HAL_UART_DEV_2; break;
        case 2: dev = HAL_UART_DEV_3; break;
        default: return FALSE;
    }

    /* ================= BAUD ================= */

    cfg.baudrate = (hal_uart_baud_t)ulBaudRate;

    /* ================= DATA BITS ================= */

    cfg.databits = (ucDataBits == 9)
                 ? HAL_UART_DATABITS_9
                 : HAL_UART_DATABITS_8;

    /* ================= STOP BITS ================= */

    cfg.stopbits = (ucStopBits == 2)
                 ? HAL_UART_STOPBIT_2
                 : HAL_UART_STOPBIT_1;

    /* ================= PARITY ================= */

    cfg.parity =
        (eParity == MB_PAR_NONE) ? HAL_UART_PARITY_NONE :
        (eParity == MB_PAR_EVEN) ? HAL_UART_PARITY_EVEN :
                                   HAL_UART_PARITY_ODD;

    /* ================= MODE ================= */

    cfg.comm_mode   = UART_MODE_INTERRUPT;
    cfg.duplex_mode = UART_DUPLEX_FULL;
    cfg.comm_control = UART_DIR_NONE;

    /* ================= RX CONFIG ================= */

    cfg.rx_mode        = UART_RX_MODE_LINEAR;
    cfg.rx_buffer      = rx_buf;
    cfg.rx_buffer_size = 1;

    cfg.rx_done_mode   = UART_RX_DONE_ON_LENGTH;
    cfg.rx_done_length = 1;

    /* ================= OPEN UART ================= */

    mb_uart = hal_uart_open(dev, &cfg);
    if (!mb_uart)
        return FALSE;

    hal_uart_set_event_cb(mb_uart, mb_uart_event_cb, NULL);

    return TRUE;
}

void vMBPortSerialClose(void)
{
    if (mb_uart)
    {
        hal_uart_close(mb_uart);
        mb_uart = NULL;
    }
}


/* ========================================================= */
/* ENABLE / DISABLE RX & TX                                  */
/* ========================================================= */

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    if (xRxEnable)
        hal_uart_rx_enable(mb_uart);
    else
        hal_uart_rx_disable(mb_uart);

    if (xTxEnable)
    {
        /* Força início da transmissão */
        if (pxMBFrameCBTransmitterEmpty)
        {
            pxMBFrameCBTransmitterEmpty();
        }
    }
}


/* ========================================================= */
/* SEND ONE BYTE (BYTE A BYTE - INTERRUPT SAFE)             */
/* ========================================================= */

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    size_t written;

    /* ⚠️ NÃO usar variável local */
    tx_byte = (uint8_t)ucByte;

    return (hal_uart_write(mb_uart,
                           (uint8_t *)&tx_byte,
                           1,
                           &written,
                           100) == UART_STATUS_OK);
}


/* ========================================================= */
/* GET ONE BYTE                                              */
/* ========================================================= */

BOOL xMBPortSerialGetByte(CHAR *pucByte)
{
    if (!pucByte)
        return FALSE;

    *pucByte = (CHAR)rx_byte;
    return TRUE;
}
