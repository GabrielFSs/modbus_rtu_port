#ifndef MB_MASTER_H
#define MB_MASTER_H

#include <stdint.h>

/* ============================================================
 * STATUS
 * ============================================================ */

typedef enum
{
    MBM_ERR_OK = 0,
    MBM_ERR_BUSY,
    MBM_ERR_TIMEOUT,
    MBM_ERR_CRC,
    MBM_ERR_INVALID

} mbm_status_t;

/* ============================================================
 * CALLBACK
 * ============================================================ */

typedef void (*mbm_callback_t)(uint8_t *response,
                               uint16_t length,
                               mbm_status_t status);

/* ============================================================
 * REQUEST
 * ============================================================ */

typedef struct
{
    uint8_t  slave_id;
    uint8_t  function;
    uint16_t address;
    uint16_t quantity;

    uint32_t timeout_ms;
    uint8_t  retries;

    uint32_t period_ms;   /* 0 = one-shot */

    mbm_callback_t callback;

} mbm_request_t;

/* ============================================================
 * PORT INTERFACE (Hardware abstraction)
 * ============================================================ */

typedef struct
{
    void     (*uart_send)(uint8_t *data, uint16_t len);
    uint16_t (*crc16)(uint8_t *data, uint16_t len);
    uint32_t (*get_time_ms)(void);
    void     (*timer_start_35char)(void);

} mbm_port_t;

/* ============================================================
 * API
 * ============================================================ */

void mbm_init(const mbm_port_t *port);

mbm_status_t mbm_add_request(const mbm_request_t *req);
mbm_status_t mbm_schedule(const mbm_request_t *req);

void mbm_poll(void);

/* RX interrupt entry */
void mbm_rx_byte(uint8_t byte);

/* 3.5 char timeout entry */
void mbm_frame_timeout(void);

/* cache */
uint8_t mbm_cache_get(uint8_t slave,
                      uint16_t reg,
                      uint16_t *value);

#endif
