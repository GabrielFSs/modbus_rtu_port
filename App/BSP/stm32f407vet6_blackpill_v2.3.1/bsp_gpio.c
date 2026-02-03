#include "bsp_gpio.h"

/* ===== MAPEAMENTO GPIO =====
 *
 * Ajuste aqui conforme sua placa
 * (Blackpill, Nucleo, custom, etc)
 */
typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
} bsp_gpio_map_t;

static const bsp_gpio_map_t gpio_map[HAL_GPIO_N] =
{
    /* HAL_GPIO_0 */ { GPIOA, GPIO_PIN_0 },   // botão
    /* HAL_GPIO_1 */ { GPIOB, GPIO_PIN_1 },   // LED
    /* HAL_GPIO_2 */ { GPIOC, GPIO_PIN_13 },  // botão onboard
    /* HAL_GPIO_3 */ { GPIOB, GPIO_PIN_10 },  // qualquer outro
};

/* ===== CLOCK ENABLE ===== */
static void bsp_gpio_enable_clock(GPIO_TypeDef *port)
{
    if (port == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (port == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (port == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (port == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (port == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();
}

/* ===== PUBLIC API ===== */
bool bsp_gpio_get_pin(hal_gpio_id_t id,
                      GPIO_TypeDef **port,
                      uint16_t *pin)
{
    if (id >= HAL_GPIO_N || !port || !pin)
        return false;

    *port = gpio_map[id].port;
    *pin  = gpio_map[id].pin;

    bsp_gpio_enable_clock(*port);

    return true;
}

/* ===== EXTI IRQ ENABLE ===== */
void bsp_gpio_enable_irq(uint16_t pin)
{
    IRQn_Type irq;

    switch (pin)
    {
        case GPIO_PIN_0:  irq = EXTI0_IRQn;  break;
        case GPIO_PIN_1:  irq = EXTI1_IRQn;  break;
        case GPIO_PIN_2:  irq = EXTI2_IRQn;  break;
        case GPIO_PIN_3:  irq = EXTI3_IRQn;  break;
        case GPIO_PIN_4:  irq = EXTI4_IRQn;  break;

        case GPIO_PIN_5:
        case GPIO_PIN_6:
        case GPIO_PIN_7:
        case GPIO_PIN_8:
        case GPIO_PIN_9:
            irq = EXTI9_5_IRQn;
            break;

        default:
            irq = EXTI15_10_IRQn;
            break;
    }

    HAL_NVIC_SetPriority(irq, 5, 0);
    HAL_NVIC_EnableIRQ(irq);
}
