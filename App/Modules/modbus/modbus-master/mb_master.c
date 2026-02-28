#include "mb_master.h"
#include <string.h>

/* ================= CONFIG ================= */

#define MBM_MAX_QUEUE                16
#define MBM_MAX_FRAME                256
#define MBM_MAX_SLAVES               16
#define MBM_MAX_CACHE_ITEMS          32
#define MBM_MAX_SCHEDULED            16
#define MBM_SLAVE_FAIL_LIMIT         3
#define MBM_SLAVE_BACKOFF_MS         5000
#define MBM_DEFAULT_TIMEOUT_MS       200
#define MBM_DEFAULT_RETRIES          2
#define MBM_MAX_COILS_PER_SLAVE      256
#define MBM_MAX_DISCRETE_PER_SLAVE   256

/* ================= STATE ================= */

typedef enum
{
    MBM_STATE_IDLE,
    MBM_STATE_WAIT_RESPONSE
} mbm_state_t;

static mbm_state_t state = MBM_STATE_IDLE;
static const mbm_port_t *port = NULL;

/* ================= OPERATION STATE ================= */

typedef enum
{
    MBM_OPER_UNINIT = 0,
    MBM_OPER_INITIALIZED,
    MBM_OPER_ENABLED,
    MBM_OPER_BUSY,
    MBM_OPER_DISABLED,
    MBM_OPER_ERROR
} mbm_oper_state_t;

static mbm_oper_state_t oper_state = MBM_OPER_UNINIT;

/* ================= CACHE STRUCTURES ================= */

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
} mbm_analog_cache_t;

typedef struct
{
    uint8_t slave_id;
    uint8_t coils[MBM_MAX_COILS_PER_SLAVE / 8];
    uint8_t discrete[MBM_MAX_DISCRETE_PER_SLAVE / 8];
} mbm_digital_cache_t;

typedef struct
{
    uint8_t slave_id;
    uint32_t timeout_ms;
    uint8_t fail_count;
    uint8_t online;
    uint32_t next_retry_time;
} mbm_slave_state_t;

typedef struct
{
    mbm_request_t req;
    uint32_t next_run;
    uint8_t active;
} mbm_scheduled_job_t;

/* ================= GLOBALS ================= */

static mbm_request_t job_queue[MBM_MAX_QUEUE];
static uint8_t queue_count = 0;
static mbm_request_t *current_job = NULL;

static mbm_scheduled_job_t scheduled[MBM_MAX_SCHEDULED];
static uint8_t scheduled_count = 0;

static uint8_t rx_buffer[MBM_MAX_FRAME];
static uint16_t rx_index = 0;
static uint8_t frame_ready = 0;

static uint32_t job_start_time = 0;
static uint8_t retry_count = 0;

static mbm_analog_cache_t analog_cache[MBM_MAX_SLAVES];
static uint8_t analog_count = 0;

static mbm_digital_cache_t digital_cache[MBM_MAX_SLAVES];
static uint8_t digital_count = 0;

static mbm_slave_state_t slave_state[MBM_MAX_SLAVES];
static uint8_t slave_state_count = 0;

/* ============================================================ */
/* CACHE HELPERS                                               */
/* ============================================================ */

static mbm_analog_cache_t* get_analog_cache(uint8_t id)
{
    for (uint8_t i = 0; i < analog_count; i++)
        if (analog_cache[i].slave_id == id)
            return &analog_cache[i];

    if (analog_count < MBM_MAX_SLAVES)
    {
        analog_cache[analog_count].slave_id = id;
        analog_cache[analog_count].count = 0;
        return &analog_cache[analog_count++];
    }

    return NULL;
}

static mbm_digital_cache_t* get_digital_cache(uint8_t id)
{
    for (uint8_t i = 0; i < digital_count; i++)
        if (digital_cache[i].slave_id == id)
            return &digital_cache[i];

    if (digital_count < MBM_MAX_SLAVES)
    {
        digital_cache[digital_count].slave_id = id;
        memset(digital_cache[digital_count].coils, 0, sizeof(digital_cache[digital_count].coils));
        memset(digital_cache[digital_count].discrete, 0, sizeof(digital_cache[digital_count].discrete));
        return &digital_cache[digital_count++];
    }

    return NULL;
}

