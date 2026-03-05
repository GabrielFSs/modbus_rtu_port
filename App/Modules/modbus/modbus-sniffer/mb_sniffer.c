#include "mb_sniffer.h"
#include "hal_storage.h"
#include <string.h>
#include <stdio.h>

/* ================= PRIVATE STATE ================= */

static uint8_t enabled = 0;
static const char *log_path = "modbus.log";

/* RX buffer */
static uint8_t rx_temp[SNIFFER_MAX_FRAME];
static volatile uint16_t rx_index = 0;

/* TX pending */
static uint8_t tx_temp[SNIFFER_MAX_FRAME];
static uint16_t tx_len_pending = 0;
static uint8_t tx_waiting_confirm = 0;

/* Queue */
static sniffer_frame_t queue[SNIFFER_QUEUE_SIZE];
static volatile uint8_t head = 0;
static volatile uint8_t tail = 0;

/* Últimos N frames para UI (ring, last_frames_head = mais recente) */
static sniffer_frame_t last_frames[SNIFFER_LAST_FRAMES_N];
static uint8_t last_frames_head = 0;
static uint8_t last_frames_count = 0;

static void push_last_frame(const sniffer_frame_t *f)
{
    last_frames[last_frames_head] = *f;
    last_frames_head = (last_frames_head + 1) % SNIFFER_LAST_FRAMES_N;
    if (last_frames_count < SNIFFER_LAST_FRAMES_N)
        last_frames_count++;
}

/* ================= INIT ================= */

void mb_sniffer_init(void)
{
    enabled = 0;
    rx_index = 0;
    tx_waiting_confirm = 0;
    head = 0;
    tail = 0;
    last_frames_head = 0;
    last_frames_count = 0;
}

/* ================= ENABLE ================= */

void mb_sniffer_enable(uint8_t en)
{
    enabled = en;
}

/* ================= RX BYTE ================= */

void mb_sniffer_rx_byte(uint8_t byte)
{
    if (!enabled)
        return;

    if (rx_index < SNIFFER_MAX_FRAME)
        rx_temp[rx_index++] = byte;
}

/* ================= RX TIMEOUT ================= */

void mb_sniffer_rx_timeout(void)
{
    if (!enabled || rx_index == 0)
        return;

    uint8_t next = (head + 1) % SNIFFER_QUEUE_SIZE;
    if (next == tail)
        return; // fila cheia

    sniffer_frame_t *f = &queue[head];

    f->dir = SNIFFER_DIR_RX;
    f->length = rx_index;
    memcpy(f->data, rx_temp, rx_index);

    hal_rtc_get_timestamp(&f->ts);
    push_last_frame(f);

    head = next;
    rx_index = 0;
}

/* ================= TX STORE ================= */

void mb_sniffer_tx_store(const uint8_t *data, uint16_t len)
{
    if (!enabled)
        return;

    if (len > SNIFFER_MAX_FRAME)
        len = SNIFFER_MAX_FRAME;

    memcpy(tx_temp, data, len);
    tx_len_pending = len;
    tx_waiting_confirm = 1;
}

/* ================= TX CONFIRM ================= */

void mb_sniffer_tx_confirm(void)
{
    if (!enabled || !tx_waiting_confirm)
        return;

    uint8_t next = (head + 1) % SNIFFER_QUEUE_SIZE;
    if (next == tail)
        return;

    sniffer_frame_t *f = &queue[head];

    f->dir = SNIFFER_DIR_TX;
    f->length = tx_len_pending;
    memcpy(f->data, tx_temp, tx_len_pending);

    hal_rtc_get_timestamp(&f->ts);
    push_last_frame(f);

    head = next;
    tx_waiting_confirm = 0;
}

/* ================= PROCESS ================= */

void mb_sniffer_process(void)
{
    if (!enabled)
        return;

    while (tail != head)
    {
        sniffer_frame_t *f = &queue[tail];

        char line[600];
        int offset = 0;

        offset += snprintf(line + offset,
                           sizeof(line) - offset,
                           "%04u-%02u-%02u %02u:%02u:%02u.%03u | %s | ",
                           f->ts.year,
                           f->ts.month,
                           f->ts.day,
                           f->ts.hour,
                           f->ts.min,
                           f->ts.sec,
                           f->ts.ms,
                           (f->dir == SNIFFER_DIR_RX) ? "RX" : "TX");

        for (uint16_t i = 0; i < f->length; i++)
        {
            offset += snprintf(line + offset,
                               sizeof(line) - offset,
                               "%02X ",
                               f->data[i]);
        }

        offset += snprintf(line + offset,
                           sizeof(line) - offset,
                           "\r\n");

        hal_storage_append_file(log_path,
                                (uint8_t*)line,
                                offset);

        tail = (tail + 1) % SNIFFER_QUEUE_SIZE;
    }
}

/* ================= GET LAST FRAMES (UI) ================= */

void mb_sniffer_get_last_frames(sniffer_frame_t *out, uint8_t max, uint8_t *count)
{
    if (!out || !count)
        return;

    *count = 0;
    if (max == 0 || last_frames_count == 0)
        return;

    uint8_t n = (max < last_frames_count) ? max : last_frames_count;
    uint8_t idx = (last_frames_head + SNIFFER_LAST_FRAMES_N - 1) % SNIFFER_LAST_FRAMES_N;

    for (uint8_t i = 0; i < n; i++)
    {
        out[i] = last_frames[idx];
        idx = (idx + SNIFFER_LAST_FRAMES_N - 1) % SNIFFER_LAST_FRAMES_N;
    }
    *count = n;
}
