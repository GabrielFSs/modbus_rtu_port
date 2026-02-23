#include "modbus_map.h"

bool     mb_coils[MB_COILS_N];
bool     mb_discrete_inputs[MB_DISCRETE_N];
uint16_t mb_input_regs[MB_INPUT_REGS_N];
uint16_t mb_holding_regs[MB_HOLDING_REGS_N];

void modbus_map_init(void)
{
    for (int i = 0; i < MB_COILS_N; i++)
        mb_coils[i] = false;

    for (int i = 0; i < MB_DISCRETE_N; i++)
        mb_discrete_inputs[i] = false;

    for (int i = 0; i < MB_INPUT_REGS_N; i++)
        mb_input_regs[i] = 0;

    for (int i = 0; i < MB_HOLDING_REGS_N; i++)
        mb_holding_regs[i] = 0;
}
