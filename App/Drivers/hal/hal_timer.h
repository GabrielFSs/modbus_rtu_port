#ifndef _HAL_TIMER_H_
#define _HAL_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

/* ===== OPAQUE HANDLE ===== */
typedef struct hal_timer_drv_s *hal_timer_t;

/* ===== TIMER MODE ===== */
typedef enum
{
    HAL_TIMER_ONE_SHOT,
    HAL_TIMER_PERIODIC
} hal_timer_mode_t;

typedef enum
{
    HAL_TIMER_RESOLUTION_MS,
    HAL_TIMER_RESOLUTION_US
} hal_timer_resolution_t;

/* ===== TIMER STATUS ===== */
typedef enum
{
    HAL_TIMER_OK,
    HAL_TIMER_ERROR,
    HAL_TIMER_BUSY
} hal_timer_status_t;

/* ===== TIMER CALLBACK ===== */
typedef void (*hal_timer_cb_t)(hal_timer_t timer, void *ctx);

/* ===== TIMER CONFIG ===== */
typedef struct
{
    uint32_t period;
    bool periodic;
    hal_timer_resolution_t resolution;
    hal_timer_cb_t cb;
    void *cb_ctx;
} hal_timer_cfg_t;


/* ===== DRIVER INTERFACE ===== */
typedef struct
{
    void (*init)(void);
    void (*deinit)(void);

    hal_timer_t (*open)(const hal_timer_cfg_t *cfg);
    void (*close)(hal_timer_t timer);

    hal_timer_status_t (*start)(hal_timer_t timer);
    hal_timer_status_t (*stop)(hal_timer_t timer);
    hal_timer_status_t (*reset)(hal_timer_t timer);

    bool (*is_running)(hal_timer_t timer);

} hal_timer_drv_imp_t;

/* ===== DRIVER INSTANCE ===== */
extern hal_timer_drv_imp_t HAL_TIMER_DRV;

/* ===== HAL API ===== */
void hal_timer_init(void);
void hal_timer_deinit(void);

hal_timer_t hal_timer_open(const hal_timer_cfg_t *cfg);
void hal_timer_close(hal_timer_t timer);

hal_timer_status_t hal_timer_start(hal_timer_t timer);
hal_timer_status_t hal_timer_stop(hal_timer_t timer);
hal_timer_status_t hal_timer_reset(hal_timer_t timer);

bool hal_timer_is_running(hal_timer_t timer);

#endif /* _HAL_TIMER_H_ */
