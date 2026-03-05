# LVGL — Próximos passos (STM32 + ILI9341 + XPT2046)

Resumo do que fazer para ter LVGL a correr no teu projeto e usar a UI Modbus que já criámos.

**Estrutura actual (com LVGL dentro de GUI/):**

```
App/Modules/GUI/
├── lvgl/
│   └── lvgl-master/       ← raiz do repositório LVGL
│       ├── lvgl.h
│       ├── lv_version.h
│       ├── lv_conf_template.h
│       ├── lv_conf.h      ← criar aqui (cópia do template)
│       └── src/           ← todos os .c do LVGL
├── lvgl_port.c / lvgl_port.h   ← tick com hal_timer (TIM5)
├── lvgl_ui.c / lvgl_ui.h
├── GUIDESIGN.md
└── LVGL_SETUP.md
```

---

## 1. Onde colocar `lv_conf.h`

Coloca **`lv_conf.h`** **no mesmo diretório que `lvgl.h`**, ou seja:

- **`App/Modules/GUI/lvgl/lvgl-master/lv_conf.h`**

Assim, quando o compilador trata dos fontes do LVGL (em `lvgl-master/src/`), o `lv_conf_internal.h` consegue encontrar `lv_conf.h` pelo include path (desde que **`App/Modules/GUI/lvgl/lvgl-master`** esteja nos **Include paths** do projeto).

**Como criar:**
1. Copia **`App/Modules/GUI/lvgl/lvgl-master/lv_conf_template.h`**.
2. Renomeia a cópia para **`lv_conf.h`** (no mesmo diretório).
3. Edita **`lv_conf.h`**:
   - Muda o primeiro **`#if 0`** para **`#if 1`** para activar o ficheiro.
   - Define **`LV_COLOR_DEPTH`** para **16** (ILI9341 = RGB565).
   - (Opcional) **`LV_USE_EXAMPLES`** a **1** se quiseres compilar os exemplos.

Exemplo mínimo no início de `lv_conf.h`:

```c
#if 1

#include <stdint.h>

#define LV_COLOR_DEPTH     16
#define LV_COLOR_16_SWAP   0    /* 0 = normal; 1 = troca byte (alguns displays) */

/* ... resto das opções conforme o template ... */
```

---

## 3. Adicionar LVGL ao build no STM32CubeIDE

1. **Include paths**
   - **MCU GCC Compiler** → **Include paths** adiciona:
     - **`${workspace_loc:/${ProjName}/App/Modules/GUI/lvgl/lvgl-master}`** (para `lvgl.h`, `lv_conf.h`, etc.).
   - Assim, `#include "lvgl.h"` e `#include "lv_conf.h"` funcionam quando compilas fontes do LVGL e do teu código.

2. **Fontes a compilar**
   - Incluir no build: **`App/Modules/GUI/lvgl/lvgl-master/src/**/*.c`** (todos os `.c` dentro de `src/`).
   - Incluir também: **`App/Modules/GUI/lvgl_port.c`**, **`App/Modules/GUI/lvgl_ui.c`**.

