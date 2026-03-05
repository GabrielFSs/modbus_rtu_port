/**
 * Teste Touch: STM32F407VET6 Black + ILI9341 240×320 + XPT2046 SPI2
 *
 * ==========================================================================
 * IMPLEMENTAÇÃO BASEADA EM XPT2046_touch.c (mesma placa confirmada pelo
 * STM32_LVGL project, mesmos pinos T_CS=PB12, T_IRQ=PC5, SPI2=PB13/14/15)
 * ==========================================================================
 *
 * PROTOCOLO SPI — exactamente como XPT2046_touch.c:
 *   CS fica LOW durante todos os 16 samples de X e Y
 *   Por eixo (24 clocks totais):
 *     HAL_SPI_Transmit(cmd, 1)              ← envia comando, descarta rx
 *     HAL_SPI_TransmitReceive(zeros, raw, 2) ← envia 0x00 0x00, recebe 2 bytes
 *     raw16 = (raw[0]<<8) | raw[1]          ← sem >>3, igual à biblioteca
 *
 * CALIBRAÇÃO — igual a XPT2046_touch.c:
 *   tx = (raw_x - RAW_X_MIN) * SCALE_X / (RAW_X_MAX - RAW_X_MIN)
 *   Valores derivados dos pontos do Tetris STM32 (mesma placa):
 *     display(40,40) ↔ raw16(23760, 5360)
 *     display(200,40) ↔ raw16(6920, 5688)
 *     display(200,280) ↔ raw16(6312, 25912)
 *   → X está invertido no painel (raw16 alto = esquerda)
 *   → Calibração por 4 valores: X_LEFT, X_RIGHT, Y_TOP, Y_BOTTOM
 *
 * SPI prescaler: PCLK1 42 MHz / 64 ≈ 656 kHz (datasheet recomenda < 2.5 MHz)
 * Comparar com XPT2046_touch.h: "< 2.5 Mbit speed, better ~650 Kbit"
 */

#include "main.h"
#include "app.h"
#include "hal_gpio.h"
#include "hal_spi.h"
#include "Modules/Display/ili9341.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ==========================================================================
 * CONFIG GERAL
 * ========================================================================== */
#define TOUCH_SPI_ID     HAL_SPI_2

/* Comandos XPT2046 (igual a XPT2046_touch.c) */
#define READ_Y           0x90u   /* Y-Position, differential, 12-bit */
#define READ_X           0xD0u   /* X-Position, differential, 12-bit */

/* Número de amostras por leitura (igual a XPT2046_touch.c: SAMPLES=16) */
#define XPT_SAMPLES      16u

/* SPI baudrate — datasheet recomenda < 2.5 MHz; XPT2046_touch.h diz ~650 kHz.
 * PCLK1 = 42 MHz → prescaler 64 = 656 kHz. */
#define XPT_BAUDRATE_HZ  656250u

/* ==========================================================================
 * CALIBRAÇÃO (raw16, sem >>3 — igual a XPT2046_touch.c)
 *
 * Derivado dos pontos de calibração do Tetris STM32 (mesma placa).
 * raw16 = raw12 * 8  (raw12 vem de >>3; raw16 = valor directo do SPI)
 *
 * Para recalibrar o teu painel específico:
 * 1. Activa CALIB_SHOW_RAW abaixo para ver os valores raw16 no ecrã
 * 2. Toca nos 4 cantos e anota os valores rX/rY
 * 3. Preenche as 4 constantes abaixo com os valores medidos
 *
 * Painel em portrait (0°, USB em baixo):
 *   X é INVERTIDO: raw16 alto = esquerda do ecrã, raw16 baixo = direita
 *   Y é NORMAL: raw16 baixo = topo do ecrã, raw16 alto = fundo
 * ========================================================================== */
#define XPT_X_LEFT    27970u   /* raw16 ao tocar borda esquerda (display X=0) */
#define XPT_X_RIGHT   2815u    /* raw16 ao tocar borda direita (display X=239) */
#define XPT_Y_TOP     2317u    /* raw16 ao tocar borda superior (display Y=0) */
#define XPT_Y_BOTTOM  29199u   /* raw16 ao tocar borda inferior (display Y=319) */

/* 1 = mostra raw16 rX/rY no ecrã para poder calibrar; 0 = normal */
#define CALIB_SHOW_RAW  1

