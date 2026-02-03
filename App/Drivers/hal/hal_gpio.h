#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

#include <stdint.h>
#include <stdbool.h>

/* ===== GPIO DEVICE ID ===== */
typedef enum
{
    HAL_GPIO_0,
    HAL_GPIO_1,
    HAL_GPIO_2,
    HAL_GPIO_3,
    HAL_GPIO_N
} hal_gpio_id_t;

/* ===== DIREÇÃO ===== */
typedef enum
{
    HAL_GPIO_INPUT,
    HAL_GPIO_OUTPUT
} hal_gpio_dir_t;

/* ===== PULL ===== */
typedef enum
{
    HAL_GPIO_NOPULL,
    HAL_GPIO_PULLUP,
    HAL_GPIO_PULLDOWN
} hal_gpio_pull_t;

/* ===== OUTPUT TYPE ===== */
typedef enum
{
    HAL_GPIO_PUSH_PULL,
    HAL_GPIO_OPEN_DRAIN
} hal_gpio_out_type_t;

/* ===== INTERRUPT EDGE ===== */
typedef enum
{
    HAL_GPIO_IRQ_NONE,
    HAL_GPIO_IRQ_RISING,
    HAL_GPIO_IRQ_FALLING,
    HAL_GPIO_IRQ_BOTH
} hal_gpio_irq_edge_t;

/* ===== STATUS ===== */
typedef enum
{
    HAL_GPIO_OK,
    HAL_GPIO_ERROR
} hal_gpio_status_t;

/* ===== CONFIG STRUCT ===== */
typedef struct
{
    hal_gpio_dir_t       direction;
    hal_gpio_pull_t      pull;
    hal_gpio_out_type_t  out_type;
    hal_gpio_irq_edge_t  irq_edge;
} hal_gpio_cfg_t;

/* ===== OPAQUE HANDLE ===== */
typedef struct hal_gpio_drv_s *hal_gpio_drv_t;

/* ===== IRQ CALLBACK ===== */
typedef void (*hal_gpio_irq_cb_t)(hal_gpio_drv_t gpio, void *ctx);

/* ===== DRIVER INTERFACE ===== */
typedef struct
{
    void (*init)(void);
    void (*deinit)(void);

    hal_gpio_drv_t (*open)(hal_gpio_id_t id, const hal_gpio_cfg_t *cfg);
    void (*close)(hal_gpio_drv_t gpio);

    hal_gpio_status_t (*write)(hal_gpio_drv_t gpio, bool value);
    hal_gpio_status_t (*read)(hal_gpio_drv_t gpio, bool *value);

    void (*set_irq_cb)(hal_gpio_drv_t gpio, hal_gpio_irq_cb_t cb, void *ctx);

} hal_gpio_drv_imp_t;

/* ===== DRIVER INSTANCE ===== */
extern hal_gpio_drv_imp_t HAL_GPIO_DRV;

/* ===== HAL API ===== */
void hal_gpio_init(void);
void hal_gpio_deinit(void);

hal_gpio_drv_t hal_gpio_open(hal_gpio_id_t id, const hal_gpio_cfg_t *cfg);
void hal_gpio_close(hal_gpio_drv_t gpio);

hal_gpio_status_t hal_gpio_write(hal_gpio_drv_t gpio, bool value);
hal_gpio_status_t hal_gpio_read(hal_gpio_drv_t gpio, bool *value);

void hal_gpio_set_irq_cb(hal_gpio_drv_t gpio, hal_gpio_irq_cb_t cb, void *ctx);

#endif /* _HAL_GPIO_H_ */
