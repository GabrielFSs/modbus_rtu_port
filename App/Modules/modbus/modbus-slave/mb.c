/* ----------------------- System includes ----------------------------------*/
#include "mb.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbfunc.h"
#include "mbport.h"
#include "mbproto.h"
#include "mb_port.h"

#include <stdlib.h>
#include <string.h>

/* ----------------------- Mode specific includes ---------------------------*/
#if MB_RTU_ENABLED == 1
#include "mbrtu.h"
#endif

#if MB_ASCII_ENABLED == 1
#include "mbascii.h"
#endif

#if MB_TCP_ENABLED == 1
#include "mbtcp.h"
#endif

/* ----------------------- Static variables ---------------------------------*/

static UCHAR    ucMBAddress;
static eMBMode  eMBCurrentMode;

static enum
{
    STATE_ENABLED,
    STATE_DISABLED,
    STATE_NOT_INITIALIZED
} eMBState = STATE_NOT_INITIALIZED;

static peMBFrameSend    peMBFrameSendCur;
static pvMBFrameStart   pvMBFrameStartCur;
static pvMBFrameStop    pvMBFrameStopCur;
static peMBFrameReceive peMBFrameReceiveCur;
static pvMBFrameClose   pvMBFrameCloseCur;

/* Port callbacks */
BOOL( *pxMBFrameCBByteReceived ) ( void );
BOOL( *pxMBFrameCBTransmitterEmpty ) ( void );
BOOL( *pxMBPortCBTimerExpired ) ( void );

BOOL( *pxMBFrameCBReceiveFSMCur ) ( void );
BOOL( *pxMBFrameCBTransmitFSMCur ) ( void );

/* Function handler table */
static xMBFunctionHandler xFuncHandlers[MB_FUNC_HANDLERS_MAX] = {
#if MB_FUNC_OTHER_REP_SLAVEID_ENABLED > 0
    {MB_FUNC_OTHER_REPORT_SLAVEID, eMBFuncReportSlaveID},
#endif
#if MB_FUNC_READ_INPUT_ENABLED > 0
    {MB_FUNC_READ_INPUT_REGISTER, eMBFuncReadInputRegister},
#endif
#if MB_FUNC_READ_HOLDING_ENABLED > 0
    {MB_FUNC_READ_HOLDING_REGISTER, eMBFuncReadHoldingRegister},
#endif
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_REGISTERS, eMBFuncWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_REGISTER, eMBFuncWriteHoldingRegister},
#endif
#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0
    {MB_FUNC_READWRITE_MULTIPLE_REGISTERS, eMBFuncReadWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_READ_COILS_ENABLED > 0
    {MB_FUNC_READ_COILS, eMBFuncReadCoils},
#endif
#if MB_FUNC_WRITE_COIL_ENABLED > 0
    {MB_FUNC_WRITE_SINGLE_COIL, eMBFuncWriteCoil},
#endif
#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_COILS, eMBFuncWriteMultipleCoils},
#endif
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
    {MB_FUNC_READ_DISCRETE_INPUTS, eMBFuncReadDiscreteInputs},
#endif
};