/* ==========================================================================
 * DISPLAY
 * ========================================================================== */
#define BG_COLOR       0x0000u
#define TEXT_COLOR     0xFFFFu
#define DOT_COLOR      0xF800u
#define DOT_RADIUS     8u
#define DOT_TIMEOUT_MS 2000u

/* ==========================================================================
 * ESTADO
 * ========================================================================== */
static hal_gpio_drv_t g_cs;
static hal_gpio_drv_t g_irq;
static hal_spi_drv_t  g_spi;
static bool     dot_visible;
static uint16_t dot_cx, dot_cy;
static uint32_t dot_tick;
typedef enum { TS_IDLE, TS_PRESSED } ts_t;
static ts_t     ts_state;

/* ==========================================================================
 * T_PEN — activo baixo
 * hal_gpio_read com active_high=false devolve true=tocado (pino físico LOW).
 * ========================================================================== */
static bool is_touched(void)
{
    if (!g_irq) return false;
    bool v = false;
    hal_gpio_read(g_irq, &v);
    return v;
}

/* ==========================================================================
 * SPI — exactamente como XPT2046_touch.c
 *
 * Por eixo:
 *   Transmit(cmd, 1)              → 8 clocks, envia cmd, descarta rx
 *   TransmitReceive(zeros, raw, 2) → 16 clocks, envia 0x0000, recebe MSB+LSB
 *   raw16 = (raw[0]<<8) | raw[1]
 *
 * CS fica LOW durante toda a sessão de leitura (chamador gere CS).
 * ========================================================================== */
static uint16_t read_axis(uint8_t cmd)
{
    const uint8_t zeros[2] = {0x00u, 0x00u};
    uint8_t       raw[2]   = {0u, 0u};

    (void)hal_spi_transmit(g_spi, &cmd, 1u, 100u);
    (void)hal_spi_transmit_receive(g_spi, zeros, raw, 2u, 100u);

    return (uint16_t)(((uint16_t)raw[0] << 8) | (uint16_t)raw[1]);
}

/* ==========================================================================
 * LEITURA PRINCIPAL — igual a XPT2046_TouchGetRawCoordinates()
 *
 * CS fica LOW durante todos os 16 samples.
 * Verifica is_touched() ANTES de baixar CS (guarda real).
 * Lê Y antes de X em cada iteração (igual à biblioteca).
 * Devolve false se o dedo sair antes das 16 amostras.
 * ========================================================================== */
static bool read_raw(uint16_t *out_rx, uint16_t *out_ry)
{
    if (!is_touched()) return false;

    uint32_t sum_x = 0u, sum_y = 0u;
    uint8_t  n = 0u;

    hal_gpio_write(g_cs, false);   /* CS LOW */

    for (uint8_t i = 0u; i < XPT_SAMPLES; i++) {
        if (!is_touched()) break;
        n++;
        sum_y += read_axis(READ_Y);   /* Y primeiro, igual à biblioteca */
        sum_x += read_axis(READ_X);
    }

    hal_gpio_write(g_cs, true);    /* CS HIGH */

    if (n != XPT_SAMPLES) return false;

    *out_rx = (uint16_t)(sum_x / XPT_SAMPLES);
    *out_ry = (uint16_t)(sum_y / XPT_SAMPLES);
    return true;
}

/* ==========================================================================
 * CALIBRAÇÃO — igual a XPT2046_TouchGetCoordinates()
 *
 * X está INVERTIDO no nosso painel (raw16 alto = esquerda):
 *   tx = (X_LEFT - raw_x) * 240 / (X_LEFT - X_RIGHT)
 *
 * Y está NORMAL (raw16 baixo = topo):
 *   ty = (raw_y - Y_TOP) * 320 / (Y_BOTTOM - Y_TOP)
 *
 * Fórmula geral da biblioteca para orientação portrait:
 *   tx = (raw - min) * SCALE / (max - min)
 *   Aqui min/max trocados para X por causa da inversão.
 * ========================================================================== */
