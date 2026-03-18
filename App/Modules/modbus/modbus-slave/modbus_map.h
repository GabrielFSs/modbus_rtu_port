#ifndef MODBUS_MAP_H
#define MODBUS_MAP_H

#include "mb.h"

/* ============================================================
 * CONFIGURAÇÃO DOS MAPAS
 * ============================================================ */

#define MB_HOLDING_REGS_START   1
#define MB_HOLDING_REGS_N       512

#define MB_INPUT_REGS_START     1
#define MB_INPUT_REGS_N         512

#define MB_COILS_START          1
#define MB_COILS_N              512

#define MB_DISCRETE_START       1
#define MB_DISCRETE_N           16


/* ============================================================
 * DECLARAÇÃO GLOBAL (extern)
 * ============================================================ */

extern UCHAR  mb_coils[MB_COILS_N];
extern UCHAR  mb_discrete_inputs[MB_DISCRETE_N];
extern USHORT mb_input_regs[MB_INPUT_REGS_N];
extern USHORT mb_holding_regs[MB_HOLDING_REGS_N];

void modbus_map_init(void);

#endif
