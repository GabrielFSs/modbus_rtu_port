#ifndef _PORT_H
#define _PORT_H

#include <assert.h>
#include <stdint.h>

/* Sniffer TX: callback quando o timer 3.5T após TX expira (frame completo) */
typedef void (*mb_port_tx_complete_cb_t)(void);
void mb_port_timer_set_tx_complete_cb(mb_port_tx_complete_cb_t cb);
void mb_port_timer_restart_tx(void);

#define INLINE inline

#define ENTER_CRITICAL_SECTION()
#define EXIT_CRITICAL_SECTION()

typedef uint8_t BOOL;
typedef unsigned char UCHAR;
typedef char CHAR;
typedef uint16_t USHORT;
typedef uint32_t ULONG;

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

#endif
