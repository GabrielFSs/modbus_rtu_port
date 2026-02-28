#ifndef MBM_RTU_H
#define MBM_RTU_H

#include "mbm.h"

#define MBM_MAX_FRAME 256

void mbm_rtu_init(const mbm_port_t *port);
void mbm_rtu_start_tx(const mbm_request_t *req);

void mbm_rtu_rx_byte(uint8_t byte);
void mbm_rtu_frame_timeout(void);

uint8_t mbm_rtu_frame_ready(void);
uint8_t* mbm_rtu_get_buffer(void);
uint16_t mbm_rtu_get_length(void);
void mbm_rtu_clear(void);

#endif
