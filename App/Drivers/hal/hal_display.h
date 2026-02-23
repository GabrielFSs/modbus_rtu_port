#ifndef _HAL_DISPLAY_H_
#define _HAL_DISPLAY_H_

#include <stdint.h>
#include <stddef.h>

/* ================= OPAQUE HANDLE ================= */

typedef struct hal_display_drv_s *hal_display_t;

/* ================= STATUS ================= */

typedef enum
{
    HAL_DISPLAY_OK = 0,
    HAL_DISPLAY_ERROR
} hal_display_status_t;

/* ================= DRIVER INTERFACE ================= */

typedef struct
{
    void (*init)(void);
    void (*deinit)(void);

    hal_display_t (*open)(void);
    void (*close)(hal_display_t disp);

    hal_display_status_t (*write_cmd)(hal_display_t disp, uint16_t cmd);
    hal_display_status_t (*write_data)(hal_display_t disp, uint16_t data);

    hal_display_status_t (*write_buffer)(hal_display_t disp,
                                         const uint16_t *data,
                                         size_t len);

} hal_display_drv_imp_t;

/* ================= DRIVER INSTANCE ================= */

extern hal_display_drv_imp_t *HAL_DISPLAY_DRV;

/* ================= HAL API ================= */

void hal_display_init(void);
void hal_display_deinit(void);

hal_display_t hal_display_open(void);
void hal_display_close(hal_display_t disp);

hal_display_status_t hal_display_write_cmd(hal_display_t disp, uint16_t cmd);
hal_display_status_t hal_display_write_data(hal_display_t disp, uint16_t data);

hal_display_status_t hal_display_write_buffer(hal_display_t disp,
                                              const uint16_t *data,
                                              size_t len);

#endif
