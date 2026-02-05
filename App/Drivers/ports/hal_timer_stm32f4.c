#include "hal_timer.h"
#include "stm32f4xx_hal.h"
#include <string.h>

/* ===== CONFIG ===== */
#define HAL_TIMER_MAX   3

/* ===== TIMER STATE ===== */
typedef enum
{
    TIMER_STOPPED = 0,
    TIMER_RUNNING
} timer_state_t;

/* ===== DRIVER STRUCT ===== */
struct hal_timer_drv_s
{
    TIM_HandleTypeDef *htim;
    timer_state_t state;

    bool periodic;
    uint32_t period;
    hal_timer_resolution_t resolution;

    hal_timer_cb_t cb;
    void *cb_ctx;
};

/* ===== INSTÂNCIAS ===== */
static struct hal_timer_drv_s timers[HAL_TIMER_MAX];

/* ===== HANDLES ===== */
static TIM_HandleTypeDef htim2;
static TIM_HandleTypeDef htim5;
static TIM_HandleTypeDef htim3;

/* ===== MAP ===== */
static TIM_HandleTypeDef *timer_hw_map(hal_timer_id_t id)
{
    switch (id)
    {
        case HAL_TIMER_0: return &htim2;
        case HAL_TIMER_1: return &htim5;
        case HAL_TIMER_2: return &htim3;
        default: return NULL;
    }
}

/* ===== INIT ===== */
static void stm32_timer_init(void)
{
    memset(timers, 0, sizeof(timers));
}

/* ===== OPEN ===== */
static hal_timer_drv_t stm32_timer_open(hal_timer_id_t id,
                                        const hal_timer_cfg_t *cfg)
{
    if (!cfg || id >= HAL_TIMER_MAX)
        return NULL;

    struct hal_timer_drv_s *t = &timers[id];
    TIM_HandleTypeDef *htim = timer_hw_map(id);

    if (!htim)
        return NULL;

    /* Clock enable */
    if (id == HAL_TIMER_0) __HAL_RCC_TIM2_CLK_ENABLE();
    if (id == HAL_TIMER_1) __HAL_RCC_TIM5_CLK_ENABLE();
    if (id == HAL_TIMER_2) __HAL_RCC_TIM3_CLK_ENABLE();

    uint32_t tim_clk = HAL_RCC_GetPCLK1Freq() * 2;
    uint32_t prescaler;
    uint32_t period;

    if (cfg->resolution == HAL_TIMER_RESOLUTION_MS)
    {
        prescaler = (tim_clk / 1000) - 1;
        period = cfg->period - 1;
    }
    else /* HAL_TIMER_RESOLUTION_US */
    {
        prescaler = (tim_clk / 1000000) - 1;
        period = cfg->period - 1;
    }

    htim->Instance =
        (id == HAL_TIMER_0) ? TIM2 :
        (id == HAL_TIMER_1) ? TIM5 : TIM3;

    htim->Init.Prescaler = prescaler;
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = period;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(htim) != HAL_OK)
        return NULL;

    HAL_TIM_Base_Stop_IT(htim);

    t->htim = htim;
    t->periodic = cfg->periodic;
    t->period = cfg->period;
    t->resolution = cfg->resolution;
    t->cb = cfg->cb;
    t->cb_ctx = cfg->cb_ctx;
    t->state = TIMER_STOPPED;

    IRQn_Type irq =
        (id == HAL_TIMER_0) ? TIM2_IRQn :
        (id == HAL_TIMER_1) ? TIM5_IRQn : TIM3_IRQn;

    HAL_NVIC_SetPriority(irq, 5, 0);
    HAL_NVIC_EnableIRQ(irq);

    return (hal_timer_drv_t)t;
}

/* ===== START ===== */
static hal_timer_status_t stm32_timer_start(hal_timer_drv_t drv)
{
    struct hal_timer_drv_s *t = drv;

    if (!t || !t->htim)
        return HAL_TIMER_ERROR;

    __HAL_TIM_SET_COUNTER(t->htim, 0);
    HAL_TIM_Base_Start_IT(t->htim);

    t->state = TIMER_RUNNING;
    return HAL_TIMER_OK;
}

/* ===== STOP ===== */
static void stm32_timer_stop(hal_timer_drv_t drv)
{
    struct hal_timer_drv_s *t = drv;

    if (!t || !t->htim)
        return;

    HAL_TIM_Base_Stop_IT(t->htim);
    t->state = TIMER_STOPPED;
}

/* ===== IRQ CALLBACK ===== */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    for (int i = 0; i < HAL_TIMER_MAX; i++)
    {
        struct hal_timer_drv_s *t = &timers[i];

        if (t->htim == htim && t->state == TIMER_RUNNING)
        {
            if (t->cb)
                t->cb((hal_timer_drv_t)t, t->cb_ctx);

            if (!t->periodic)
            {
                HAL_TIM_Base_Stop_IT(htim);
                t->state = TIMER_STOPPED;
            }
        }
    }
}

/* ===== IRQ HANDLERS ===== */
void TIM2_IRQHandler(void) { HAL_TIM_IRQHandler(&htim2); }
void TIM5_IRQHandler(void) { HAL_TIM_IRQHandler(&htim5); }
void TIM3_IRQHandler(void) { HAL_TIM_IRQHandler(&htim3); }

/* ===== EXPORT ===== */
hal_timer_drv_imp_t HAL_TIMER_DRV =
{
    .init  = stm32_timer_init,
    .open  = stm32_timer_open,
    .start = stm32_timer_start,
    .stop  = stm32_timer_stop
};
