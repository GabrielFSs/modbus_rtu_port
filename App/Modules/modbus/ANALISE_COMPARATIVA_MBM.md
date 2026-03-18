# Análise Comparativa: mbm.c, mbm_port/port e mbm_rtu.c

Comparação entre o projeto **ModbusRTU** (atual) e o projeto de **referência** (modbus_rtu_port-test-modbus-master-gpio-led-3buttons) para identificar diferenças que possam causar envio 8x.

---

## 1. Arquitetura de Port

| Aspecto | ModbusRTU (atual) | Referência |
|---------|-------------------|------------|
| **Port usado** | `port_modbus_manager_stm32f4.c` (via modbus_manager) | `mbm_port.c` (standalone) |
| **mbm_port.c** | Compilado mas **não usado** (modbus_manager usa port próprio) | Usado diretamente |

O ModbusRTU usa `modbus_manager` que monta `master_port` a partir de `modbus_manager_port`. O `mbm_port.c` existe no projeto mas não é chamado em modo Master.

---

## 2. mbm.c – Diferenças

### 2.1 Defines e variáveis

| Item | ModbusRTU | Referência |
|------|-----------|------------|
| `MBM_TURNAROUND_MS` | 5 | **Não existe** |
| `turnaround_end` | Sim | **Não existe** |
| `MBM_SCHED_MIN_INTERVAL_MS` | 50 | **Não existe** |
| `MBM_SLAVE_BACKOFF_MS` | 2000 | 5000 |

### 2.2 IDLE – antes de enviar

**ModbusRTU:**
```c
if (queue_count == 0)
    break;

if (now < turnaround_end)   // ← NOVO: espera 5ms após resposta
    break;

current_job = &job_queue[0];
// ... slave offline check com callback ...
mbm_rtu_start_tx(current_job);
```

**Referência:**
```c
if (queue_count == 0)
    break;

current_job = &job_queue[0];
// ... slave offline: só remove_current_job(), sem callback ...
mbm_rtu_start_tx(current_job);
```

O turnaround no ModbusRTU é uma proteção extra (3.5 char) e não deve causar 8x.

### 2.3 Slave offline

**ModbusRTU:** Chama `current_job->callback(NULL, 0, MBM_ERR_TIMEOUT, job_id)` e define `turnaround_end`.  
**Referência:** Apenas `remove_current_job()` – sem callback.

### 2.4 Scheduler (loop de jobs agendados)

**ModbusRTU:**
```c
if (mbm_add_request(&scheduled[i].req) == MBM_ERR_OK)
{
    uint32_t period = scheduled[i].req.period_ms;
    if (period < MBM_SCHED_MIN_INTERVAL_MS)
        period = MBM_SCHED_MIN_INTERVAL_MS;
    scheduled[i].next_run = now + period;
}
else
{
    scheduled[i].next_run = now + 100;  // Fila cheia: retry em 100ms
}
```

**Referência:**
```c
if (mbm_add_request(&scheduled[i].req) == MBM_ERR_OK)
    scheduled[i].next_run = now + scheduled[i].req.period_ms;
// Se falhar: next_run NÃO é atualizado → tenta de novo no próximo poll
```

O ModbusRTU trata melhor fila cheia (backoff 100ms). A referência pode tentar `mbm_add_request` em todo poll enquanto a fila estiver cheia.

### 2.5 WAIT_RESPONSE – `!current_job`

**ModbusRTU:** `break` (continua e sai do switch).  
**Referência:** `return` (sai de `mbm_poll` imediatamente).

Comportamento equivalente em termos de fluxo.

### 2.6 Callback

**ModbusRTU:** 4 parâmetros `(response, length, status, job_id)`.  
**Referência:** 3 parâmetros `(response, length, status)`.

---

## 3. Port (UART/TX) – Diferenças Críticas

### 3.1 ModbusRTU: `port_modbus_manager_stm32f4.c`

```c
static void port_uart_send(uint8_t *data, uint16_t len)
{
    hal_uart_write_blocking(uart, data, len, 1000);  // BLOQUEANTE
}
```

- **UART:** `UART_DUPLEX_FULL`, `UART_DIR_NONE` (sem RS485)
- **TX:** Bloqueante – espera fim da transmissão antes de retornar
- **RX:** Callback por byte via `manager_uart_rx_cb` → `mbm_rx_byte`

### 3.2 Referência: `mbm_port.c`

```c
static void port_uart_send(uint8_t *data, uint16_t len)
{
    size_t written;
    hal_uart_write(uart, data, len, &written, 1000);  // NÃO BLOQUEANTE
}
```

- **UART:** `UART_DUPLEX_HALF` (RS485), `UART_DIR_EXTERNAL` com `rs485_dir_ctrl`
- **TX:** Não bloqueante – inicia TX e retorna; conclusão em `TxCpltCallback`
- **RX:** Callback por byte em `uart_event_cb` → `mbm_rx_byte`

