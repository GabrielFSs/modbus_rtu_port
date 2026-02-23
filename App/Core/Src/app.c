#include "main.h"
#include "mb.h"
#include "mbport.h"
#include "modbus_map.h"

/* ========================================================= */
/* CONFIGURAÇÃO                                              */
/* ========================================================= */

#define SLAVE_ID      0x0A
#define MB_PORT       0
#define MB_BAUDRATE   9600
#define MB_PARITY     MB_PAR_NONE
#define MB_TIMEOUT    50   /* Em ms (obrigatório nessa versão) */

/* ========================================================= */
/* SETUP                                                     */
/* ========================================================= */

void app_setup(void)
{
    eMBErrorCode eStatus;

    /* Inicializa mapa Modbus */
    modbus_map_init();

    /* Inicializa stack RTU */
    eStatus = eMBInit(MB_RTU,
                      SLAVE_ID,
                      MB_PORT,
                      MB_BAUDRATE,
                      MB_PARITY,
                      MB_TIMEOUT);

    if (eStatus != MB_ENOERR)
    {
        Error_Handler();
    }

    /* Valores iniciais para teste */
    for (int i = 0; i < MB_HOLDING_REGS_N; i++)
    {
        mb_holding_regs[i] = i + 1;
    }

    eStatus = eMBEnable();
    if (eStatus != MB_ENOERR)
    {
        Error_Handler();
    }
}

/* ========================================================= */
/* LOOP                                                      */
/* ========================================================= */

void app_loop(void)
{
    (void)eMBPoll();
}
