#include "mbm_rtu.h"
#include <string.h>

static const mbm_port_t *port = NULL;

static uint8_t  rx_buffer[MBM_MAX_FRAME];
static uint16_t rx_index = 0;
static uint8_t  frame_ready = 0;

/* ============================================================ */
/* INIT                                                         */
/* ============================================================ */

void mbm_rtu_init(const mbm_port_t *p)
{
    port = p;
    rx_index = 0;
    frame_ready = 0;
}

/* ============================================================ */
/* BUILD REQUEST (COPIADO COMPLETO DO mb_master.c ORIGINAL)    */
/* ============================================================ */

static uint16_t build_request(uint8_t *frame,
                              const mbm_request_t *req)
{
    uint16_t len = 0;

    frame[len++] = req->slave_id;
    frame[len++] = req->function;

    switch (req->function)
    {
        /* READ COILS / DISCRETE / HOLDING / INPUT */
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
            frame[len++] = req->address >> 8;
            frame[len++] = req->address & 0xFF;
            frame[len++] = req->quantity >> 8;
            frame[len++] = req->quantity & 0xFF;
            break;

        /* WRITE SINGLE COIL */
        case 0x05:
            frame[len++] = req->address >> 8;
            frame[len++] = req->address & 0xFF;
            frame[len++] = req->quantity ? 0xFF : 0x00;
            frame[len++] = 0x00;
            break;

        /* WRITE SINGLE REGISTER */
        case 0x06:
            frame[len++] = req->address >> 8;
            frame[len++] = req->address & 0xFF;
            frame[len++] = req->quantity >> 8;
            frame[len++] = req->quantity & 0xFF;
            break;

        /* WRITE MULTIPLE COILS */
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

        /* WRITE MULTIPLE REGISTERS */
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

/* ============================================================ */
/* START TX                                                     */
/* ============================================================ */

void mbm_rtu_start_tx(const mbm_request_t *req)
{
    if (!port || !req)
        return;

    static uint8_t frame[MBM_MAX_FRAME];
    uint16_t len = build_request(frame, req);

    if (len == 0)
        return;

    port->uart_send(frame, len);

    rx_index = 0;
    frame_ready = 0;
}

/* ============================================================ */
/* RX BYTE                                                      */
/* ============================================================ */

void mbm_rtu_rx_byte(uint8_t byte)
{
    if (rx_index < MBM_MAX_FRAME)
        rx_buffer[rx_index++] = byte;

    if (port && port->timer_start_35char)
        port->timer_start_35char();
}

/* ============================================================ */
/* FRAME TIMEOUT (3.5 CHAR)                                     */
/* ============================================================ */

void mbm_rtu_frame_timeout(void)
{
    if (rx_index > 0)
        frame_ready = 1;
}

/* ============================================================ */
/* ACCESSORS                                                    */
/* ============================================================ */

uint8_t mbm_rtu_frame_ready(void)
{
    return frame_ready;
}

uint8_t* mbm_rtu_get_buffer(void)
{
    return rx_buffer;
}

uint16_t mbm_rtu_get_length(void)
{
    return rx_index;
}

void mbm_rtu_clear(void)
{
    frame_ready = 0;
    rx_index = 0;
}
