# Fluxo GUI ↔ Modbus (sem bloqueios)

## 1. Modbus manager — só um poll por ciclo

No `app_loop()` chama-se **apenas** `modbus_manager_poll()`. Dentro dele:

- **`current_mode == MODBUS_MODE_NONE`** → não chama nem `eMBPoll()` nem `mbm_poll()`.
- **`current_mode == MODBUS_MODE_SLAVE`** → chama **só** `eMBPoll()` (FreeModbus).
- **`current_mode == MODBUS_MODE_MASTER`** → chama **só** `mbm_poll()` (teu master).

Ou seja: em cada ciclo corre **só um** dos dois, consoante o modo actual. O modo muda quando o utilizador clica em “Aplicar” (Barramento) com Master ou Slave seleccionado, ou em “Parar”.

---

## 2. Onde a GUI “envia” e “recebe”

| Onde (ficheiro / callback) | O que faz |
|----------------------------|-----------|
| **app_setup()** | Cria a GUI com `lvgl_modbus_ui_create(NULL)` (tabview + abas). |
| **Aba Modo → “Master” / “Slave”** | Guarda `pending_mode`; não inicia o barramento. |
| **Aba Modo → “Parar”** | `modbus_manager_stop()` e `pending_mode = NONE`. |
| **Aba Barramento → “Aplicar”** | Monta `modbus_serial_cfg_t` (baud, parity, etc.) e chama `modbus_manager_start(pending_mode, &cfg)` → inicia UART + modo Master ou Slave. |
| **Aba Master → “Enviar”** | Preenche `mbm_request_t` (slave_id, função, endereço, quantidade, valor para 0x05/0x06) e chama `mbm_add_request(&req)`. O resultado (OK/Timeout/Busy) aparece no label via `mbm_callback`. |
| **Aba Slave → “Escrever”** | Escreve em `mb_holding_regs[]`, `mb_input_regs[]` ou `mb_coils[]`. Quando um master externo faz pedidos, o FreeModbus usa estes mapas. |
| **Aba Log → “Actualizar”** | Chama `mb_sniffer_get_last_frames()` e mostra os últimos frames na lista. |

O **envio/receção real na UART** não é feito nos callbacks da GUI: é feito no **app_loop()** por `modbus_manager_poll()` (e, em modo slave, por `eMBPoll()`; em modo master, por `mbm_poll()`). A GUI só altera configuração, enfileira pedidos ou escreve nos mapas; o loop trata do barramento.

---

## 3. app_loop sem bloqueios

- **Antes:** `HAL_Delay(5)` bloqueava o processador.
- **Agora:**
  - Só se chama `modbus_manager_poll()`, `mb_sniffer_process()` e `lv_timer_handler()` de **5 em 5 ms** (usando `HAL_GetTick()`).
  - No resto do tempo chama-se **`app_idle()`**, que por defeito faz **`__WFI()`** (CPU em baixo consumo até à próxima interrupção). Podes redefinir `app_idle()` noutro ficheiro (por exemplo para `vTaskDelay()` se usares FreeRTOS) sem alterar o `app.c`.

Assim não há delay bloqueante; o ciclo continua a correr e a CPU pode dormir entre ciclos úteis.