static void cache_update_analog(uint8_t slave, uint16_t start, uint16_t qty, uint8_t *data)
{
    mbm_analog_cache_t *sc = get_analog_cache(slave);
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

static void cache_update_bits(uint8_t slave,
                              uint16_t start,
                              uint16_t qty,
                              uint8_t *data,
                              uint8_t is_coil)
{
    mbm_digital_cache_t *dc = get_digital_cache(slave);
    if (!dc) return;

    for (uint16_t i = 0; i < qty; i++)
    {
        uint8_t value = (data[i / 8] >> (i % 8)) & 0x01;
        uint16_t addr = start + i;
        uint16_t byte = addr / 8;
        uint8_t bit = addr % 8;

        if (is_coil)
        {
            if (value) dc->coils[byte] |= (1 << bit);
            else dc->coils[byte] &= ~(1 << bit);
        }
        else
        {
            if (value) dc->discrete[byte] |= (1 << bit);
            else dc->discrete[byte] &= ~(1 << bit);
        }
    }
}

/* ============================================================ */
/* CACHE PUBLIC API                                            */
/* ============================================================ */

uint8_t mbm_cache_get(uint8_t slave,
                      uint16_t reg,
                      uint16_t *value)
{
    for (uint8_t i = 0; i < analog_count; i++)
    {
        if (analog_cache[i].slave_id == slave)
        {
            for (uint8_t j = 0; j < analog_cache[i].count; j++)
            {
                if (analog_cache[i].items[j].reg == reg)
                {
                    *value = analog_cache[i].items[j].value;
                    return 1;
                }
            }
        }
    }
    return 0;
}

uint8_t mbm_cache_get_coil(uint8_t slave,
                           uint16_t address,
                           uint8_t *value)
{
    for (uint8_t i = 0; i < digital_count; i++)
    {
        if (digital_cache[i].slave_id == slave)
        {
            uint16_t byte = address / 8;
            uint8_t bit = address % 8;
            *value = (digital_cache[i].coils[byte] >> bit) & 0x01;
            return 1;
        }
    }
    return 0;
}

uint8_t mbm_cache_get_discrete(uint8_t slave,
                               uint16_t address,
                               uint8_t *value)
{
    for (uint8_t i = 0; i < digital_count; i++)
    {
        if (digital_cache[i].slave_id == slave)
        {
            uint16_t byte = address / 8;
            uint8_t bit = address % 8;
            *value = (digital_cache[i].discrete[byte] >> bit) & 0x01;
            return 1;
        }
    }
    return 0;
}

static mbm_slave_state_t* get_slave_state(uint8_t id)
{
    for (uint8_t i = 0; i < slave_state_count; i++)
        if (slave_state[i].slave_id == id)
            return &slave_state[i];

    if (slave_state_count < MBM_MAX_SLAVES)
    {
        mbm_slave_state_t *s = &slave_state[slave_state_count++];
        s->slave_id = id;
        s->timeout_ms = MBM_DEFAULT_TIMEOUT_MS;
        s->fail_count = 0;
        s->online = 1;
        s->next_retry_time = 0;
        return s;
    }
    return NULL;
}

static uint16_t build_request(uint8_t *frame, mbm_request_t *req)
{
    uint16_t len = 0;

    frame[len++] = req->slave_id;
    frame[len++] = req->function;

    switch (req->function)
    {
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
            frame[len++] = req->address >> 8;
            frame[len++] = req->address & 0xFF;
            frame[len++] = req->quantity >> 8;
            frame[len++] = req->quantity & 0xFF;
            break;

        case 0x05:
            frame[len++] = req->address >> 8;
            frame[len++] = req->address & 0xFF;
            frame[len++] = req->quantity ? 0xFF : 0x00;
            frame[len++] = 0x00;
            break;

        case 0x06:
            frame[len++] = req->address >> 8;
            frame[len++] = req->address & 0xFF;
            frame[len++] = req->quantity >> 8;
            frame[len++] = req->quantity & 0xFF;
            break;

        case 0x0F:
        {
            frame[len++] = req->address >> 8;
            frame[len++] = req->address & 0xFF;
            frame[len++] = req->quantity >> 8;
            frame[len++] = req->quantity & 0xFF;
            uint8_t byteCount = (req->quantity + 7) / 8;
            frame[len++] = byteCount;
            memcpy(&frame[len], req->bit_data, byteCount);
            len += byteCount;
        }
        break;

        case 0x10:
        {
            frame[len++] = req->address >> 8;
            frame[len++] = req->address & 0xFF;
            frame[len++] = req->quantity >> 8;
            frame[len++] = req->quantity & 0xFF;
            frame[len++] = req->quantity * 2;
            for (uint16_t i = 0; i < req->quantity; i++)
            {
                frame[len++] = req->data[i] >> 8;
                frame[len++] = req->data[i] & 0xFF;
            }
        }
        break;

        default:
            return 0;
    }

    uint16_t crc = port->crc16(frame, len);
    frame[len++] = crc & 0xFF;
    frame[len++] = crc >> 8;

    return len;
}


void mbm_init(const mbm_port_t *p)
{
    if (!p)
    {
        oper_state = MBM_OPER_ERROR;
        return;
    }

    port = p;

    /* Reset interno */
    queue_count = 0;
    scheduled_count = 0;
    rx_index = 0;
    frame_ready = 0;
    retry_count = 0;
    current_job = NULL;

    state = MBM_STATE_IDLE;

    oper_state = MBM_OPER_INITIALIZED;
}

mbm_status_t mbm_enable(void)
{
    if (oper_state != MBM_OPER_INITIALIZED &&
        oper_state != MBM_OPER_DISABLED)
        return MBM_ERR_INVALID;

    oper_state = MBM_OPER_ENABLED;
    return MBM_ERR_OK;
}

mbm_status_t mbm_disable(void)
{
    if (oper_state != MBM_OPER_ENABLED &&
        oper_state != MBM_OPER_BUSY)
        return MBM_ERR_INVALID;

    /* Abort job atual */
    current_job = NULL;
    rx_index = 0;
    frame_ready = 0;

    oper_state = MBM_OPER_DISABLED;
    state = MBM_STATE_IDLE;

    return MBM_ERR_OK;
}

mbm_status_t mbm_deinit(void)
{
    if (oper_state == MBM_OPER_UNINIT)
        return MBM_ERR_INVALID;

    /* Limpa tudo */
    queue_count = 0;
    scheduled_count = 0;
    rx_index = 0;
    frame_ready = 0;
    retry_count = 0;
    current_job = NULL;
    analog_count = 0;
    digital_count = 0;
    slave_state_count = 0;

    state = MBM_STATE_IDLE;
    oper_state = MBM_OPER_UNINIT;
    port = NULL;

    return MBM_ERR_OK;
}

void mbm_poll(void)
{
    if (oper_state != MBM_OPER_ENABLED &&
        oper_state != MBM_OPER_BUSY)
        return;

    if (!port)
        return;
    uint32_t now = port->get_time_ms();

    for (uint8_t i = 0; i < scheduled_count; i++)
    {
        if (!scheduled[i].active) continue;
        if (now >= scheduled[i].next_run)
        {
            if (mbm_add_request(&scheduled[i].req) == MBM_ERR_OK)
                scheduled[i].next_run = now + scheduled[i].req.period_ms;
        }
    }

    switch (state)
    {
        case MBM_STATE_IDLE:
            if (queue_count == 0) break;

            current_job = &job_queue[0];

            {
                mbm_slave_state_t *s = get_slave_state(current_job->slave_id);
                if (s && !s->online && now < s->next_retry_time)
                {
                    remove_current_job();
                    break;
                }
            }

            {
                uint8_t frame[MBM_MAX_FRAME];
                uint16_t len = build_request(frame, current_job);
                port->uart_send(frame, len);

                oper_state = MBM_OPER_BUSY;
            }

            rx_index = 0;
            frame_ready = 0;
            retry_count = 0;
            job_start_time = now;
            state = MBM_STATE_WAIT_RESPONSE;
            break;

        case MBM_STATE_WAIT_RESPONSE:
        {
            mbm_slave_state_t *s = get_slave_state(current_job->slave_id);
            uint32_t timeout = s ? s->timeout_ms : current_job->timeout_ms;

            if (frame_ready)
            {
                frame_ready = 0;
                process_response();
            }
            else if ((now - job_start_time) > timeout)
            {
                process_response();
            }
        }
        break;
    }
}

uint8_t mbm_is_enabled(void)
{
    return (oper_state == MBM_OPER_ENABLED);
}

uint8_t mbm_is_busy(void)
{
    return (oper_state == MBM_OPER_BUSY);
}

static void process_response(void)
{
    if (!current_job)
        return;

    mbm_slave_state_t *s = get_slave_state(current_job->slave_id);
    uint32_t now = port->get_time_ms();

    /* ================= BASIC LENGTH CHECK ================= */

    if (rx_index < 5)   /* slave + func + 1 data + crc */
        goto fail;

    /* ================= CRC CHECK ================= */

    uint16_t crc_rx =
        rx_buffer[rx_index - 2] |
        (rx_buffer[rx_index - 1] << 8);

    uint16_t crc_calc =
        port->crc16(rx_buffer, rx_index - 2);

    if (crc_rx != crc_calc)
        goto fail;

    /* ================= SLAVE ID ================= */

    if (rx_buffer[0] != current_job->slave_id)
        goto fail;

    /* ================= EXCEPTION ================= */

    if (rx_buffer[1] & 0x80)
    {
        /* Exception frame: slave + (func|0x80) + exception + crc */
        if (rx_index < 5)
            goto fail;

        if (current_job->callback)
            current_job->callback(rx_buffer,
                                  rx_index,
                                  MBM_ERR_INVALID);

        goto success_cleanup;
    }

    /* ================= FUNCTION MATCH ================= */

    if (rx_buffer[1] != current_job->function)
        goto fail;

    /* ================= FUNCTION-SPECIFIC VALIDATION ================= */

    switch (current_job->function)
    {
        /* ===== READ COILS ===== */
        case 0x01:
        case 0x02:
        {
            uint8_t byteCount = rx_buffer[2];
            uint8_t expectedBytes =
                (current_job->quantity + 7) / 8;

            if (byteCount != expectedBytes)
                goto fail;

            if (rx_index != (3 + byteCount + 2))
                goto fail;

            cache_update_bits(current_job->slave_id,
                              current_job->address,
                              current_job->quantity,
                              &rx_buffer[3],
                              current_job->function == 0x01);

            break;
        }

        /* ===== READ HOLDING / INPUT ===== */
        case 0x03:
        case 0x04:
        {
            uint8_t byteCount = rx_buffer[2];
            uint8_t expectedBytes =
                current_job->quantity * 2;

            if (byteCount != expectedBytes)
                goto fail;

            if (rx_index != (3 + byteCount + 2))
                goto fail;

            cache_update_analog(current_job->slave_id,
                                current_job->address,
                                current_job->quantity,
                                &rx_buffer[3]);

            break;
        }

        /* ===== WRITE SINGLE ===== */
        case 0x05:
        case 0x06:
        {
            /* Echo frame: slave func addr_hi addr_lo val_hi val_lo crc */
            if (rx_index != 8)
                goto fail;

            break;
        }

        /* ===== WRITE MULTIPLE ===== */
        case 0x0F:
        case 0x10:
        {
            /* Echo: slave func addr_hi addr_lo qty_hi qty_lo crc */
            if (rx_index != 8)
                goto fail;

            break;
        }

        default:
            goto fail;
    }

    /* ================= SUCCESS ================= */

    if (current_job->callback)
        current_job->callback(rx_buffer,
                              rx_index,
                              MBM_ERR_OK);

    if (s)
    {
        s->fail_count = 0;
        s->online = 1;
    }

    success_cleanup:

        retry_count = 0;
        remove_current_job();
        state = MBM_STATE_IDLE;
        oper_state = MBM_OPER_ENABLED;
        return;

	fail:

		/* ================= RETRY LOGIC ================= */

	if (retry_count++ < current_job->retries)
	{
	    state = MBM_STATE_IDLE;
	    oper_state = MBM_OPER_ENABLED;
	    return;
	}

    /* ================= SLAVE SUPERVISION ================= */

    if (s)
    {
        s->fail_count++;

        if (s->fail_count >= MBM_SLAVE_FAIL_LIMIT)
        {
            s->online = 0;
            s->next_retry_time =
                now + MBM_SLAVE_BACKOFF_MS;
        }
    }

    if (current_job->callback)
        current_job->callback(NULL, 0, MBM_ERR_TIMEOUT);

    retry_count = 0;
    remove_current_job();
    state = MBM_STATE_IDLE;
    oper_state = MBM_OPER_ENABLED;
}

void mbm_frame_timeout(void)
{
    if (rx_index > 0)
        frame_ready = 1;
}

void mbm_rx_byte(uint8_t byte)
{
    if (rx_index < MBM_MAX_FRAME)
        rx_buffer[rx_index++] = byte;

    if (port && port->timer_start_35char)
        port->timer_start_35char();
}

mbm_status_t mbm_add_request(const mbm_request_t *req)
{
    if (!req)
        return MBM_ERR_INVALID;

    if (queue_count >= MBM_MAX_QUEUE)
        return MBM_ERR_BUSY;

    job_queue[queue_count] = *req;

    if (job_queue[queue_count].timeout_ms == 0)
        job_queue[queue_count].timeout_ms = MBM_DEFAULT_TIMEOUT_MS;

    if (job_queue[queue_count].retries == 0)
        job_queue[queue_count].retries = MBM_DEFAULT_RETRIES;

    queue_count++;
    return MBM_ERR_OK;
}

mbm_status_t mbm_schedule(const mbm_request_t *req)
{
    if (!req)
        return MBM_ERR_INVALID;

    if (scheduled_count >= MBM_MAX_SCHEDULED)
        return MBM_ERR_BUSY;

    scheduled[scheduled_count].req = *req;
    scheduled[scheduled_count].next_run = port->get_time_ms();
    scheduled[scheduled_count].active = 1;

    scheduled_count++;
    return MBM_ERR_OK;
}

static void remove_current_job(void)
{
    if (queue_count == 0)
        return;

    memmove(&job_queue[0],
            &job_queue[1],
            sizeof(mbm_request_t) * (queue_count - 1));

    queue_count--;
    current_job = NULL;
}

mbm_status_t mbm_cancel_job(uint32_t job_id)
{
    /* ===== 1. JOB EM EXECUÇÃO ===== */
    if (current_job && current_job->job_id == job_id)
    {
        if (current_job->callback)
            current_job->callback(NULL, 0, MBM_ERR_CANCELED);

        retry_count = 0;
        rx_index = 0;
        frame_ready = 0;

        remove_current_job();
        state = MBM_STATE_IDLE;
        oper_state = MBM_OPER_ENABLED;

        return MBM_ERR_OK;
    }

    /* ===== 2. JOB NA FILA ===== */
    for (uint8_t i = 0; i < queue_count; i++)
    {
        if (job_queue[i].job_id == job_id)
        {
            if (job_queue[i].callback)
                job_queue[i].callback(NULL, 0, MBM_ERR_CANCELED);

            memmove(&job_queue[i],
                    &job_queue[i + 1],
                    sizeof(mbm_request_t) * (queue_count - i - 1));

            queue_count--;

            return MBM_ERR_OK;
        }
    }

    return MBM_ERR_INVALID;
}

mbm_status_t mbm_cancel_scheduled(uint32_t job_id)
{
    for (uint8_t i = 0; i < scheduled_count; i++)
    {
        if (scheduled[i].req.job_id == job_id &&
            scheduled[i].active)
        {
            scheduled[i].active = 0;

            if (scheduled[i].req.callback)
                scheduled[i].req.callback(NULL, 0, MBM_ERR_CANCELED);

            return MBM_ERR_OK;
        }
    }

    return MBM_ERR_INVALID;
}