static void raw_to_display(uint16_t rx, uint16_t ry,
                            int16_t *out_x, int16_t *out_y)
{
    /* Clamp */
    if (rx < XPT_X_RIGHT) rx = (uint16_t)XPT_X_RIGHT;
    if (rx > XPT_X_LEFT)  rx = (uint16_t)XPT_X_LEFT;
    if (ry < XPT_Y_TOP)   ry = (uint16_t)XPT_Y_TOP;
    if (ry > XPT_Y_BOTTOM)ry = (uint16_t)XPT_Y_BOTTOM;

    /* X invertido: raw alto → esquerda (display X=0) */
    int32_t tx = (int32_t)((uint32_t)(XPT_X_LEFT - rx) * 240u /
                            (uint32_t)(XPT_X_LEFT - XPT_X_RIGHT));

    /* Y normal: raw baixo → topo (display Y=0) */
    int32_t ty = (int32_t)((uint32_t)(ry - XPT_Y_TOP) * 320u /
                            (uint32_t)(XPT_Y_BOTTOM - XPT_Y_TOP));

    if (tx < 0L)   tx = 0L;
    if (tx > 239L) tx = 239L;
    if (ty < 0L)   ty = 0L;
    if (ty > 319L) ty = 319L;

    *out_x = (int16_t)tx;
    *out_y = (int16_t)ty;
}

/* ==========================================================================
 * CÍRCULO
 * ========================================================================== */
static void fill_circle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t color)
{
    int32_t r2 = (int32_t)r * (int32_t)r;
    int32_t x0 = (int32_t)cx - (int32_t)r;
    int32_t y0 = (int32_t)cy - (int32_t)r;
    int32_t x1 = (int32_t)cx + (int32_t)r;
    int32_t y1 = (int32_t)cy + (int32_t)r;
    if (x0 < 0)  x0 = 0;
    if (y0 < 0)  y0 = 0;
    if (x1 >= ILI9341_WIDTH)  x1 = ILI9341_WIDTH  - 1;
    if (y1 >= ILI9341_HEIGHT) y1 = ILI9341_HEIGHT - 1;
    for (int32_t row = y0; row <= y1; row++) {
        int32_t dy = row - (int32_t)cy;
        for (int32_t col = x0; col <= x1; col++) {
            int32_t dx = col - (int32_t)cx;
            if (dx*dx + dy*dy <= r2)
                ili9341_fill_rect((uint16_t)col, (uint16_t)row, 1u, 1u, color);
        }
    }
}

static void erase_dot(void) {
    if (!dot_visible) return;
    fill_circle(dot_cx, dot_cy, (uint16_t)(DOT_RADIUS + 2u), BG_COLOR);
    dot_visible = false;
}

static void place_dot(uint16_t x, uint16_t y) {
    erase_dot();
    dot_cx = x; dot_cy = y;
    fill_circle(x, y, DOT_RADIUS, DOT_COLOR);
    dot_visible = true;
    dot_tick = HAL_GetTick();
}

/* ==========================================================================
 * FONTE 5×7
 * ========================================================================== */
