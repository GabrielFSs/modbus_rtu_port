#ifndef MBM_PORT_H
#define MBM_PORT_H

#include "mbm.h"

typedef struct
{
    uint32_t baudrate;
    uint8_t  databits;
    uint8_t  parity;
    uint8_t  stopbits;
    int      uart;
} mbm_serial_cfg_t;

mbm_status_t mbm_port_open(const mbm_serial_cfg_t *cfg);
mbm_status_t mbm_port_close(void);

#endif
