#include "bsp_gpio.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    bool active_high;
} bsp_gpio_map_t;

static const bsp_gpio_map_t gpio_map[HAL_GPIO_N] =
{
    /* HAL_GPIO_0 */        { GPIOA, GPIO_PIN_0, true  },
    /* HAL_GPIO_1 */        { GPIOA, GPIO_PIN_6, false },
    /* HAL_GPIO_2 */        { GPIOA, GPIO_PIN_7, false },
    /* HAL_GPIO_3 KEY0   */ { GPIOE, GPIO_PIN_4, false },
    /* HAL_GPIO_4 KEY1   */ { GPIOE, GPIO_PIN_3, false },
    /* HAL_GPIO_RS485_DE */ { GPIOE, GPIO_PIN_4, true  },  /* PE4 (não PB12: conflita com T_CS) */
    /* HAL_GPIO_RS485_RE */ { GPIOE, GPIO_PIN_3, true  },  /* PE3 (não PB13: conflita com SPI2_SCK) */
    /* HAL_GPIO_TOUCH_CS */ { GPIOB, GPIO_PIN_12, true },  /* T_CS XPT2046 */
    /* HAL_GPIO_TOUCH_TIN*/ { GPIOC, GPIO_PIN_5, false }, /* T_PEN ativo low */
    /* HAL_GPIO_LCD_BL */   { GPIOB, GPIO_PIN_1, false },
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
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* Limpa pending do EXTI antes de habilitar NVIC (evita spurious e destrava linha) */
    __HAL_GPIO_EXTI_CLEAR_IT(pin);

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

bool bsp_gpio_get_active_level(hal_gpio_id_t id)
{
    if (id >= HAL_GPIO_N)
        return true; // default seguro: ativo HIGH

    return gpio_map[id].active_high;
}

