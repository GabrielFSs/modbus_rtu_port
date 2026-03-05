# UI Modbus — Design mínimo (LVGL)

## Resumo das APIs existentes

| Função | Uso na UI |
|--------|-----------|
| `modbus_manager_init()` | Startup |
| `modbus_manager_start(mode, &cfg)` | Modo + Barramento: aplicar modo (Master/Slave) e cfg serial; `cfg.slave_id` usado em modo Slave |
| `modbus_manager_stop()` | Parar Modbus |
| `modbus_manager_poll()` | Loop principal |
| `modbus_manager_get_mode()` | Mostrar estado actual (Nenhum / Master / Slave) |
| `modbus_manager_enable_sniffer(bool)` | Ligar/desligar log para SD |
| `mbm_add_request(&req)` | Master: enviar pedido (slave_id, function, address, quantity, data/bit_data conforme função) |
| `mbm_is_busy()` | Master: desabilitar "Enviar" enquanto busy |
| `mb_holding_regs[]`, `mb_input_regs[]`, `mb_coils[]` | Slave: ler/escrever mapa (modbus_map.h: MB_HOLDING_REGS_N=10, MB_COILS_N=16, etc.) |
| `mb_sniffer_get_last_frames(out, max, &count)` | Aba Log: últimos N frames (timestamp + RX/TX + hex) |

---

## Opções fixas (dropdowns / rollers)

- **Baudrate:** 9600, 19200, 38400, 57600, 115200, 230400 (hal_uart.h)
- **Databits:** 7, 8, 9 (HAL_UART_DATABITS_7/8/9)
- **Parity:** Nenhum=0, Par=1, Ímpar=2 (modbus_manager parity)
- **Stop bits:** 1, 2 (hal_uart)
- **Modo:** Nenhum, Slave, Master
- **Slave ID (modo Slave):** 1..247 (campo numérico ou roller)
- **Função (Master):**  
  Read Coils=1, Read Discrete=2, Read Holding=3, Read Input=4,  
  Write Coil=5, Write Register=6, Write Multiple Coils=15, Write Multiple Regs=16  
  (mbm_rtu.c / mbproto.h)

---

## Estrutura da UI (tabview — 5 abas)

1. **Modo**  
   - 2 botões: "Master", "Slave"  
   - 1 botão: "Parar" (só activo se modo ≠ Nenhum)  
   - Label: "Actual: Nenhum | Master | Slave"  
   - Ao premir Master/Slave: não inicia logo; usuário vai a "Barramento", configura e "Aplicar" inicia com esse modo.

2. **Barramento**  
   - Dropdown: Baudrate (lista fixa)  
   - Dropdown: Databits (7, 8, 9)  
   - Dropdown: Parity (Nenhum, Par, Ímpar)  
   - Dropdown: Stop bits (1, 2)  
   - Campo: Slave ID (1–247, só relevante em Slave)  
   - Botão: "Aplicar" → `modbus_manager_stop(); modbus_manager_start(mode_escolhido, &cfg);`  
   - Nota: modo_escolhido vem da aba Modo (variável global ou estado da UI).

3. **Master**  
   - Dropdown: Função (1, 2, 3, 4, 5, 6, 15, 16) — texto curto: "Read Coils", "Write Reg", etc.  
   - Campo: Slave ID (1–247)  
   - Campo: Endereço (0–65535)  
   - Campo: Quantidade (1–125 para regs, 1–2000 para coils conforme Modbus)  
   - Para Write (5, 6, 15, 16): valor(es) — 1 valor para 5/6; lista/área para 15/16  
   - Botão: "Enviar" → preencher `mbm_request_t`, chamar `mbm_add_request(&req)`  
   - Label de status: "OK" / "Timeout" / "Busy" (via callback do request)

4. **Slave**  
   - Tabela/listas por tipo:  
     - Holding (0..MB_HOLDING_REGS_N-1): valor por registo; clicar = editar (teclado numérico ou roller)  
     - Input (0..MB_INPUT_REGS_N-1): idem  
     - Coils (0..MB_COILS_N-1): 0/1 ou checkboxes  
   - Edição = escrever em `mb_holding_regs[i]`, `mb_input_regs[i]`, `mb_coils[i]`

5. **Log**  
   - Lista/rolagem: últimos N frames (ex.: 10)  
   - Cada linha: `HH:MM:SS.mmm RX|TX XX XX XX ...` (hex)  
   - Dados: `mb_sniffer_get_last_frames(buf, 10, &n)` e formatar

---

## Fluxo de dados

- **Modo:** Ao premir "Master" ou "Slave" guarda-se `mode_escolhido`; "Aplicar" em Barramento usa esse valor.  
- **Barramento:** Preencher `modbus_serial_cfg_t` (baudrate, databits, parity, stopbits, slave_id); Aplicar = start com modo actual.  
- **Master:** Um único formulário por pedido; callback do `mbm_add_request` pode actualizar um label "Resultado: OK/Timeout/CRC".  
- **Slave:** Leitura directa dos arrays do modbus_map; escrita ao editar célula.  
- **Log:** Timer ou ao entrar na aba: chamar `mb_sniffer_get_last_frames` e refrescar lista.

---

## Ficheiros

- `lvgl_ui.h` / `lvgl_ui.c`: cria tabview, painéis por aba, callbacks que chamam modbus_manager, mbm_*, modbus_map, mb_sniffer_get_last_frames.  
- Garantir que `modbus_manager_poll()` e `mb_sniffer_process()` são chamados no loop principal; `lv_timer_handler()` também.

---

## Integração no projeto

1. Adicionar LVGL ao projeto (lib + `lv_conf.h`, driver display `flush_cb` e input `read_cb` conforme `ili9341.h` e `bsp_spi.h`).
2. Incluir no build: `App/Modules/GUI/lvgl_ui.c` e paths para `lvgl_ui.h`, `modbus_manager.h`, `mbm.h`, `modbus_map.h`, `mb_sniffer.h`.
3. Em `app_setup()`: após `lv_init()`, registar display e input drivers; chamar `lvgl_modbus_ui_create(NULL)` (ou passar o screen desejado).
4. Em `app_loop()`: chamar `modbus_manager_poll()`, `mb_sniffer_process()`, `lv_tick_inc(5)` (ou num timer) e `lv_timer_handler()`.

**Nota:** As funções Write N Regs (0x10) e Write N Coils (0x0F) exigem `data[]`/`bit_data[]`; a UI actual só envia um valor para Write 1 Reg (0x06) e Write 1 Coil (0x05). Para 0x10/0x0F podes estender a aba Master com vários spinboxes ou um campo de texto (valores separados por vírgula).
