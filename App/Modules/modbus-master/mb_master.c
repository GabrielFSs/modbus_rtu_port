#include "mb_master.h"
#include <string.h>

/* ============================================================ */

#define MBM_MAX_QUEUE          16
#define MBM_MAX_FRAME          256
#define MBM_MAX_SLAVES         16
#define MBM_MAX_CACHE_ITEMS    16
#define MBM_MAX_SCHEDULED      16

/* ============================================================ */

typedef enum
{
    MBM_STATE_IDLE = 0,
    MBM_STATE_WAIT_RESPONSE

} mbm_state_t;

/* ============================================================ */

typedef struct
{
    uint16_t reg;
    uint16_t value;

} mbm_cache_item_t;

typedef struct
{
    uint8_t slave_id;
    mbm_cache_item_t items[MBM_MAX_CACHE_ITEMS];
    uint8_t count;

} mbm_slave_cache_t;

typedef struct
{
    mbm_request_t req;
    uint32_t next_run;
    uint8_t active;

} mbm_scheduled_job_t;

/* ============================================================ */
/* STATIC STATE */
/* ============================================================ */

static const mbm_port_t *port;

static mbm_state_t state = MBM_STATE_IDLE;

/* Queue */
static mbm_request_t job_queue[MBM_MAX_QUEUE];
static uint8_t queue_count = 0;
static mbm_request_t *current_job = NULL;

/* Scheduler */
static mbm_scheduled_job_t scheduled[MBM_MAX_SCHEDULED];
static uint8_t scheduled_count = 0;

/* RX */
static uint8_t rx_buffer[MBM_MAX_FRAME];
static uint16_t rx_index = 0;
static uint8_t frame_ready = 0;

/* Timing */
static uint32_t job_start_time = 0;
static uint8_t retry_count = 0;

/* Cache */
static mbm_slave_cache_t slave_cache[MBM_MAX_SLAVES];
static uint8_t slave_count = 0;

/* ============================================================ */
/* CACHE */
/* ============================================================ */

static mbm_slave_cache_t *get_slave_cache(uint8_t id)
{
    for (uint8_t i = 0; i < slave_count; i++)
        if (slave_cache[i].slave_id == id)
            return &slave_cache[i];

    if (slave_count < MBM_MAX_SLAVES)
    {
        slave_cache[slave_count].slave_id = id;
        slave_cache[slave_count].count = 0;
        return &slave_cache[slave_count++];
    }

    return NULL;
}

static void cache_update(uint8_t slave,
                         uint16_t start,
                         uint16_t qty,
                         uint8_t *data)
{
    mbm_slave_cache_t *sc = get_slave_cache(slave);
    if (!sc) return;

    for (uint16_t i = 0; i < qty; i++)
    {
        uint16_t reg = start + i;
        uint16_t value = (data[i*2] << 8) | data[i*2 + 1];

        uint8_t found = 0;

        for (uint8_t j = 0; j < sc->count; j++)
        {
            if (sc->items[j].reg == reg)
            {
                sc->items[j].value = value;
                found = 1;
                break;
            }
        }

        if (!found && sc->count < MBM_MAX_CACHE_ITEMS)
        {
            sc->items[sc->count].reg = reg;
            sc->items[sc->count].value = value;
            sc->count++;
        }
    }
}

/* ============================================================ */
/* FRAME BUILD */
/* ============================================================ */

static uint16_t build_request(uint8_t *frame,
                              mbm_request_t *req)
{
    frame[0] = req->slave_id;
    frame[1] = req->function;

    frame[2] = req->address >> 8;
    frame[3] = req->address & 0xFF;

    frame[4] = req->quantity >> 8;
    frame[5] = req->quantity & 0xFF;

    uint16_t crc = port->crc16(frame, 6);

    frame[6] = crc & 0xFF;
    frame[7] = crc >> 8;

    return 8;
}

/* ============================================================ */
/* RESPONSE PROCESS */
/* ============================================================ */

static void remove_current_job(void)
{
    if (queue_count == 0)
        return;

    memmove(&job_queue[0],
            &job_queue[1],
            sizeof(mbm_request_t) * (queue_count - 1));

    queue_count--;
}

