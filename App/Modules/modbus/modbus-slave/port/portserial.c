#include "mb.h"
#include "mbport.h"
#include "mb_port.h"

#include "hal_uart.h"
#include "hal_gpio.h"
#include "mb_sniffer.h"

/* ========================================================= */
/* RS485 POLICY (UART3 = RS485)                              */
/* ========================================================= */

#define RS485_DE_GPIO   HAL_GPIO_3   /* ajuste conforme seu BSP */
#define RS485_RE_GPIO   HAL_GPIO_4   /* ajuste conforme seu BSP */

static hal_gpio_drv_t rs485_de = NULL;
static hal_gpio_drv_t rs485_re = NULL;
static BOOL rs485_active = FALSE;

static void rs485_init(void)
{
    if (rs485_active)
        return;

    hal_gpio_cfg_t cfg =
    {
        .direction = HAL_GPIO_OUTPUT,
        .pull      = HAL_GPIO_NOPULL,
        .out_type  = HAL_GPIO_PUSH_PULL,
        .irq_edge  = HAL_GPIO_IRQ_NONE
    };

    rs485_de = hal_gpio_open(RS485_DE_GPIO, &cfg);
    rs485_re = hal_gpio_open(RS485_RE_GPIO, &cfg);

    if (rs485_de) hal_gpio_write(rs485_de, FALSE);
    if (rs485_re) hal_gpio_write(rs485_re, FALSE);

    rs485_active = TRUE;
}

static void rs485_deinit(void)
{
    if (!rs485_active)
        return;

    if (rs485_de)
    {
        hal_gpio_close(rs485_de);
        rs485_de = NULL;
    }

    if (rs485_re)
    {
        hal_gpio_close(rs485_re);
        rs485_re = NULL;
    }

    rs485_active = FALSE;
}

static void rs485_dir_control(void *ctx, bool enable)
{
    (void)ctx;

    if (!rs485_active)
        return;

    if (enable)
    {
        hal_gpio_write(rs485_de, TRUE);
        hal_gpio_write(rs485_re, TRUE);
    }
    else
    {
        hal_gpio_write(rs485_de, FALSE);
        hal_gpio_write(rs485_re, FALSE);
    }
}

/* ========================================================= */
/* VARIÁVEIS GLOBAIS                                         */
/* ========================================================= */

static hal_uart_drv_t mb_uart = NULL;
static hal_uart_dev_interface_t current_dev;

static volatile uint8_t rx_byte;
static volatile uint8_t tx_byte;

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

        mb_sniffer_rx_byte(rx_byte);

        if (pxMBFrameCBByteReceived)
        {
            pxMBFrameCBByteReceived();
        }
    }

    /* ================= TX ================= */

    if (event == UART_EVENT_TX_DONE)
    {
    	mb_sniffer_tx_confirm();

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

    current_dev = dev;

    /* ================= RS485 POLICY ================= */

    BOOL use_rs485 = (dev == HAL_UART_DEV_3);

    if (use_rs485)
        rs485_init();

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

    cfg.comm_mode = UART_MODE_INTERRUPT;

    if (use_rs485)
    {
        cfg.duplex_mode  = UART_DUPLEX_HALF;
        cfg.comm_control = UART_DIR_GPIO;
        cfg.dir_ctrl     = rs485_dir_control;
        cfg.dir_ctrl_ctx = NULL;
    }
    else
    {
        cfg.duplex_mode  = UART_DUPLEX_FULL;
        cfg.comm_control = UART_DIR_NONE;
    }

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
/* CLOSE                                                     */
/* ========================================================= */

void vMBPortSerialClose(void)
{
    if (mb_uart)
    {
        hal_uart_close(mb_uart);
        mb_uart = NULL;
    }

    if (current_dev == HAL_UART_DEV_3)
    {
        rs485_deinit();
    }
}

/* ========================================================= */
/* ENABLE / DISABLE RX & TX                                  */
/* ========================================================= */

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable)
{
    if (!mb_uart)
        return;

    if (xRxEnable)
        hal_uart_rx_enable(mb_uart);
    else
        hal_uart_rx_disable(mb_uart);

    if (xTxEnable)
    {
        if (pxMBFrameCBTransmitterEmpty)
        {
            pxMBFrameCBTransmitterEmpty();
        }
    }
}

/* ========================================================= */
/* SEND ONE BYTE                                             */
/* ========================================================= */

BOOL xMBPortSerialPutByte(CHAR ucByte)
{
    size_t written;

    tx_byte = (uint8_t)ucByte;

    mb_sniffer_tx_store(&tx_byte, 1);

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
