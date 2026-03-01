#ifndef MBM_H
#define MBM_H

#include <stdint.h>

/* ================= STATUS ================= */

typedef enum
{
    MBM_ERR_OK = 0,
    MBM_ERR_BUSY,
    MBM_ERR_TIMEOUT,
    MBM_ERR_CRC,
    MBM_ERR_INVALID,
    MBM_ERR_CANCELED
} mbm_status_t;

/* ================= CALLBACK ================= */

typedef void (*mbm_callback_t)(uint8_t *response,
                               uint16_t length,
                               mbm_status_t status);

/* ================= REQUEST ================= */

typedef struct
{
    uint8_t  slave_id;
    uint8_t  function;
    uint16_t address;
    uint16_t quantity;

    uint32_t job_id;

    const uint16_t *data;
    const uint8_t  *bit_data;

    uint32_t timeout_ms;
    uint8_t  retries;
    uint32_t period_ms;

    mbm_callback_t callback;

} mbm_request_t;

/* ================= PORT ================= */

typedef struct
{
    void     (*uart_send)(uint8_t *data, uint16_t len);
    uint16_t (*crc16)(uint8_t *data, uint16_t len);
    uint32_t (*get_time_ms)(void);
    void     (*timer_start_35char)(void);
} mbm_port_t;

/* ================= CORE API ================= */

void mbm_init(const mbm_port_t *port);
mbm_status_t mbm_enable(void);
mbm_status_t mbm_disable(void);
mbm_status_t mbm_deinit(void);

void mbm_poll(void);

void mbm_rx_byte(uint8_t byte);
void mbm_frame_timeout(void);

mbm_status_t mbm_add_request(const mbm_request_t *req);
mbm_status_t mbm_schedule(const mbm_request_t *req);

mbm_status_t mbm_cancel_job(uint32_t job_id);
mbm_status_t mbm_cancel_scheduled(uint32_t job_id);

void mbm_set_slave_timeout(uint8_t slave, uint32_t timeout_ms);

uint8_t mbm_is_enabled(void);
uint8_t mbm_is_busy(void);

/* ================= CACHE API ================= */

uint8_t mbm_cache_get(uint8_t slave, uint16_t reg, uint16_t *value);
uint8_t mbm_cache_get_coil(uint8_t slave, uint16_t addr, uint8_t *value);
uint8_t mbm_cache_get_discrete(uint8_t slave, uint16_t addr, uint8_t *value);

#endif
