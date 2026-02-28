#include "mb.h"
#include "mbport.h"
#include "modbus_map.h"

/* ============================================================
 * HOLDING REGISTERS (FC03, FC06, FC16)
 * ============================================================ */

eMBErrorCode eMBRegHoldingCB(UCHAR *pucRegBuffer,
                             USHORT usAddress,
                             USHORT usNRegs,
                             eMBRegisterMode eMode)
{
    if ((usAddress < MB_HOLDING_REGS_START) ||
        ((usAddress + usNRegs) >
         (MB_HOLDING_REGS_START + MB_HOLDING_REGS_N)))
    {
        return MB_ENOREG;
    }

    USHORT index = usAddress - MB_HOLDING_REGS_START;

    if (eMode == MB_REG_READ)
    {
        while (usNRegs--)
        {
            *pucRegBuffer++ = (UCHAR)(mb_holding_regs[index] >> 8);
            *pucRegBuffer++ = (UCHAR)(mb_holding_regs[index] & 0xFF);
            index++;
        }
    }
    else /* WRITE */
    {
        while (usNRegs--)
        {
            mb_holding_regs[index] =
                ((USHORT)pucRegBuffer[0] << 8) |
                (USHORT)pucRegBuffer[1];

            pucRegBuffer += 2;
            index++;
        }
    }

    return MB_ENOERR;
}


/* ============================================================
 * INPUT REGISTERS (FC04)
 * ============================================================ */

eMBErrorCode eMBRegInputCB(UCHAR *pucRegBuffer,
                           USHORT usAddress,
                           USHORT usNRegs)
{
    if ((usAddress < MB_INPUT_REGS_START) ||
        ((usAddress + usNRegs) >
         (MB_INPUT_REGS_START + MB_INPUT_REGS_N)))
    {
        return MB_ENOREG;
    }

    USHORT index = usAddress - MB_INPUT_REGS_START;

    while (usNRegs--)
    {
        *pucRegBuffer++ = (UCHAR)(mb_input_regs[index] >> 8);
        *pucRegBuffer++ = (UCHAR)(mb_input_regs[index] & 0xFF);
        index++;
    }

    return MB_ENOERR;
}


/* ============================================================
 * COILS (FC01, FC05, FC15)
 * ============================================================ */

eMBErrorCode eMBRegCoilsCB(UCHAR *pucRegBuffer,
                           USHORT usAddress,
                           USHORT usNCoils,
                           eMBRegisterMode eMode)
{
    if ((usAddress < MB_COILS_START) ||
        ((usAddress + usNCoils) >
         (MB_COILS_START + MB_COILS_N)))
    {
        return MB_ENOREG;
    }

    USHORT index = usAddress - MB_COILS_START;

    if (eMode == MB_REG_READ)
    {
        for (USHORT i = 0; i < usNCoils; i++)
        {
            if (mb_coils[index + i])
                pucRegBuffer[i / 8] |=  (1 << (i % 8));
            else
                pucRegBuffer[i / 8] &= ~(1 << (i % 8));
        }
    }
    else /* WRITE */
    {
        for (USHORT i = 0; i < usNCoils; i++)
        {
            mb_coils[index + i] =
                (pucRegBuffer[i / 8] >> (i % 8)) & 0x01;
        }
    }

    return MB_ENOERR;
}


/* ============================================================
 * DISCRETE INPUTS (FC02)
 * ============================================================ */

eMBErrorCode eMBRegDiscreteCB(UCHAR *pucRegBuffer,
                              USHORT usAddress,
                              USHORT usNDiscrete)
{
    if ((usAddress < MB_DISCRETE_START) ||
        ((usAddress + usNDiscrete) >
         (MB_DISCRETE_START + MB_DISCRETE_N)))
    {
        return MB_ENOREG;
    }

    USHORT index = usAddress - MB_DISCRETE_START;

    for (USHORT i = 0; i < usNDiscrete; i++)
    {
        if (mb_discrete_inputs[index + i])
            pucRegBuffer[i / 8] |=  (1 << (i % 8));
        else
            pucRegBuffer[i / 8] &= ~(1 << (i % 8));
    }

    return MB_ENOERR;
}