#define FW 5u
#define FH 7u
static const uint8_t font5x7[14][FH] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00},  /* 0=sp */
    {0x00,0x04,0x00,0x00,0x04,0x00,0x00},  /* 1=:  */
    {0x0E,0x11,0x13,0x15,0x19,0x11,0x0E},  /* 2=0  */
    {0x04,0x0C,0x04,0x04,0x04,0x04,0x0E},  /* 3=1  */
    {0x0E,0x11,0x01,0x02,0x04,0x08,0x1F},  /* 4=2  */
    {0x0E,0x11,0x01,0x06,0x01,0x11,0x0E},  /* 5=3  */
    {0x02,0x06,0x0A,0x12,0x1F,0x02,0x02},  /* 6=4  */
    {0x1F,0x10,0x1E,0x01,0x01,0x11,0x0E},  /* 7=5  */
    {0x06,0x08,0x10,0x1E,0x11,0x11,0x0E},  /* 8=6  */
    {0x1F,0x01,0x02,0x04,0x08,0x08,0x08},  /* 9=7  */
    {0x0E,0x11,0x11,0x0E,0x11,0x11,0x0E},  /* 10=8 */
    {0x0E,0x11,0x11,0x0F,0x01,0x02,0x0C},  /* 11=9 */
    {0x11,0x0A,0x04,0x04,0x04,0x0A,0x11},  /* 12=X */
    {0x11,0x0A,0x04,0x04,0x04,0x04,0x04},  /* 13=Y */
};
static uint8_t cidx(char c) {
    if(c==' ') return 0; if(c==':') return 1;
    if(c>='0'&&c<='9') return (uint8_t)(2+(c-'0'));
    if(c=='X') return 12; if(c=='Y') return 13;
    return 0;
}
static void draw_char(uint16_t x, uint16_t y, char c, uint16_t fg, uint16_t bg) {
    uint16_t buf[FW*FH];
    const uint8_t *g = font5x7[cidx(c)];
    for(uint8_t r=0;r<FH;r++){uint8_t ln=g[r];
        for(uint8_t col=0;col<FW;col++)
            buf[r*FW+col]=(ln&(1u<<(4u-col)))?fg:bg;}
    ili9341_set_window(x,y,(uint16_t)(x+FW-1),(uint16_t)(y+FH-1));
    ili9341_write_pixels(buf,FW*FH);
}
static void draw_str(uint16_t x, uint16_t y, const char *s, uint16_t fg, uint16_t bg){
    while(*s&&x+FW<=ILI9341_WIDTH){draw_char(x,y,*s++,fg,bg);x+=(uint16_t)(FW+1u);}
}
static void fmt_u16(char *b, uint16_t v){
    b[0]=(v>=10000u)?(char)('0'+(v/10000u)%10u):' ';
    b[1]=(v>=1000u) ?(char)('0'+(v/1000u) %10u):' ';
    b[2]=(v>=100u)  ?(char)('0'+(v/100u)  %10u):' ';
    b[3]=(v>=10u)   ?(char)('0'+(v/10u)   %10u):' ';
    b[4]=(char)('0'+v%10u);b[5]='\0';
}

/* ==========================================================================
 * DISPLAY DE VALORES
 *
 *   Linha 1 (Y=  4): rX: 00000   ← raw16 do canal X (0xD0)
 *   Linha 2 (Y= 16): rY: 00000   ← raw16 do canal Y (0x90)
 *   Linha 3 (Y= 30): cX: 000     ← coordenada calibrada X (0..239)
 *   Linha 4 (Y= 42): cY: 000     ← coordenada calibrada Y (0..319)
 *
 * Se CALIB_SHOW_RAW=1: também mostra rX/rY para ajudar a calibrar.
 * Anota os valores rX e rY ao tocar nos 4 cantos do ecrã.
 * ========================================================================== */
static void show_values(uint16_t rx, uint16_t ry, int16_t cx, int16_t cy)
{
    char b[8];
#if CALIB_SHOW_RAW
    draw_str(0,  4, "rX:", TEXT_COLOR, BG_COLOR); fmt_u16(b,(uint16_t)rx); draw_str(24, 4,b,TEXT_COLOR,BG_COLOR);
    draw_str(0, 16, "rY:", TEXT_COLOR, BG_COLOR); fmt_u16(b,(uint16_t)ry); draw_str(24,16,b,TEXT_COLOR,BG_COLOR);
#endif
    draw_str(0, 30, "cX:", TEXT_COLOR, BG_COLOR); fmt_u16(b,(uint16_t)(cx<0?0:cx)); draw_str(24,30,b,TEXT_COLOR,BG_COLOR);
    draw_str(0, 42, "cY:", TEXT_COLOR, BG_COLOR); fmt_u16(b,(uint16_t)(cy<0?0:cy)); draw_str(24,42,b,TEXT_COLOR,BG_COLOR);
}

/* ==========================================================================
 * app_setup
 * ========================================================================== */
