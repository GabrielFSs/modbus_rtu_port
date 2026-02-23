#ifndef MODBUS_SLAVE_CB_H
#define MODBUS_SLAVE_CB_H

#include "mb.h"

eMBErrorCode eMBRegHoldingCB(
    UCHAR *pucRegBuffer,
    USHORT usAddress,
    USHORT usNRegs,
    eMBRegisterMode eMode
);

eMBErrorCode eMBRegInputCB(
    UCHAR *pucRegBuffer,
    USHORT usAddress,
    USHORT usNRegs
);

eMBErrorCode eMBRegCoilsCB(
    UCHAR *pucRegBuffer,
    USHORT usAddress,
    USHORT usNCoils,
    eMBRegisterMode eMode
);

eMBErrorCode eMBRegDiscreteCB(
    UCHAR *pucRegBuffer,
    USHORT usAddress,
    USHORT usNDiscrete
);

#endif