Se a pasta **App** já for uma source location do projeto, os ficheiros em **App/Modules/GUI/** serão compilados desde que não estejam excluídos. Confirma que **lvgl-master/src** está incluído (por exemplo adicionando **App/Modules/GUI** como source folder e garantindo que os subdiretórios do LVGL estão incluídos, ou referindo explicitamente **App/Modules/GUI/lvgl/lvgl-master**).

---

## 4. Tick com `hal_timer` (recomendado)

O **HAL_TIMER_0 (TIM2)** já está ocupado pelo **Modbus** (timeout 3.5T do barramento, no `modbus_manager` e nos ports do master/slave). Por isso o tick do LVGL usa **HAL_TIMER_1 (TIM5)**.

Foi criado o módulo **`lvgl_port.c` / `lvgl_port.h`** que:
- Abre **HAL_TIMER_1** com período **5 ms**, **periódico**.
- No callback da interrupção chama **`lv_tick_inc(5)`**.

**Uso:** após **`lv_init()`** e **`hal_timer_init()`**, chama **`lvgl_port_tick_init()`**. Não precisas de `lv_tick_set_cb(HAL_GetTick)` — o tick passa a ser driven pelo timer.

Resumo dos timers:
- **HAL_TIMER_0 (TIM2)** → Modbus (manager + slave/master).
- **HAL_TIMER_1 (TIM5)** → LVGL tick (5 ms).
- **HAL_TIMER_2 (TIM3)** → livre para outro uso.

---

## 5. Display (ILI9341) e Input (XPT2046)

- **Display:** Cria um **display** LVGL e regista a **flush callback** que escreve no teu ILI9341 (usa a referência em **`App/Modules/Display/ili9341.h`** — função que chama `ili9341_set_window` e `ili9341_write_pixels`).
- **Input (toque):** Cria um **indev** do tipo **pointer** e regista a **read callback** que lê o XPT2046 (usa a referência em **`App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_spi.h`** — `is_touched`, `read_raw`, `raw_to_display`, e o `touch_read_cb` para o `read_cb` do LVGL).

Ou seja: não precisas de “exemplos de botões” para começar — **a nossa UI em `lvgl_ui.c` já usa botões** (Master, Slave, Parar, Aplicar, Enviar, etc.). O LVGL também traz exemplos na pasta **`lvgl/examples`** (por exemplo **`examples/widgets/button`**); se quiseres usá-los, em **`lv_conf.h`** podes activar **`LV_USE_EXAMPLES`** e adicionar a pasta **`lvgl/examples`** ao build (e chamar a função do exemplo no teu código). Para a UI Modbus, não é obrigatório.

---

## 6. Onde fazer isto: `app.c` e loop

Sim, **convém centralizar** a inicialização e o loop no **`app.c`** (ou no `main.c` se preferires), para ficar claro o que corre.

- **Inicialização (ex.: `app_setup()`):**
  1. Hardware (BSP, display, touch, etc.).
  2. **`modbus_manager_init()`** (e, se quiseres, sniffer).
  3. **`lv_init()`**.
  4. **`lvgl_port_tick_init()`** (tick LVGL com HAL_TIMER_1).
  5. Display: criar `lv_display` e **flush_cb** (ILI9341).
  6. Input: criar `lv_indev` e **read_cb** (XPT2046).
  7. **`lvgl_modbus_ui_create(NULL)`** — cria o tabview e as abas da UI Modbus.

- **Loop (ex.: `app_loop()`):**
  - **`modbus_manager_poll()`** — **obrigatório**: é ele que decide se está em modo Master ou Slave e chama **`mbm_poll()`** ou **`eMBPoll()`** conforme o modo. Não precisas de chamar `mbm_poll`/`eMBPoll` directamente.
  - **`mb_sniffer_process();`** (se o sniffer estiver activo).
  - **`lv_timer_handler();`**
  - **`HAL_Delay(5)`** (ou outro delay ~5 ms). O tick do LVGL já é feito no timer (não precisas de `lv_tick_inc` no loop).

---

## 7. Resumo rápido

| Passo | O que fazer |
|-------|-------------|
| 1 | LVGL já está em **`App/Modules/GUI/lvgl/lvgl-master/`**. |
| 2 | Copiar **`lv_conf_template.h`** para **`lv_conf.h`** **no mesmo diretório** (`lvgl-master/`); activar com `#if 1` e **`LV_COLOR_DEPTH 16`**. |
| 3 | No STM32CubeIDE: **Include path** = **`App/Modules/GUI/lvgl/lvgl-master`**; fontes = **`lvgl-master/src/**/*.c`**, **`lvgl_port.c`**, **`lvgl_ui.c`**. |
| 4 | No código: **`lvgl_port_tick_init()`** após `lv_init()` (tick com HAL_TIMER_1). |
| 5 | Registar **display** (flush_cb → ILI9341) e **input** (read_cb → XPT2046). |
| 6 | **`lvgl_modbus_ui_create(NULL)`** após display/input; no loop: **`modbus_manager_poll()`**, **`mb_sniffer_process()`**, **`lv_timer_handler()`**, **delay ~5 ms**. |

---

## 8. Modbus: “master é id 0”?

Não. Em Modbus RTU **só os escravos têm ID (1–247)**. O **master não tem ID** no frame: o primeiro byte do PDU é sempre o **slave ID** do dispositivo a quem o master se dirige.

No nosso código:
- **`modbus_mode_t`**: **MODBUS_MODE_NONE = 0**, **MODBUS_MODE_SLAVE = 1**, **MODBUS_MODE_MASTER = 2**. Ou seja, **0 = nenhum modo** (barramento parado), não “master”.
- O **slave ID** (1–247) configura-se na aba **Barramento** (campo “Slave ID”) e é usado quando o modo é **Slave**; em modo **Master** esse campo não altera o nosso ID (porque o master não tem ID).
