#ifndef _MB_SNIFFER_H_
#define _MB_SNIFFER_H_

#include <stdint.h>
#include "hal_rtc.h"

#define SNIFFER_MAX_FRAME   256
#define SNIFFER_QUEUE_SIZE  32

typedef enum
{
    SNIFFER_DIR_RX,
    SNIFFER_DIR_TX
} sniffer_direction_t;

typedef struct
{
    sniffer_direction_t dir;
    uint16_t length;
    uint8_t  data[SNIFFER_MAX_FRAME];
    hal_rtc_timestamp_t ts;
} sniffer_frame_t;

void mb_sniffer_init(void);
void mb_sniffer_enable(uint8_t en);

void mb_sniffer_rx_byte(uint8_t byte);
void mb_sniffer_rx_timeout(void);

void mb_sniffer_tx_store(const uint8_t *data, uint16_t len);
void mb_sniffer_tx_confirm(void);

void mb_sniffer_process(void);

/** Últimos N frames em RAM para a UI (sem ler SD). out[0] = mais recente. */
#define SNIFFER_LAST_FRAMES_N  10
void mb_sniffer_get_last_frames(sniffer_frame_t *out, uint8_t max, uint8_t *count);

#endif
