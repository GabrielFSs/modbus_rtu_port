#include "mb.h"
#include "mbport.h"
#include "mb_port.h"

#include "hal_uart.h"
#include "hal_gpio.h"

/* ========================================================= */
/* GLOBALS                                                   */
/* ========================================================= */

static hal_uart_drv_t mb_uart = NULL;

static hal_gpio_drv_t gpio_de = NULL;
static hal_gpio_drv_t gpio_re = NULL;

/* RX byte */
static volatile uint8_t rx_byte;

/* TX byte */
static volatile uint8_t tx_byte;

/* RX buffer físico */
static uint8_t rx_buf[1];


/* ========================================================= */
/* RS485 DIRECTION CONTROL                                   */
/* ========================================================= */

static void rs485_dir_ctrl(void *ctx, hal_uart_dir_t dir)
{
    (void)ctx;

    if (!gpio_de || !gpio_re)
        return;

    if (dir == HAL_UART_DIR_TX)
    {
        /* habilita TX */
        hal_gpio_write(gpio_de, true);
        hal_gpio_write(gpio_re, true);
    }
    else
    {
        /* habilita RX */
        hal_gpio_write(gpio_de, false);
        hal_gpio_write(gpio_re, false);
    }
}


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

    /* ================= MAP PORT ================= */

    switch (ucPort)
    {
        case 0: dev = HAL_UART_DEV_1; break;
        case 1: dev = HAL_UART_DEV_2; break;
        case 2: dev = HAL_UART_DEV_3; break;
        default: return FALSE;
    }

    /* ================= GPIO RS485 ================= */

    hal_gpio_cfg_t gpio_cfg =
    {
        .direction = HAL_GPIO_OUTPUT,
        .pull      = HAL_GPIO_NOPULL,
        .out_type  = HAL_GPIO_PUSH_PULL,
        .irq_edge  = HAL_GPIO_IRQ_NONE
    };

    gpio_de = hal_gpio_open(HAL_GPIO_RS485_DE, &gpio_cfg);
    gpio_re = hal_gpio_open(HAL_GPIO_RS485_RE, &gpio_cfg);

    if (!gpio_de || !gpio_re)
        return FALSE;

    /* inicia em RX */

    hal_gpio_write(gpio_de, false);
    hal_gpio_write(gpio_re, false);

    /* ================= BAUD ================= */

    cfg.baudrate = (hal_uart_baud_t)ulBaudRate;

    /* ================= DATA BITS ================= */

    cfg.databits =
        (ucDataBits == 9) ?
        HAL_UART_DATABITS_9 :
        HAL_UART_DATABITS_8;

    /* ================= STOP BITS ================= */

    cfg.stopbits =
        (ucStopBits == 2) ?
        HAL_UART_STOPBIT_2 :
        HAL_UART_STOPBIT_1;

    /* ================= PARITY ================= */

    cfg.parity =
        (eParity == MB_PAR_NONE) ? HAL_UART_PARITY_NONE :
        (eParity == MB_PAR_EVEN) ? HAL_UART_PARITY_EVEN :
                                   HAL_UART_PARITY_ODD;

    /* ================= MODE ================= */

    cfg.comm_mode   = UART_MODE_INTERRUPT;
    cfg.duplex_mode = UART_DUPLEX_HALF;

    cfg.comm_control = UART_DIR_EXTERNAL;
    cfg.dir_ctrl     = rs485_dir_ctrl;
    cfg.dir_ctrl_ctx = NULL;

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


/* ========================================================= */
/* SERIAL CLOSE                                              */
/* ========================================================= */

void vMBPortSerialClose(void)
{
    if (mb_uart)
    {
        hal_uart_close(mb_uart);
        mb_uart = NULL;
    }

    if (gpio_de)
    {
        hal_gpio_close(gpio_de);
        gpio_de = NULL;
    }

    if (gpio_re)
    {
        hal_gpio_close(gpio_re);
        gpio_re = NULL;
    }
}


/* ========================================================= */
/* ENABLE / DISABLE RX & TX                                  */
/* ========================================================= */

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    (void)xRxEnable;

    if (xTxEnable)
    {
        if (pxMBFrameCBTransmitterEmpty)
        {
            pxMBFrameCBTransmitterEmpty();
        }
    }
}


/* ========================================================= */
/* SEND BYTE                                                 */
/* ========================================================= */

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    size_t written;

    tx_byte = (uint8_t)ucByte;

    return (hal_uart_write(mb_uart,
                           (uint8_t *)&tx_byte,
                           1,
                           &written,
                           100) == UART_STATUS_OK);
}


/* ========================================================= */
/* GET BYTE                                                  */
/* ========================================================= */

BOOL xMBPortSerialGetByte(CHAR *pucByte)
{
    if (!pucByte)
        return FALSE;

    *pucByte = (CHAR)rx_byte;
    return TRUE;
}
