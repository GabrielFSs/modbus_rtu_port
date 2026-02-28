#include "modbus_map.h"

/* ============================================================
 * DEFINIÇÃO REAL DAS VARIÁVEIS
 * ============================================================ */

UCHAR  mb_coils[MB_COILS_N];
UCHAR  mb_discrete_inputs[MB_DISCRETE_N];
USHORT mb_input_regs[MB_INPUT_REGS_N];
USHORT mb_holding_regs[MB_HOLDING_REGS_N];


/* ============================================================
 * INIT
 * ============================================================ */

void modbus_map_init(void)
{
    for (int i = 0; i < MB_COILS_N; i++)
        mb_coils[i] = 0;

    for (int i = 0; i < MB_DISCRETE_N; i++)
        mb_discrete_inputs[i] = 0;

    for (int i = 0; i < MB_INPUT_REGS_N; i++)
        mb_input_regs[i] = 0;

    for (int i = 0; i < MB_HOLDING_REGS_N; i++)
        mb_holding_regs[i] = 0;
}
