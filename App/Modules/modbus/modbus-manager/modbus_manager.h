#ifndef MODBUS_MANAGER_H
#define MODBUS_MANAGER_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    MODBUS_MODE_NONE = 0,
    MODBUS_MODE_SLAVE,
    MODBUS_MODE_MASTER
} modbus_mode_t;

typedef struct
{
    uint32_t baudrate;
    uint8_t  databits;
    uint8_t  parity;
    uint8_t  stopbits;
    uint8_t  uart_index;  /* 0=UART1, 1=UART2, 2=UART3 (RS485 típico) */
} modbus_serial_cfg_t;

void modbus_manager_init(void);

bool modbus_manager_start(modbus_mode_t mode,
                          const modbus_serial_cfg_t *cfg);

void modbus_manager_stop(void);
void modbus_manager_poll(void);

void modbus_manager_enable_sniffer(bool enable);
modbus_mode_t modbus_manager_get_mode(void);

/** Aplica nova config serial e reinicia o barramento no mesmo modo (para GUI/runtime). */
bool modbus_manager_apply_serial_cfg(const modbus_serial_cfg_t *cfg);

/** Define o ID do slave (usado no próximo start em modo SLAVE). */
void modbus_manager_set_slave_id(uint8_t slave_id);

/** Adiciona request ao master (só tem efeito se o modo atual for MASTER). Retorna true se aceito. */
bool modbus_manager_add_request(const void *req);

#endif