### 3.3 Impacto no envio 8x

- **Bloqueante:** Uma chamada a `port_uart_send` envia o frame inteiro e só retorna ao fim. Não há risco de múltiplos envios por reentrância no TX.
- **Não bloqueante:** Também envia uma vez por chamada; o HAL cuida do DMA/IT.

Ou seja, a diferença TX bloqueante vs não bloqueante não explica sozinha o envio 8x.

---

## 4. mbm_rtu.c – Diferenças

| Item | ModbusRTU | Referência |
|------|-----------|------------|
| `rx_index` | `volatile uint16_t` | `uint16_t` |
| `frame_ready` | `volatile uint8_t` | `uint8_t` |
| `build_request` | Idêntico | Idêntico |
| `mbm_rtu_start_tx` | Idêntico | Idêntico |

O `volatile` no ModbusRTU é para uso em contexto de interrupção e não altera a lógica de envio.

---

## 5. Origem do Input – Diferença Importante

### Referência: GPIO + debounce

```c
static void button_cb(hal_gpio_drv_t gpio, void *ctx)
{
    if (mbm_is_busy())           // ← Verifica se já está ocupado
        return;

    if ((now - last_press_key1) < DEBOUNCE_MS)   // 200ms
        return;

    last_press_key1 = now;
    // ...
    mbm_add_request(&req);
}
```

- Um toque físico → uma interrupção → um `mbm_add_request`
- Debounce de 200 ms
- `mbm_is_busy()` impede adicionar quando há job em andamento

### ModbusRTU: LVGL + touch

```c
static void master_send_cb(lv_event_t *e)
{
    // Cooldown 150ms
    if (now - last_send_tick < MASTER_SEND_COOLDOWN_MS)
        return;

    // NÃO verifica mbm_is_busy()
    mbm_status_t st = (req.period_ms > 0)
        ? mbm_schedule(&req) : mbm_add_request(&req);
}
```

- Touch pode gerar vários `LV_EVENT_CLICKED` por toque
- Cooldown de 150 ms reduz o efeito, mas não há `mbm_is_busy()`
- Se 8 eventos chegarem em janelas diferentes (>150 ms entre eles), podem ser 8 `mbm_add_request`

---

## 6. Conclusões e Recomendações

### O que não explica o 8x

- Diferenças em `mbm.c` (turnaround, scheduler, etc.) – são proteções adicionais
- Diferenças em `mbm_rtu.c` – apenas `volatile`
- Uso de TX bloqueante vs não bloqueante – ambos enviam uma vez por chamada

### O que pode explicar o 8x

1. **Múltiplos eventos LVGL**  
   Um único toque pode gerar vários `LV_EVENT_CLICKED`. Se o cooldown não cobrir todos, vários `mbm_add_request` podem ser feitos.

2. **Falta de `mbm_is_busy()`**  
   A referência evita adicionar quando já há job em andamento. No ModbusRTU isso não é verificado.

3. **Fila com espaço**  
   Com `MBM_MAX_QUEUE = 16`, até 8 requests podem ser enfileirados antes de qualquer processamento.

### Ações sugeridas

1. **Adicionar `mbm_is_busy()` em `master_send_cb`** (como na referência):

```c
if (modbus_manager_get_mode() != MODBUS_MODE_MASTER) { ... }
if (mbm_is_busy())
    return;  // Já tem job na fila ou em execução
```

2. **Aumentar o cooldown**  
   Ex.: de 150 ms para 250–300 ms, para reduzir o impacto de múltiplos eventos de touch.

3. **Breakpoints para confirmar**  
   - `mbm_add_request` – quantas vezes é chamado por toque  
   - `mbm_rtu_start_tx` – quantas vezes é chamado por request  
   - `port_uart_send` – quantas vezes é chamado por frame  

4. **Alinhar ao fluxo da referência**  
   - Usar `mbm_port.c` com RS485 e TX não bloqueante, se o hardware permitir  
   - Ou manter `port_modbus_manager` mas adotar as mesmas proteções (debounce + `mbm_is_busy()`)

---

## 7. Resumo das Diferenças por Arquivo

| Arquivo | ModbusRTU | Referência |
|---------|-----------|------------|
| **mbm.c** | turnaround_end, MBM_SCHED_MIN_INTERVAL, callback 4 args, slave offline callback | Sem turnaround, sem min interval, callback 3 args |
| **port** | port_modbus_manager: TX bloqueante, UART full duplex | mbm_port: TX não bloqueante, RS485 half duplex |
| **mbm_rtu.c** | rx_index/frame_ready volatile | Variáveis normais |

Nenhuma dessas diferenças indica claramente um bug que cause 8 envios no núcleo Modbus. A hipótese mais forte continua sendo múltiplos eventos de input (LVGL/touch) e ausência de `mbm_is_busy()`.
