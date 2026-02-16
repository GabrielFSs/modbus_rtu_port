#ifndef HAL_TIME_H
#define HAL_TIME_H

#include <stdint.h>

/* ================= OPAQUE HANDLE ================= */

typedef struct hal_time_s *hal_time_t;

/* ================= DRIVER INTERFACE ================= */

typedef struct
{
    uint32_t (*get_ms)(void);
    void     (*delay_ms)(uint32_t ms);

} hal_time_driver_t;

/* ================= API ================= */

void hal_time_init(const hal_time_driver_t *driver);

uint32_t hal_time_ms(void);

void hal_time_delay_ms(uint32_t ms);

#endif
