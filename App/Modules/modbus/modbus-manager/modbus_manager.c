#include "modbus_manager.h"
#include "port_modbus_manager.h"

/* FreeModbus (Slave) */
#include "mb.h"

/* Master */
#include "mbm.h"

/* Sniffer */
#include "mb_sniffer.h"

/* ============================================================ */

static modbus_mode_t current_mode = MODBUS_MODE_NONE;
static bool sniffer_enabled = false;

/* Port do mbm (master): preenchido a partir de modbus_manager_port */
static mbm_port_t master_port;

/* ============================================================
 * Timer físico controlado pelo manager (apenas modo Master)
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
 * CALLBACKS DO PORT (HAL → Manager) — usados apenas em modo Master
 * ============================================================ */

static void manager_uart_rx_cb(const uint8_t *data, size_t len)
{
    if (!data || len == 0)
        return;

    if (modbus_manager_port.timer_restart)
        modbus_manager_port.timer_restart();

    for (size_t i = 0; i < len; i++)
    {
        mb_sniffer_rx_byte(data[i]);
        mbm_rx_byte(data[i]);
    }
}

static void manager_timer_cb(void)
{
    mb_sniffer_rx_timeout();
    mbm_frame_timeout();
}

static void manager_uart_send(uint8_t *data, uint16_t len)
{
    mb_sniffer_tx_store(data, len);
    modbus_manager_port.uart_send(data, len);
    mb_sniffer_tx_confirm();
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

        uint8_t sid = (cfg->slave_id >= 1 && cfg->slave_id <= 247)
                     ? cfg->slave_id : 1;

        /* ucPort: 0=UART1, 1=UART2, 2=UART3. uart_dev 0=RS232(UART2)→1, 1=RS485(UART3)→2 */
        UCHAR ucPort = (cfg->uart_dev == 0) ? 1u : 2u;

        if (eMBInit(MB_RTU,
                    sid,
                    ucPort,
                    cfg->baudrate,
                    parity,
                    cfg->stopbits) != MB_ENOERR)
        {
            return false;
        }

        eMBEnable();
        current_mode = mode;
        return true;
    }

    /* ================= MASTER ================= */

    if (!modbus_manager_port.init(cfg))
        return false;

    modbus_manager_port.set_uart_rx_callback(manager_uart_rx_cb);
    modbus_manager_port.set_timer_callback(manager_timer_cb);

    master_port.uart_send          = manager_uart_send;
    master_port.crc16              = modbus_manager_port.crc16;
    master_port.get_time_ms        = modbus_manager_port.get_time_ms;
    master_port.timer_start_35char = modbus_manager_port.timer_restart;

    mbm_init(&master_port);

    if (mbm_enable() != MBM_ERR_OK)
    {
        modbus_manager_port.deinit();
        return false;
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
        mbm_deinit();
        modbus_manager_port.deinit();
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
