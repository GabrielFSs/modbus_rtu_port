#include "modbus_map.h"
#include <string.h>

/* ===== MAPAS ===== */
bool     mb_coils[MB_COILS_N];
bool     mb_discrete_inputs[MB_DISCRETE_N];
uint16_t mb_input_regs[MB_INPUT_REGS_N];
uint16_t mb_holding_regs[MB_HOLDING_REGS_N];

void modbus_map_init(void)
{
    memset(mb_coils, 0, sizeof(mb_coils));
    memset(mb_discrete_inputs, 0, sizeof(mb_discrete_inputs));
    memset(mb_input_regs, 0, sizeof(mb_input_regs));
    memset(mb_holding_regs, 0, sizeof(mb_holding_regs));
}
