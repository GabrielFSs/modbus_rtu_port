#include "modbus_manager.h"
#include "port_modbus_manager.h"

/* FreeModbus (Slave) */
#include "mb.h"
#include "mbport.h"
#include "mb_port.h"

/* Master */
#include "mb_master.h"

/* ============================================================ */

static modbus_mode_t current_mode = MODBUS_MODE_NONE;
static bool sniffer_enabled = false;

/* ============================================================
 * Timer físico controlado pelo manager
 * ============================================================ */

void mb_port_timer_restart(void)
{
    if (modbus_manager_port.timer_restart)
        modbus_manager_port.timer_restart();
}

void mb_port_timer_stop(void)
{
    /* opcional */
}

/* ============================================================
 * CALLBACKS DO PORT (HAL → Manager)
 * ============================================================ */

static void manager_uart_rx_cb(const uint8_t *data, size_t len)
{
    if (!data || len == 0 || current_mode == MODBUS_MODE_NONE)
        return;

    if (current_mode == MODBUS_MODE_SLAVE)
    {
        for (size_t i = 0; i < len; i++)
            mb_port_rx_byte(data[i]);
    }
    else if (current_mode == MODBUS_MODE_MASTER)
    {
        if (modbus_manager_port.timer_restart)
            modbus_manager_port.timer_restart();

        for (size_t i = 0; i < len; i++)
            mbm_rx_byte(data[i]);
    }

    /* Futuro: sniffer */
}

static void manager_timer_cb(void)
{
    if (current_mode == MODBUS_MODE_SLAVE)
    {
        mb_port_timer_expired();
    }
    else if (current_mode == MODBUS_MODE_MASTER)
    {
        mbm_frame_timeout();
    }
}

/* ============================================================ */

void modbus_manager_init(void)
{
    current_mode = MODBUS_MODE_NONE;
    sniffer_enabled = false;
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

    if (!modbus_manager_port.init(cfg->baudrate,
                                  cfg->databits,
                                  cfg->parity,
                                  cfg->stopbits))
    {
        return false;
    }

    modbus_manager_port.set_uart_rx_callback(manager_uart_rx_cb);
    modbus_manager_port.set_timer_callback(manager_timer_cb);

    /* ================= SLAVE ================= */

    if (mode == MODBUS_MODE_SLAVE)
    {
        eMBParity parity;

        switch (cfg->parity)
        {
            case 0: parity = MB_PAR_NONE; break;
            case 1: parity = MB_PAR_EVEN; break;
            case 2: parity = MB_PAR_ODD;  break;
            default: parity = MB_PAR_NONE;
        }

        if (eMBInit(MB_RTU,
                    0x01,
                    0,
                    cfg->baudrate,
                    parity) != MB_ENOERR)
        {
            modbus_manager_port.deinit();
            return false;
        }

        eMBEnable();
    }

    /* ================= MASTER ================= */

    if (mode == MODBUS_MODE_MASTER)
    {
        port_modbus_master_init();

        port_modbus_master_set_uart_send(
            modbus_manager_port_uart_send);

        port_modbus_master_set_timer_restart(
            modbus_manager_port.timer_restart);

        mbm_init(&modbus_master_port);
    }

    current_mode = mode;
    return true;
}

/* ============================================================ */

void modbus_manager_stop(void)
{
    if (current_mode == MODBUS_MODE_SLAVE)
        eMBDisable();

    modbus_manager_port.deinit();

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
