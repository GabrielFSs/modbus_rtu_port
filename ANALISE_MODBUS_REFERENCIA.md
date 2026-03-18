# Análise: App de Referência vs App Atual (Modbus)

## Resumo

O app de referência (`modbus_rtu_port-923fa35...`) usa uma arquitetura mais simples e **não depende do modbus_manager** para o fluxo principal. O app atual foi refatorado para usar `modbus_manager` + `port_modbus_manager_stm32f4`, com várias camadas adicionais.

---

## 1. Arquitetura de TX (envio de frames)

### Referência (funcional)
- **mbm_port.c** → `port_uart_send()` → `hal_uart_write(uart, data, len, &written, 1000)`
- Usa `HAL_UART_Transmit_IT` (modo interrupção)
- **Não espera** conclusão do TX; retorna imediatamente
- O frame é enviado byte a byte pela ISR, de forma contínua

### Atual
- **port_modbus_manager_stm32f4.c** → `port_uart_send()` → `hal_uart_write_blocking()`
- Usa `HAL_UART_Transmit` (modo bloqueante)
- **Espera** o envio completo antes de retornar
- O frame é enviado em um único fluxo contínuo

**Conclusão TX:** O app atual já usa TX bloqueante (`hal_uart_write_blocking`), o que deve garantir frame completo. Se ainda houver fragmentação, pode ser:
- Buffering no USB/CDC no PC
- Cabo ou hardware

---

## 2. Arquitetura de RX (recepção)

### Referência – HAL
- `UART_RX_DONE_ON_LENGTH`: **não chama** `drv->cb`
- Apenas define `drv->rx_done = true` e reinicia o timer
- Os bytes ficam em `rx_buf`; o callback **não** é chamado para cada byte

### Referência – port_modbus_manager
- Usa `UART_RX_DONE_ON_TIMEOUT` com buffer de 256 bytes
- Timer T3.5 em **microsegundos**
- Entrega batches de bytes no timeout (silêncio)

### Atual – HAL
- `UART_RX_DONE_ON_LENGTH` com `rx_done_length = 1`: **chama** `drv->cb` para cada byte
- Cada byte dispara o callback → `mbm_rx_byte()` + restart do timer T3.5
- Compatível com Modbus RTU (byte a byte + T3.5)

### Atual – port_modbus_manager
- Usa `UART_RX_DONE_ON_LENGTH` com buffer de 1 byte
- Timer T3.5 em **milissegundos**
- Entrega byte a byte via callback

**Conclusão RX:** O app atual está correto para Modbus RTU (byte a byte + T3.5). O HAL de referência, com `UART_RX_DONE_ON_LENGTH` sem callback, **não** entregaria bytes ao mbm nesse modo.

---

## 3. Fluxo Master: referência vs atual

### Referência
- `modbus_manager` usa `port_modbus_master_init()` e `port_modbus_master_set_uart_send()`
- API diferente; usa `mb_master` (não `mbm`)
- O app de referência em `app.c` está vazio (skeleton)

### Atual
- `modbus_manager` usa `port_modbus_manager_stm32f4` diretamente
- `master_port.uart_send = manager_uart_send` (wrapper com sniffer)
- `manager_uart_send` → `modbus_manager_port.uart_send` → `port_uart_send` → `hal_uart_write_blocking`

---

## 4. Slave – portserial

### Referência e atual
- Ambos usam `hal_uart_write()` para `xMBPortSerialPutByte`
- Slave envia byte a byte; cada byte usa `hal_uart_write(..., 1, ...)`
- Com `UART_MODE_INTERRUPT`, usa `HAL_UART_Transmit_IT`
- **Importante:** TX bloqueante no driver quebraria o slave (deadlock na cadeia de callbacks)

---

## 5. O que mudou e pode afetar

| Componente        | Referência              | Atual                         |
|-------------------|-------------------------|-------------------------------|
| TX Master         | `hal_uart_write` (IT)   | `hal_uart_write_blocking`    |
| RX callback       | LENGTH sem callback     | LENGTH com callback por byte |
| modbus_manager    | API antiga              | API nova com mbm              |
| Port              | port_modbus_master       | port_modbus_manager_stm32f4  |
| Sniffer           | Não integrado           | Integrado no manager         |

---

## 6. Recomendações

1. **TX Master:** Manter `hal_uart_write_blocking` para garantir frame completo.
2. **RX:** Manter `UART_RX_DONE_ON_LENGTH` com callback por byte (necessário para mbm).
3. **Slave:** Manter `hal_uart_write` não bloqueante para o slave.
4. **Se ainda houver fragmentação:** Verificar USB/CDC, cabo e, se possível, testar com RS232/RS485 direto (sem USB).

---

## 7. Código de referência para comparação

O **mbm_port.c** de referência (linhas 136–141):

```c
static void port_uart_send(uint8_t *data, uint16_t len)
{
    mb_sniffer_tx_store(data, len);
    size_t written;
    hal_uart_write(uart, data, len, &written, 1000);
}
```

O app atual usa `hal_uart_write_blocking`, que é mais adequado para evitar fragmentação.
