#include "mb_sniffer.h"
#include "hal_storage.h"
#include "hal_rtc.h"
#include <string.h>
#include <stdio.h>

/* ================= PRIVATE STATE ================= */

static uint8_t enabled = 0;
static const char *log_path = "Debug.txt";

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

/* ================= INIT ================= */

void mb_sniffer_init(void)
{
    enabled = 0;
    rx_index = 0;
    tx_waiting_confirm = 0;
    head = 0;
    tail = 0;
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
