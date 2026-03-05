#ifndef PORT_MODBUS_MANAGER_H
#define PORT_MODBUS_MANAGER_H

#include <stdbool.h>
#include <stdint.h>
#include "modbus_manager.h"
#include <stddef.h>

typedef void (*mbm_uart_rx_cb_t)(const uint8_t *data, size_t len);
typedef void (*mbm_timer_timeout_cb_t)(void);

typedef struct
{
    bool (*init)(const modbus_serial_cfg_t *cfg);
    void (*deinit)(void);

    void (*set_uart_rx_callback)(mbm_uart_rx_cb_t cb);
    void (*set_timer_callback)(mbm_timer_timeout_cb_t cb);

    void (*timer_restart)(void);

    /* Para modo Master (mbm): envio UART, CRC16 Modbus, tempo em ms */
    void     (*uart_send)(uint8_t *data, uint16_t len);
    uint16_t (*crc16)(uint8_t *data, uint16_t len);
    uint32_t (*get_time_ms)(void);

} modbus_manager_port_t;

extern const modbus_manager_port_t modbus_manager_port;

#endif
