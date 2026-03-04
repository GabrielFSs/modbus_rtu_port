#include "modbus_manager.h"

/* FreeModbus (Slave) */
#include "mb.h"
#include "mbport.h"
#include "mb_port.h"

/* Master (mbm + port próprio) */
#include "mbm.h"
#include "mbm_port.h"

#include "hal_uart.h"
#include <stddef.h>

/* ============================================================ */

static modbus_mode_t current_mode = MODBUS_MODE_NONE;
static bool sniffer_enabled = false;

/* Slave: ID usado em eMBInit (alterável via modbus_manager_set_slave_id) */
static uint8_t slave_address = 0x01;

/* UART/port index: 0=UART1, 1=UART2, 2=UART3 (usado em master e slave) */
#define DEFAULT_UART_INDEX  2

/* Última config usada (para apply_serial_cfg reiniciar no mesmo modo) */
static modbus_serial_cfg_t last_cfg = {
    .baudrate  = 9600,
    .databits  = 8,
    .parity    = 0,
    .stopbits  = 1,
    .uart_index = DEFAULT_UART_INDEX
};

/* ============================================================ */

static eMBParity cfg_parity_to_mb(uint8_t parity)
{
    switch (parity)
    {
        case 0: return MB_PAR_NONE;
        case 1: return MB_PAR_EVEN;
        case 2: return MB_PAR_ODD;
        default: return MB_PAR_NONE;
    }
}

static void cfg_to_mbm_serial(const modbus_serial_cfg_t *cfg,
                              uint8_t uart_index,
                              mbm_serial_cfg_t *out)
{
    if (!cfg || !out)
        return;

    out->baudrate = cfg->baudrate;
    out->databits = cfg->databits;
    out->parity   = (cfg->parity == 0) ? HAL_UART_PARITY_NONE :
                    (cfg->parity == 1) ? HAL_UART_PARITY_EVEN :
                                         HAL_UART_PARITY_ODD;
    out->stopbits = (cfg->stopbits == 2) ? HAL_UART_STOPBIT_2 : HAL_UART_STOPBIT_1;
    out->uart     = (uart_index <= 2) ? (int)(HAL_UART_DEV_1 + uart_index) : (int)HAL_UART_DEV_3;
}

/* ============================================================ */

void modbus_manager_init(void)
{
    current_mode = MODBUS_MODE_NONE;
    sniffer_enabled = false;
    slave_address = 0x01;
}

/* ============================================================ */

bool modbus_manager_start(modbus_mode_t mode,
                          const modbus_serial_cfg_t *cfg)
{
    if (!cfg)
        return false;

    if (mode != MODBUS_MODE_SLAVE &&
        mode != MODBUS_MODE_MASTER)
        return false;

    modbus_manager_stop();

    /* Guardar config para apply_serial_cfg */
    last_cfg.baudrate   = cfg->baudrate;
    last_cfg.databits  = cfg->databits;
    last_cfg.parity    = cfg->parity;
    last_cfg.stopbits  = cfg->stopbits;
    last_cfg.uart_index = (cfg->uart_index <= 2) ? cfg->uart_index : DEFAULT_UART_INDEX;

    /* ================= SLAVE ================= */

    if (mode == MODBUS_MODE_SLAVE)
    {
        eMBParity parity = cfg_parity_to_mb(cfg->parity);
        uint8_t port = (cfg->uart_index <= 2) ? cfg->uart_index : DEFAULT_UART_INDEX;
        UCHAR ucPort = (UCHAR)port;

        if (eMBInit(MB_RTU,
                    slave_address,
                    ucPort,
                    cfg->baudrate,
                    parity,
                    cfg->stopbits) != MB_ENOERR)
        {
            return false;
        }

        if (eMBEnable() != MB_ENOERR)
        {
            eMBClose();
            return false;
        }
    }

    /* ================= MASTER ================= */

    if (mode == MODBUS_MODE_MASTER)
    {
        mbm_serial_cfg_t mbm_cfg;
        uint8_t uart_idx = (cfg->uart_index <= 2) ? cfg->uart_index : DEFAULT_UART_INDEX;

        cfg_to_mbm_serial(cfg, uart_idx, &mbm_cfg);

        if (mbm_port_open(&mbm_cfg) != MBM_ERR_OK)
            return false;

        if (mbm_enable() != MBM_ERR_OK)
        {
            mbm_port_close();
            return false;
        }
    }

    current_mode = mode;
    return true;
}

/* ============================================================ */

void modbus_manager_stop(void)
{
    if (current_mode == MODBUS_MODE_SLAVE)
    {
        eMBDisable();
        eMBClose();
    }
    else if (current_mode == MODBUS_MODE_MASTER)
    {
        mbm_disable();
        mbm_port_close();
    }

    current_mode = MODBUS_MODE_NONE;
}

/* ============================================================ */

void modbus_manager_poll(void)
{
    if (current_mode == MODBUS_MODE_SLAVE)
        eMBPoll();
    else if (current_mode == MODBUS_MODE_MASTER)
        mbm_poll();
}

/* ============================================================ */

void modbus_manager_enable_sniffer(bool enable)
{
    sniffer_enabled = enable;
}

modbus_mode_t modbus_manager_get_mode(void)
{
    return current_mode;
}

/* ============================================================
 * HELPERS PARA GUI / RUNTIME
 * ============================================================ */

bool modbus_manager_apply_serial_cfg(const modbus_serial_cfg_t *cfg)
{
    if (cfg)
    {
        last_cfg.baudrate   = cfg->baudrate;
        last_cfg.databits   = cfg->databits;
        last_cfg.parity     = cfg->parity;
        last_cfg.stopbits   = cfg->stopbits;
        last_cfg.uart_index = (cfg->uart_index <= 2) ? cfg->uart_index : DEFAULT_UART_INDEX;
    }

    if (current_mode == MODBUS_MODE_NONE)
        return true;

    return modbus_manager_start(current_mode, &last_cfg);
}

void modbus_manager_set_slave_id(uint8_t slave_id)
{
    slave_address = slave_id;
}

bool modbus_manager_add_request(const void *req)
{
    if (current_mode != MODBUS_MODE_MASTER || !req)
        return false;
    return (mbm_add_request((const mbm_request_t *)req) == MBM_ERR_OK);
}
