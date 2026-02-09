#include "modbus_slave_cb.h"
#include "modbus_map.h"

/* ================= HOLDING REGISTERS ================= */
eMBErrorCode eMBRegHoldingCB(
    UCHAR *pucRegBuffer,
    USHORT usAddress,
    USHORT usNRegs,
    eMBRegisterMode eMode
)
{
    USHORT i;
    USHORT index = usAddress - MB_HOLDING_REGS_START;

    if ((index + usNRegs) > MB_HOLDING_REGS_N)
        return MB_ENOREG;

    if (eMode == MB_REG_READ)
    {
        for (i = 0; i < usNRegs; i++)
        {
            *pucRegBuffer++ = mb_holding_regs[index + i] >> 8;
            *pucRegBuffer++ = mb_holding_regs[index + i] & 0xFF;
        }
    }
    else
    {
        for (i = 0; i < usNRegs; i++)
        {
            mb_holding_regs[index + i] =
                (*pucRegBuffer << 8) | *(pucRegBuffer + 1);
            pucRegBuffer += 2;
        }
    }

    return MB_ENOERR;
}

/* ================= INPUT REGISTERS ================= */
eMBErrorCode eMBRegInputCB(
    UCHAR *pucRegBuffer,
    USHORT usAddress,
    USHORT usNRegs
)
{
    USHORT i;
    USHORT index = usAddress - MB_INPUT_REGS_START;

    if ((index + usNRegs) > MB_INPUT_REGS_N)
        return MB_ENOREG;

    for (i = 0; i < usNRegs; i++)
    {
        *pucRegBuffer++ = mb_input_regs[index + i] >> 8;
        *pucRegBuffer++ = mb_input_regs[index + i] & 0xFF;
    }

    return MB_ENOERR;
}

/* ================= COILS ================= */
eMBErrorCode eMBRegCoilsCB(
    UCHAR *pucRegBuffer,
    USHORT usAddress,
    USHORT usNCoils,
    eMBRegisterMode eMode
)
{
    USHORT i;
    USHORT index = usAddress - MB_COILS_START;

    if ((index + usNCoils) > MB_COILS_N)
        return MB_ENOREG;

    if (eMode == MB_REG_READ)
    {
        for (i = 0; i < usNCoils; i++)
        {
            pucRegBuffer[i] = mb_coils[index + i] ? 1 : 0;
        }
    }
    else
    {
        for (i = 0; i < usNCoils; i++)
        {
            mb_coils[index + i] = pucRegBuffer[i] ? true : false;
        }
    }

    return MB_ENOERR;
}

/* ================= DISCRETE INPUTS ================= */
eMBErrorCode eMBRegDiscreteCB(
    UCHAR *pucRegBuffer,
    USHORT usAddress,
    USHORT usNDiscrete
)
{
    USHORT i;
    USHORT index = usAddress - MB_DISCRETE_START;

    if ((index + usNDiscrete) > MB_DISCRETE_N)
        return MB_ENOREG;

    for (i = 0; i < usNDiscrete; i++)
    {
        pucRegBuffer[i] = mb_discrete_inputs[index + i] ? 1 : 0;
    }

    return MB_ENOERR;
}
