#include "hal_display.h"

/* HAL_DISPLAY_DRV agora é ponteiro global definido
   via compile-time no hal_display_port.c */

/* ================= INIT ================= */

void hal_display_init(void)
{
    if (HAL_DISPLAY_DRV && HAL_DISPLAY_DRV->init)
        HAL_DISPLAY_DRV->init();
}

/* ================= DEINIT ================= */

void hal_display_deinit(void)
{
    if (HAL_DISPLAY_DRV && HAL_DISPLAY_DRV->deinit)
        HAL_DISPLAY_DRV->deinit();
}

/* ================= OPEN ================= */

hal_display_t hal_display_open(void)
{
    if (!HAL_DISPLAY_DRV || !HAL_DISPLAY_DRV->open)
        return NULL;

    return HAL_DISPLAY_DRV->open();
}

/* ================= CLOSE ================= */

void hal_display_close(hal_display_t disp)
{
    if (HAL_DISPLAY_DRV && HAL_DISPLAY_DRV->close)
        HAL_DISPLAY_DRV->close(disp);
}

/* ================= WRITE CMD ================= */

hal_display_status_t hal_display_write_cmd(hal_display_t disp,
                                           uint16_t cmd)
{
    if (!HAL_DISPLAY_DRV || !HAL_DISPLAY_DRV->write_cmd)
        return HAL_DISPLAY_ERROR;

    return HAL_DISPLAY_DRV->write_cmd(disp, cmd);
}

/* ================= WRITE DATA ================= */

hal_display_status_t hal_display_write_data(hal_display_t disp,
                                            uint16_t data)
{
    if (!HAL_DISPLAY_DRV || !HAL_DISPLAY_DRV->write_data)
        return HAL_DISPLAY_ERROR;

    return HAL_DISPLAY_DRV->write_data(disp, data);
}

/* ================= WRITE BUFFER ================= */

hal_display_status_t hal_display_write_buffer(hal_display_t disp,
                                              const uint16_t *data,
                                              size_t len)
{
    if (!HAL_DISPLAY_DRV || !HAL_DISPLAY_DRV->write_buffer)
        return HAL_DISPLAY_ERROR;

    return HAL_DISPLAY_DRV->write_buffer(disp, data, len);
}
