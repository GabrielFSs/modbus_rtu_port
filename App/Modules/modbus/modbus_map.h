#ifndef MODBUS_MAP_H
#define MODBUS_MAP_H

#include <stdint.h>
#include <stdbool.h>

/* ===== TAMANHOS ===== */
#define MB_COILS_N           128
#define MB_DISCRETE_N        128
#define MB_INPUT_REGS_N      128
#define MB_HOLDING_REGS_N    256

/* ===== ENDEREÇOS BASE (0-based INTERNAMENTE) ===== */
#define MB_COILS_START           0
#define MB_DISCRETE_START        0
#define MB_INPUT_REGS_START      0
#define MB_HOLDING_REGS_START    0

/* ===== MAPAS ===== */
extern bool     mb_coils[MB_COILS_N];
extern bool     mb_discrete_inputs[MB_DISCRETE_N];
extern uint16_t mb_input_regs[MB_INPUT_REGS_N];
extern uint16_t mb_holding_regs[MB_HOLDING_REGS_N];

/* ===== INIT ===== */
void modbus_map_init(void);

#endif
