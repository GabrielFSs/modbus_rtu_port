#ifndef _PORT_H
#define _PORT_H

#include <assert.h>
#include <stdint.h>

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
