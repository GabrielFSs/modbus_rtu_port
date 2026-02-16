#include "hal_timer.h"
#include "stm32f4xx_hal.h"

#include <string.h>
#include <stddef.h>

/* ===== CONFIG ===== */
#define HAL_TIMER_MAX   HAL_TIMER_N

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

/* ===== INSTANCES ===== */
static struct hal_timer_drv_s timers[HAL_TIMER_MAX];

/* ===== HW HANDLES ===== */
static TIM_HandleTypeDef htim2;
static TIM_HandleTypeDef htim5;
static TIM_HandleTypeDef htim3;

/* ===== TIMER MAP ===== */
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

/* ========================================================= */
/* ================= INIT / DEINIT ========================= */
/* ========================================================= */

static void stm32_timer_init(void)
{
    memset(timers, 0, sizeof(timers));
}

static void stm32_timer_deinit(void)
{
    for (int i = 0; i < HAL_TIMER_MAX; i++)
    {
        if (timers[i].htim)
        {
            HAL_TIM_Base_Stop_IT(timers[i].htim);
            HAL_TIM_Base_DeInit(timers[i].htim);
            timers[i].htim = NULL;
        }
    }
}

/* ========================================================= */
/* ================= OPEN / CLOSE ========================== */
/* ========================================================= */

static hal_timer_drv_t stm32_timer_open(hal_timer_id_t id,
                                    const hal_timer_cfg_t *cfg)
{
    if (!cfg || id >= HAL_TIMER_MAX)
        return NULL;

    struct hal_timer_drv_s *t = &timers[id];
    TIM_HandleTypeDef *htim = timer_hw_map(id);

    if (!htim)
        return NULL;

    /* Enable clock */
    if (id == HAL_TIMER_0) __HAL_RCC_TIM2_CLK_ENABLE();
    if (id == HAL_TIMER_1) __HAL_RCC_TIM5_CLK_ENABLE();
    if (id == HAL_TIMER_2) __HAL_RCC_TIM3_CLK_ENABLE();

    uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();

    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1)
    {
        pclk1 *= 2;
    }

    uint32_t tim_clk = pclk1;

    /* Base de 1MHz */
    uint32_t prescaler = (tim_clk / 1000000) - 1;

    /* Periodo em microsegundos */
    uint32_t period = cfg->period * 1000 - 1; // se period estiver em ms

    htim->Instance =
        (id == HAL_TIMER_0) ? TIM2 :
        (id == HAL_TIMER_1) ? TIM5 :
                              TIM3;

    htim->Init.Prescaler         = prescaler;
    htim->Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim->Init.Period            = period;
    htim->Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    if (HAL_TIM_Base_Init(htim) != HAL_OK)
        return NULL;

    HAL_TIM_Base_Stop_IT(htim);

    t->htim       = htim;
    t->periodic   = cfg->periodic;
    t->period     = cfg->period;
    t->resolution = cfg->resolution;
    t->cb         = cfg->cb;
    t->cb_ctx     = cfg->cb_ctx;
    t->state      = TIMER_STOPPED;

    IRQn_Type irq =
        (id == HAL_TIMER_0) ? TIM2_IRQn :
        (id == HAL_TIMER_1) ? TIM5_IRQn :
                              TIM3_IRQn;

    HAL_NVIC_SetPriority(irq, 5, 0);
    HAL_NVIC_EnableIRQ(irq);

    return (hal_timer_drv_t)t;
}

static void stm32_timer_close(hal_timer_drv_t timer)
{
    struct hal_timer_drv_s *t = (struct hal_timer_drv_s *)timer;

    if (!t || !t->htim)
        return;

    HAL_TIM_Base_Stop_IT(t->htim);
    HAL_TIM_Base_DeInit(t->htim);

    t->htim = NULL;
    t->state = TIMER_STOPPED;
}

/* ========================================================= */
/* ================= START / STOP / RESET ================== */
/* ========================================================= */

static hal_timer_status_t stm32_timer_start(hal_timer_drv_t timer)
{
    struct hal_timer_drv_s *t = (struct hal_timer_drv_s *)timer;

    if (!t || !t->htim)
        return HAL_TIMER_ERROR;

    __HAL_TIM_SET_COUNTER(t->htim, 0);
    HAL_TIM_Base_Start_IT(t->htim);

    t->state = TIMER_RUNNING;
    return HAL_TIMER_OK;
}

static hal_timer_status_t stm32_timer_stop(hal_timer_drv_t timer)
{
    struct hal_timer_drv_s *t = (struct hal_timer_drv_s *)timer;

    if (!t || !t->htim)
        return HAL_TIMER_ERROR;

    HAL_TIM_Base_Stop_IT(t->htim);
    t->state = TIMER_STOPPED;

    return HAL_TIMER_OK;
}

static hal_timer_status_t stm32_timer_reset(hal_timer_drv_t timer)
{
    struct hal_timer_drv_s *t = (struct hal_timer_drv_s *)timer;

    if (!t || !t->htim)
        return HAL_TIMER_ERROR;

    __HAL_TIM_SET_COUNTER(t->htim, 0);
    return HAL_TIMER_OK;
}

static bool stm32_timer_is_running(hal_timer_drv_t timer)
{
    struct hal_timer_drv_s *t = (struct hal_timer_drv_s *)timer;

    if (!t)
        return false;

    return (t->state == TIMER_RUNNING);
}

/* ========================================================= */
/* ================= IRQ CALLBACK ========================== */
/* ========================================================= */

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

/* ========================================================= */
/* ================= DRIVER EXPORT ========================= */
/* ========================================================= */

hal_timer_drv_imp_t HAL_TIMER_DRV =
{
    .init       = stm32_timer_init,
    .deinit     = stm32_timer_deinit,
    .open       = stm32_timer_open,
    .close      = stm32_timer_close,
    .start      = stm32_timer_start,
    .stop       = stm32_timer_stop,
    .reset      = stm32_timer_reset,
    .is_running = stm32_timer_is_running
};
