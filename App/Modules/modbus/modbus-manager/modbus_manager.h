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
} modbus_serial_cfg_t;

void modbus_manager_init(void);

bool modbus_manager_start(modbus_mode_t mode,
                          const modbus_serial_cfg_t *cfg);

void modbus_manager_stop(void);
void modbus_manager_poll(void);

#endif