void app_setup(void)
{
    g_cs = NULL; g_irq = NULL; g_spi = NULL;
    dot_visible = false; dot_cx = 0; dot_cy = 0; dot_tick = 0;
    ts_state = TS_IDLE;

    /* Display */
    ili9341_port_init();
    ili9341_fill_rect(0, 0, ILI9341_WIDTH, ILI9341_HEIGHT, BG_COLOR);

    /* GPIO */
    hal_gpio_init();

    /* Backlight */
    {
        hal_gpio_cfg_t c = {.direction=HAL_GPIO_OUTPUT,.pull=HAL_GPIO_NOPULL,
                            .out_type=HAL_GPIO_PUSH_PULL,.irq_edge=HAL_GPIO_IRQ_NONE};
        hal_gpio_drv_t bl = hal_gpio_open(HAL_GPIO_LCD_BL, &c);
        if (bl) hal_gpio_write(bl, false);
    }

    /* T_CS (PB12) — active_high=true no BSP:
     *   write(false) → GPIO_PIN_RESET → CS activo (XPT2046 seleccionado)
     *   write(true)  → GPIO_PIN_SET   → CS inactivo
     * Igual a XPT2046_touch.c:
     *   CS_ON  = HAL_GPIO_WritePin(..., GPIO_PIN_RESET)
     *   CS_OFF = HAL_GPIO_WritePin(..., GPIO_PIN_SET) */
    {
        hal_gpio_cfg_t c = {.direction=HAL_GPIO_OUTPUT,.pull=HAL_GPIO_NOPULL,
                            .out_type=HAL_GPIO_PUSH_PULL,.irq_edge=HAL_GPIO_IRQ_NONE};
        g_cs = hal_gpio_open(HAL_GPIO_TOUCH_CS, &c);
        if (g_cs) hal_gpio_write(g_cs, true);  /* inactivo no início */
    }

    /* T_IRQ / T_PEN (PC5) — active_high=false:
     *   hal_gpio_read devolve true=tocado (pino físico LOW)
     * Igual a XPT2046_touch.c:
     *   XPT2046_TouchPressed() = (ReadPin == GPIO_PIN_RESET) → true=tocado */
    {
        hal_gpio_cfg_t c = {.direction=HAL_GPIO_INPUT,.pull=HAL_GPIO_PULLUP,
                            .out_type=HAL_GPIO_PUSH_PULL,.irq_edge=HAL_GPIO_IRQ_NONE};
        g_irq = hal_gpio_open(HAL_GPIO_TOUCH_TINT, &c);
    }

    /* SPI2 — PCLK1 42 MHz / 64 ≈ 656 kHz (como recomenda XPT2046_touch.h)
     * STM32_LVGL usa SPI_BAUDRATEPRESCALER_32 = 1.3 MHz; usamos 64 = 656 kHz
     * CPOL=0, CPHA=0 (Mode 0), 8-bit, polling */
    hal_spi_init();
    {
        hal_spi_cfg_t c = {
            .mode=HAL_SPI_MODE_MASTER, .cpol=HAL_SPI_CPOL_LOW,
            .cpha=HAL_SPI_CPHA_1EDGE,  .bit_order=HAL_SPI_MSB_FIRST,
            .datasize=HAL_SPI_DATASIZE_8BIT, .xfer_mode=HAL_SPI_XFER_POLLING,
            .baudrate_hz=XPT_BAUDRATE_HZ, .cb=NULL, .cb_ctx=NULL,
        };
        g_spi = hal_spi_open(TOUCH_SPI_ID, &c);
    }

    /* Etiquetas fixas */
#if CALIB_SHOW_RAW
    draw_str(0, 4,  "rX:     ", TEXT_COLOR, BG_COLOR);
    draw_str(0, 16, "rY:     ", TEXT_COLOR, BG_COLOR);
#endif
    draw_str(0, 30, "cX:     ", TEXT_COLOR, BG_COLOR);
    draw_str(0, 42, "cY:     ", TEXT_COLOR, BG_COLOR);
}

/* ==========================================================================
 * app_loop
 *
 * TS_IDLE → detecta toque → lê 16 amostras (DEDO NA TELA) → TS_PRESSED
 * TS_PRESSED → aguarda largar → calibra → circulo + valores → TS_IDLE
 * ========================================================================== */
static uint16_t pend_rx, pend_ry;
static bool     pend_valid;

void app_loop(void)
{
    uint32_t now = HAL_GetTick();

    if (dot_visible && (now - dot_tick) >= DOT_TIMEOUT_MS)
        erase_dot();

    if (ts_state == TS_IDLE) {
        if (read_raw(&pend_rx, &pend_ry)) {
            pend_valid = true;
            ts_state   = TS_PRESSED;
        }
        return;
    }

    /* TS_PRESSED: aguarda largar */
    if (!is_touched()) {
        if (pend_valid) {
            int16_t cx = 0, cy = 0;
            raw_to_display(pend_rx, pend_ry, &cx, &cy);
            show_values(pend_rx, pend_ry, cx, cy);
            place_dot((uint16_t)cx, (uint16_t)cy);
            pend_valid = false;
        }
        ts_state = TS_IDLE;
    }
}
