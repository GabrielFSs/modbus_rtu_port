/*
 * FreeModbus: vMBPortClose - fecha serial e timers do port.
 * Habilitado quando MB_PORT_HAS_CLOSE == 1.
 */

#include "mbport.h"

void vMBPortClose(void)
{
    xMBPortSerialClose();
    xMBPortTimersClose();
}
