# O que falta para a GUI Modbus funcionar

## Já feito
- [x] UI LVGL (lvgl_ui.c): abas Modo, Barramento, Master, Slave, Log
- [x] Tick LVGL com hal_timer: lvgl_port_tick_init() (HAL_TIMER_1 = TIM5)
- [x] modbus_manager com slave_id; mb_sniffer_get_last_frames()
- [x] Documentação: LVGL_SETUP.md, GUIDESIGN.md

## O que falta (por ordem)

### 1. Criar `lv_conf.h`
- **Onde:** `App/Modules/GUI/lvgl/lvgl-master/lv_conf.h`
- **Como:** Copiar `lv_conf_template.h` → `lv_conf.h`; mudar `#if 0` para `#if 1`; `LV_COLOR_DEPTH` = 16.

### 2. Build no STM32CubeIDE
- **Include path:** `App/Modules/GUI/lvgl/lvgl-master`
- **Fontes no build:** `lvgl-master/src/**/*.c`, `lvgl_port.c`, `lvgl_ui.c`, e (agora) `lvgl_display.c` se existir.

### 3. ~~Driver de display (ILI9341) para LVGL~~ ✅
- **Feito:** `lvgl_display.c/h` — flush_cb → `ili9341_set_window` + `ili9341_write_pixels`, RGB565, buffer parcial.

### 4. ~~Driver de input (XPT2046) para LVGL~~ ✅
- **Feito:** `lvgl_input.c/h` — read_cb com is_touched, read_raw, raw_to_display (calibração em `bsp_spi.h`).

### 5. ~~Ligar tudo no `app.c`~~ ✅
- **Feito:** `app_setup()` chama hal_timer/gpio/spi init → modbus_manager_init → mb_sniffer → lv_init → lvgl_port_tick_init() → ili9341_port_init() → lvgl_display_init() → lvgl_input_init() → lvgl_modbus_ui_create(NULL).
- **app_loop():** modbus_manager_poll() → mb_sniffer_process() → lv_timer_handler() → HAL_Delay(5).

---

Falta apenas **1** e **2** (criar `lv_conf.h` e configurar o build no IDE). Garante que o `main.c` chama `app_setup()` uma vez e `app_loop()` no ciclo principal.