/* ----------------------- eMBInit ------------------------------------------*/
eMBErrorCode
eMBInit( eMBMode eMode,
         UCHAR ucSlaveAddress,
         UCHAR ucPort,
         ULONG ulBaudRate,
         eMBParity eParity,
         UCHAR ucStopBits )
{
    eMBErrorCode eStatus = MB_ENOERR;

    if( ( ucSlaveAddress == MB_ADDRESS_BROADCAST ) ||
        ( ucSlaveAddress < MB_ADDRESS_MIN ) ||
        ( ucSlaveAddress > MB_ADDRESS_MAX ) )
    {
        return MB_EINVAL;
    }

    ucMBAddress = ucSlaveAddress;

    switch( eMode )
    {
#if MB_RTU_ENABLED > 0
    case MB_RTU:
        pvMBFrameStartCur   = eMBRTUStart;
        pvMBFrameStopCur    = eMBRTUStop;
        peMBFrameSendCur    = eMBRTUSend;
        peMBFrameReceiveCur = eMBRTUReceive;

#if MB_PORT_HAS_CLOSE
        pvMBFrameCloseCur   = vMBPortClose;
#else
        pvMBFrameCloseCur   = NULL;
#endif

        pxMBFrameCBByteReceived     = xMBRTUReceiveFSM;
        pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM;
        pxMBPortCBTimerExpired      = xMBRTUTimerT35Expired;

        eStatus = eMBRTUInit( ucMBAddress, ucPort, ulBaudRate, eParity, ucStopBits );
        break;
#endif

#if MB_ASCII_ENABLED > 0
    case MB_ASCII:
        pvMBFrameStartCur   = eMBASCIIStart;
        pvMBFrameStopCur    = eMBASCIIStop;
        peMBFrameSendCur    = eMBASCIISend;
        peMBFrameReceiveCur = eMBASCIIReceive;

#if MB_PORT_HAS_CLOSE
        pvMBFrameCloseCur   = vMBPortClose;
#else
        pvMBFrameCloseCur   = NULL;
#endif

        pxMBFrameCBByteReceived     = xMBASCIIReceiveFSM;
        pxMBFrameCBTransmitterEmpty = xMBASCIITransmitFSM;
        pxMBPortCBTimerExpired      = xMBASCIITimerT1SExpired;

        eStatus = eMBASCIIInit( ucMBAddress, ucPort, ulBaudRate, eParity, ucStopBits );
        break;
#endif

    default:
        return MB_EINVAL;
    }

    if( eStatus == MB_ENOERR )
    {
        if( !xMBPortEventInit() )
        {
            eStatus = MB_EPORTERR;
        }
        else
        {
            eMBCurrentMode = eMode;
            eMBState = STATE_DISABLED;
        }
    }

    return eStatus;
}

/* ----------------------- Enable / Disable ---------------------------------*/
eMBErrorCode eMBEnable( void )
{
    if( eMBState != STATE_DISABLED )
        return MB_EILLSTATE;

    pvMBFrameStartCur();
    eMBState = STATE_ENABLED;
    return MB_ENOERR;
}

eMBErrorCode eMBDisable( void )
{
    if( eMBState == STATE_ENABLED )
    {
        pvMBFrameStopCur();
        eMBState = STATE_DISABLED;
    }
    return MB_ENOERR;
}

eMBErrorCode eMBClose( void )
{
    if( eMBState != STATE_DISABLED )
        return MB_EILLSTATE;

    if( pvMBFrameCloseCur != NULL )
        pvMBFrameCloseCur();

    return MB_ENOERR;
}

/* ----------------------- Poll ---------------------------------------------*/
eMBErrorCode eMBPoll( void )
{
    static UCHAR   *ucMBFrame;
    static UCHAR    ucRcvAddress;
    static UCHAR    ucFunctionCode;
    static USHORT   usLength;
    static eMBException eException;

    eMBEventType eEvent;
    eMBErrorCode eStatus = MB_ENOERR;

    if( eMBState != STATE_ENABLED )
        return MB_EILLSTATE;

    if( xMBPortEventGet( &eEvent ) == TRUE )
    {
        switch( eEvent )
        {
        case EV_FRAME_RECEIVED:

            eStatus = peMBFrameReceiveCur(
                        &ucRcvAddress,
                        &ucMBFrame,
                        &usLength );

            if( eStatus == MB_ENOERR )
            {
                if( ( ucRcvAddress == ucMBAddress ) ||
                    ( ucRcvAddress == MB_ADDRESS_BROADCAST ) )
                {
                    xMBPortEventPost( EV_EXECUTE );
                }
            }
            break;

        case EV_EXECUTE:

            ucFunctionCode = ucMBFrame[MB_PDU_FUNC_OFF];
            eException = MB_EX_ILLEGAL_FUNCTION;

            for( int i = 0; i < MB_FUNC_HANDLERS_MAX; i++ )
            {
                if( xFuncHandlers[i].ucFunctionCode == 0 )
                    break;

                if( xFuncHandlers[i].ucFunctionCode == ucFunctionCode )
                {
                    eException =
                        xFuncHandlers[i].pxHandler( ucMBFrame, &usLength );
                    break;
                }
            }

            if( ucRcvAddress != MB_ADDRESS_BROADCAST )
            {
                if( eException != MB_EX_NONE )
                {
                    usLength = 0;
                    ucMBFrame[usLength++] =
                        (UCHAR)( ucFunctionCode | MB_FUNC_ERROR );
                    ucMBFrame[usLength++] = eException;
                }

                eStatus = peMBFrameSendCur(
                            ucMBAddress,
                            ucMBFrame,
                            usLength );
            }
            break;

        default:
            break;
        }
    }

    return eStatus;
}