static void process_response(void)
{
    if (!current_job)
        return;

    if (rx_index < 5)
        goto error;

    uint16_t crc_rx =
        rx_buffer[rx_index - 2] |
        (rx_buffer[rx_index - 1] << 8);

    uint16_t crc_calc =
        port->crc16(rx_buffer, rx_index - 2);

    if (crc_rx != crc_calc)
        goto error;

    if (rx_buffer[0] != current_job->slave_id)
        goto error;

    /* SUCCESS */

    if (current_job->function == 0x03)
    {
        cache_update(current_job->slave_id,
                     current_job->address,
                     current_job->quantity,
                     &rx_buffer[3]);
    }

    if (current_job->callback)
        current_job->callback(rx_buffer,
                              rx_index,
                              MBM_ERR_OK);

    remove_current_job();
    state = MBM_STATE_IDLE;
    return;

error:

    if (retry_count++ < current_job->retries)
    {
        state = MBM_STATE_IDLE;
        return;
    }

    if (current_job->callback)
        current_job->callback(NULL, 0, MBM_ERR_TIMEOUT);

    remove_current_job();
    state = MBM_STATE_IDLE;
}

/* ============================================================ */
/* PUBLIC */
/* ============================================================ */

void mbm_init(const mbm_port_t *p)
{
    port = p;
    state = MBM_STATE_IDLE;
}

mbm_status_t mbm_add_request(const mbm_request_t *req)
{
    if (queue_count >= MBM_MAX_QUEUE)
        return MBM_ERR_BUSY;

    job_queue[queue_count++] = *req;
    return MBM_ERR_OK;
}

mbm_status_t mbm_schedule(const mbm_request_t *req)
{
    if (scheduled_count >= MBM_MAX_SCHEDULED)
        return MBM_ERR_BUSY;

    scheduled[scheduled_count].req = *req;
    scheduled[scheduled_count].next_run = port->get_time_ms();
    scheduled[scheduled_count].active = 1;

    scheduled_count++;
    return MBM_ERR_OK;
}

void mbm_rx_byte(uint8_t byte)
{
    if (rx_index < MBM_MAX_FRAME)
        rx_buffer[rx_index++] = byte;

    port->timer_start_35char();
}

void mbm_frame_timeout(void)
{
    if (rx_index > 0)
        frame_ready = 1;
}

uint8_t mbm_cache_get(uint8_t slave,
                      uint16_t reg,
                      uint16_t *value)
{
    mbm_slave_cache_t *sc = get_slave_cache(slave);
    if (!sc) return 0;

    for (uint8_t i = 0; i < sc->count; i++)
    {
        if (sc->items[i].reg == reg)
        {
            *value = sc->items[i].value;
            return 1;
        }
    }

    return 0;
}

/* ============================================================ */
/* MAIN POLL */
/* ============================================================ */

void mbm_poll(void)
{
    uint32_t now = port->get_time_ms();

    /* Scheduler */

    for (uint8_t i = 0; i < scheduled_count; i++)
    {
        if (!scheduled[i].active)
            continue;

        if (scheduled[i].req.period_ms == 0)
        {
            if (mbm_add_request(&scheduled[i].req) == MBM_ERR_OK)
                scheduled[i].active = 0;
        }
        else if (now >= scheduled[i].next_run)
        {
            if (mbm_add_request(&scheduled[i].req) == MBM_ERR_OK)
                scheduled[i].next_run = now + scheduled[i].req.period_ms;
        }
    }

    /* State machine */

    switch (state)
    {
        case MBM_STATE_IDLE:

            if (queue_count == 0)
                break;

            current_job = &job_queue[0];

            {
                uint8_t frame[MBM_MAX_FRAME];
                uint16_t len = build_request(frame, current_job);
                port->uart_send(frame, len);
            }

            rx_index = 0;
            frame_ready = 0;
            retry_count = 0;
            job_start_time = now;

            state = MBM_STATE_WAIT_RESPONSE;
            break;

        case MBM_STATE_WAIT_RESPONSE:

            if (frame_ready)
            {
                frame_ready = 0;
                process_response();
            }
            else if ((now - job_start_time) > current_job->timeout_ms)
            {
                process_response();
            }

            break;
    }
}
