/**
 * UI Modbus mínima (LVGL) — Modo, Barramento, Master, Slave, Log.
 * Depende: lvgl, modbus_manager, mbm, modbus_map, mb_sniffer.
 */

#include "lvgl_ui.h"
#include "modbus_manager.h"
#include "mbm.h"
#include "modbus_map.h"
#include "mb_sniffer.h"
#include <stdio.h>
#include <string.h>

/* ================= CONFIGURAÇÃO ================= */

#define GUI_PAD           8
#define GUI_BTN_H         40
#define GUI_DROPDOWN_W    200
#define GUI_LABEL_H       24

/* Baudrates (ordem igual ao dropdown) */
static const uint32_t bauds[] = { 9600, 19200, 38400, 57600, 115200, 230400 };
#define N_BAUDS           (sizeof(bauds)/sizeof(bauds[0]))

/* Funções Modbus Master (código + nome curto) */
typedef struct { uint8_t code; const char *name; } mbm_func_t;
static const mbm_func_t mbm_funcs[] = {
    { 0x03, "Read Holding" },
    { 0x04, "Read Input" },
    { 0x01, "Read Coils" },
    { 0x02, "Read Discrete" },
    { 0x06, "Write 1 Reg" },
    { 0x10, "Write N Regs" },
    { 0x05, "Write 1 Coil" },
    { 0x0F, "Write N Coils" },
};
#define N_MBM_FUNCS       (sizeof(mbm_funcs)/sizeof(mbm_funcs[0]))

/* ================= ESTADO UI ================= */

static modbus_mode_t pending_mode = MODBUS_MODE_NONE;

static lv_obj_t *label_mode_status;
static lv_obj_t *label_master_status;
static lv_obj_t *dropdown_baud;
static lv_obj_t *dropdown_databits;
static lv_obj_t *dropdown_parity;
static lv_obj_t *dropdown_stopbits;
static lv_obj_t *spinbox_slave_id_br;   /* Barramento: slave ID */
static lv_obj_t *spinbox_master_slave;
static lv_obj_t *spinbox_master_addr;
static lv_obj_t *spinbox_master_qty;
static lv_obj_t *dropdown_master_func;
static lv_obj_t *spinbox_master_val;     /* Master: valor (Write 1 Reg / 1 Coil) */
static lv_obj_t *spinbox_slave_val;      /* Slave: valor a escrever */
static lv_obj_t *dropdown_slave_type;    /* Holding / Input / Coils */
static lv_obj_t *spinbox_slave_idx;
static lv_obj_t *list_log;
static lv_obj_t *tabview;

/* ================= HELPERS ================= */

static void update_mode_status_label(void)
{
    modbus_mode_t m = modbus_manager_get_mode();
    const char *s = (m == MODBUS_MODE_MASTER) ? "Master" :
                    (m == MODBUS_MODE_SLAVE) ? "Slave" : "Nenhum";
    if (label_mode_status)
        lv_label_set_text_fmt(label_mode_status, "Actual: %s", s);
}

static void get_serial_cfg(modbus_serial_cfg_t *cfg)
{
    uint16_t sel;
    sel = lv_dropdown_get_selected(dropdown_baud);
    cfg->baudrate = (sel < N_BAUDS) ? bauds[sel] : 9600;

    sel = lv_dropdown_get_selected(dropdown_databits);
    cfg->databits = (uint8_t)(sel == 0 ? 7 : (sel == 1 ? 8 : 9));
    sel = lv_dropdown_get_selected(dropdown_parity);
    cfg->parity = (uint8_t)sel;
    sel = lv_dropdown_get_selected(dropdown_stopbits);
    cfg->stopbits = (uint8_t)(sel == 0 ? 1 : 2);
    cfg->slave_id = (uint8_t)lv_spinbox_get_value(spinbox_slave_id_br);
    if (cfg->slave_id < 1) cfg->slave_id = 1;
    if (cfg->slave_id > 247) cfg->slave_id = 247;
}

/* ================= ABA MODO ================= */

static void btn_master_cb(lv_event_t *e)
{
    (void)e;
    pending_mode = MODBUS_MODE_MASTER;
    lv_label_set_text(label_mode_status, "Seleccionado: Master (Aplicar em Barramento)");
}

static void btn_slave_cb(lv_event_t *e)
{
    (void)e;
    pending_mode = MODBUS_MODE_SLAVE;
    lv_label_set_text(label_mode_status, "Seleccionado: Slave (Aplicar em Barramento)");
}

static void btn_stop_cb(lv_event_t *e)
{
    (void)e;
    modbus_manager_stop();
    pending_mode = MODBUS_MODE_NONE;
    update_mode_status_label();
}

static void create_tab_modo(lv_obj_t *parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, GUI_PAD, 0);
    lv_obj_set_style_pad_gap(parent, GUI_PAD, 0);

    label_mode_status = lv_label_create(parent);
    lv_label_set_text(label_mode_status, "Actual: Nenhum");
    update_mode_status_label();

    lv_obj_t *btn_m = lv_btn_create(parent);
    lv_obj_set_size(btn_m, 120, GUI_BTN_H);
    lv_obj_t *lbl = lv_label_create(btn_m);
    lv_label_set_text(lbl, "Master");
    lv_obj_center(lbl);
    lv_obj_add_event_cb(btn_m, btn_master_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_s = lv_btn_create(parent);
    lv_obj_set_size(btn_s, 120, GUI_BTN_H);
    lbl = lv_label_create(btn_s);
    lv_label_set_text(lbl, "Slave");
    lv_obj_center(lbl);
    lv_obj_add_event_cb(btn_s, btn_slave_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_stop = lv_btn_create(parent);
    lv_obj_set_size(btn_stop, 120, GUI_BTN_H);
    lbl = lv_label_create(btn_stop);
    lv_label_set_text(lbl, "Parar");
    lv_obj_center(lbl);
    lv_obj_add_event_cb(btn_stop, btn_stop_cb, LV_EVENT_CLICKED, NULL);
}

/* ================= ABA BARRAMENTO ================= */

static void btn_apply_br_cb(lv_event_t *e)
{
    (void)e;
    if (pending_mode == MODBUS_MODE_NONE)
    {
        if (label_mode_status)
            lv_label_set_text(label_mode_status, "Escolha Modo primeiro");
        return;
    }
    modbus_serial_cfg_t cfg;
    get_serial_cfg(&cfg);
    if (modbus_manager_start(pending_mode, &cfg))
    {
        update_mode_status_label();
        if (label_mode_status)
            lv_label_set_text_fmt(label_mode_status, "Actual: %s (aplicado)",
                                 pending_mode == MODBUS_MODE_MASTER ? "Master" : "Slave");
    }
    else if (label_mode_status)
        lv_label_set_text(label_mode_status, "Erro ao iniciar");
}

static void create_tab_barramento(lv_obj_t *parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, GUI_PAD, 0);
    lv_obj_set_style_pad_gap(parent, GUI_PAD, 0);

    lv_obj_t *l = lv_label_create(parent);
    lv_label_set_text(l, "Baudrate");
    dropdown_baud = lv_dropdown_create(parent);
    lv_dropdown_set_options(dropdown_baud, "9600\n19200\n38400\n57600\n115200\n230400");
    lv_obj_set_width(dropdown_baud, GUI_DROPDOWN_W);

    l = lv_label_create(parent);
    lv_label_set_text(l, "Databits");
    dropdown_databits = lv_dropdown_create(parent);
    lv_dropdown_set_options(dropdown_databits, "7\n8\n9");
    lv_obj_set_width(dropdown_databits, GUI_DROPDOWN_W);

    l = lv_label_create(parent);
    lv_label_set_text(l, "Parity");
    dropdown_parity = lv_dropdown_create(parent);
    lv_dropdown_set_options(dropdown_parity, "Nenhum\nPar\nImpar");
    lv_obj_set_width(dropdown_parity, GUI_DROPDOWN_W);

    l = lv_label_create(parent);
    lv_label_set_text(l, "Stop bits");
    dropdown_stopbits = lv_dropdown_create(parent);
    lv_dropdown_set_options(dropdown_stopbits, "1\n2");
    lv_obj_set_width(dropdown_stopbits, GUI_DROPDOWN_W);

    l = lv_label_create(parent);
    lv_label_set_text(l, "Slave ID (1-247)");
    spinbox_slave_id_br = lv_spinbox_create(parent);
    lv_spinbox_set_range(spinbox_slave_id_br, 1, 247);
    lv_spinbox_set_value(spinbox_slave_id_br, 1);
    lv_spinbox_set_digit_format(spinbox_slave_id_br, 3, 0);
    lv_obj_set_width(spinbox_slave_id_br, 80);

    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 120, GUI_BTN_H);
    l = lv_label_create(btn);
    lv_label_set_text(l, "Aplicar");
    lv_obj_center(l);
    lv_obj_add_event_cb(btn, btn_apply_br_cb, LV_EVENT_CLICKED, NULL);
}

/* ================= ABA MASTER ================= */

static void mbm_callback(uint8_t *response, uint16_t length, mbm_status_t status)
{
    (void)response;
    (void)length;
    if (!label_master_status) return;
    switch (status)
    {
        case MBM_ERR_OK:       lv_label_set_text(label_master_status, "OK"); break;
        case MBM_ERR_BUSY:     lv_label_set_text(label_master_status, "Busy"); break;
        case MBM_ERR_TIMEOUT:  lv_label_set_text(label_master_status, "Timeout"); break;
        case MBM_ERR_CRC:      lv_label_set_text(label_master_status, "CRC Error"); break;
        default:               lv_label_set_text(label_master_status, "Erro"); break;
    }
}

static void btn_send_master_cb(lv_event_t *e)
{
    (void)e;
    if (modbus_manager_get_mode() != MODBUS_MODE_MASTER)
    {
        if (label_master_status)
            lv_label_set_text(label_master_status, "Inicie Modo Master primeiro");
        return;
    }
    if (mbm_is_busy())
    {
        if (label_master_status)
            lv_label_set_text(label_master_status, "Busy");
        return;
    }

    uint16_t fi = lv_dropdown_get_selected(dropdown_master_func);
    if (fi >= N_MBM_FUNCS) return;

    mbm_request_t req = {0};
    req.slave_id  = (uint8_t)lv_spinbox_get_value(spinbox_master_slave);
    req.function  = mbm_funcs[fi].code;
    req.address   = (uint16_t)lv_spinbox_get_value(spinbox_master_addr);
    req.quantity  = (uint16_t)lv_spinbox_get_value(spinbox_master_qty);
    req.timeout_ms = 500;
    req.retries   = 2;
    req.callback  = mbm_callback;

    if (req.slave_id < 1) req.slave_id = 1;
    if (req.quantity < 1) req.quantity = 1;

    static uint16_t one_reg;
    static uint8_t one_coil;
    if (req.function == 0x06) {
        one_reg = (uint16_t)(lv_spinbox_get_value(spinbox_master_val) & 0xFFFF);
        req.data = &one_reg;
        req.quantity = 1;
    }
    if (req.function == 0x05) {
        one_coil = (uint8_t)(lv_spinbox_get_value(spinbox_master_val) ? 1 : 0);
        req.bit_data = &one_coil;
        req.quantity = 1;
    }

    mbm_status_t st = mbm_add_request(&req);
    if (st != MBM_ERR_OK && label_master_status)
        lv_label_set_text(label_master_status, "Fila cheia");
}

static void create_tab_master(lv_obj_t *parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, GUI_PAD, 0);
    lv_obj_set_style_pad_gap(parent, GUI_PAD, 0);

    label_master_status = lv_label_create(parent);
    lv_label_set_text(label_master_status, "-");

    lv_obj_t *l = lv_label_create(parent);
    lv_label_set_text(l, "Funcao");
    dropdown_master_func = lv_dropdown_create(parent);
    const char *opts = "Read Holding\nRead Input\nRead Coils\nRead Discrete\nWrite 1 Reg\nWrite N Regs\nWrite 1 Coil\nWrite N Coils";
    lv_dropdown_set_options(dropdown_master_func, opts);
    lv_obj_set_width(dropdown_master_func, GUI_DROPDOWN_W);

    l = lv_label_create(parent);
    lv_label_set_text(l, "Slave ID");
    spinbox_master_slave = lv_spinbox_create(parent);
    lv_spinbox_set_range(spinbox_master_slave, 1, 247);
    lv_spinbox_set_value(spinbox_master_slave, 1);
    lv_spinbox_set_digit_format(spinbox_master_slave, 3, 0);
    lv_obj_set_width(spinbox_master_slave, 80);

    l = lv_label_create(parent);
    lv_label_set_text(l, "Endereco");
    spinbox_master_addr = lv_spinbox_create(parent);
    lv_spinbox_set_range(spinbox_master_addr, 0, 65535);
    lv_spinbox_set_value(spinbox_master_addr, 0);
    lv_spinbox_set_digit_format(spinbox_master_addr, 5, 0);
    lv_obj_set_width(spinbox_master_addr, 100);

    l = lv_label_create(parent);
    lv_label_set_text(l, "Quantidade");
    spinbox_master_qty = lv_spinbox_create(parent);
    lv_spinbox_set_range(spinbox_master_qty, 1, 125);
    lv_spinbox_set_value(spinbox_master_qty, 1);
    lv_spinbox_set_digit_format(spinbox_master_qty, 3, 0);
    lv_obj_set_width(spinbox_master_qty, 80);

    l = lv_label_create(parent);
    lv_label_set_text(l, "Valor (Write 1 Reg / 1 Coil)");
    spinbox_master_val = lv_spinbox_create(parent);
    lv_spinbox_set_range(spinbox_master_val, 0, 65535);
    lv_spinbox_set_value(spinbox_master_val, 0);
    lv_spinbox_set_digit_format(spinbox_master_val, 5, 0);
    lv_obj_set_width(spinbox_master_val, 100);

    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 120, GUI_BTN_H);
    l = lv_label_create(btn);
    lv_label_set_text(l, "Enviar");
    lv_obj_center(l);
    lv_obj_add_event_cb(btn, btn_send_master_cb, LV_EVENT_CLICKED, NULL);
}

/* ================= ABA SLAVE ================= */

static void btn_slave_write_cb(lv_event_t *e)
{
    (void)e;
    uint16_t type = lv_dropdown_get_selected(dropdown_slave_type);
    int32_t idx = lv_spinbox_get_value(spinbox_slave_idx);
    int32_t val = lv_spinbox_get_value(spinbox_slave_val);

    if (type == 0) /* Holding */
    {
        if (idx >= 0 && idx < MB_HOLDING_REGS_N)
            mb_holding_regs[idx] = (USHORT)(val & 0xFFFF);
    }
    else if (type == 1) /* Input */
    {
        if (idx >= 0 && idx < MB_INPUT_REGS_N)
            mb_input_regs[idx] = (USHORT)(val & 0xFFFF);
    }
    else /* Coils */
    {
        if (idx >= 0 && idx < MB_COILS_N)
            mb_coils[idx] = (UCHAR)(val ? 1 : 0);
    }
}

static void create_tab_slave(lv_obj_t *parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, GUI_PAD, 0);
    lv_obj_set_style_pad_gap(parent, GUI_PAD, 0);

    lv_obj_t *l = lv_label_create(parent);
    lv_label_set_text(l, "Tipo");
    dropdown_slave_type = lv_dropdown_create(parent);
    lv_dropdown_set_options(dropdown_slave_type, "Holding\nInput\nCoils");
    lv_obj_set_width(dropdown_slave_type, GUI_DROPDOWN_W);

    l = lv_label_create(parent);
    lv_label_set_text(l, "Indice (0..N-1)");
    spinbox_slave_idx = lv_spinbox_create(parent);
    lv_spinbox_set_range(spinbox_slave_idx, 0, 15);
    lv_spinbox_set_value(spinbox_slave_idx, 0);
    lv_spinbox_set_digit_format(spinbox_slave_idx, 2, 0);
    lv_obj_set_width(spinbox_slave_idx, 80);

    l = lv_label_create(parent);
    lv_label_set_text(l, "Valor (0-65535 ou 0/1 p/ coil)");
    spinbox_slave_val = lv_spinbox_create(parent);
    lv_spinbox_set_range(spinbox_slave_val, 0, 65535);
    lv_spinbox_set_value(spinbox_slave_val, 0);
    lv_spinbox_set_digit_format(spinbox_slave_val, 5, 0);
    lv_obj_set_width(spinbox_slave_val, 100);

    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 120, GUI_BTN_H);
    l = lv_label_create(btn);
    lv_label_set_text(l, "Escrever");
    lv_obj_center(l);
    lv_obj_add_event_cb(btn, btn_slave_write_cb, LV_EVENT_CLICKED, NULL);
}

/* ================= ABA LOG ================= */

static void refresh_log_tab(lv_event_t *e)
{
    (void)e;
    if (!list_log) return;
    lv_obj_clean(list_log);

    sniffer_frame_t buf[SNIFFER_LAST_FRAMES_N];
    uint8_t n = 0;
    mb_sniffer_get_last_frames(buf, SNIFFER_LAST_FRAMES_N, &n);

    char line[80];
    for (uint8_t i = 0; i < n; i++)
    {
        sniffer_frame_t *f = &buf[i];
        int pos = 0;
        pos += snprintf(line + pos, sizeof(line) - pos, "%02u:%02u:%02u %s ",
                        f->ts.hour, f->ts.min, f->ts.sec,
                        f->dir == SNIFFER_DIR_RX ? "RX" : "TX");
        for (uint16_t j = 0; j < f->length && (pos + 4) < (int)sizeof(line); j++)
            pos += snprintf(line + pos, sizeof(line) - (size_t)pos, "%02X ", f->data[j]);
        lv_obj_t *item = lv_list_add_btn(list_log, NULL, line);
        (void)item;
    }
}

static void create_tab_log(lv_obj_t *parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, GUI_PAD, 0);
    lv_obj_set_style_pad_gap(parent, GUI_PAD, 0);

    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 100, GUI_BTN_H);
    lv_obj_t *l = lv_label_create(btn);
    lv_label_set_text(l, "Actualizar");
    lv_obj_center(l);
    lv_obj_add_event_cb(btn, refresh_log_tab, LV_EVENT_CLICKED, NULL);

    list_log = lv_list_create(parent);
    lv_obj_set_flex_grow(list_log, 1);
}

/* ================= PÚBLICO ================= */

void lvgl_modbus_ui_create(lv_obj_t *parent)
{
    if (!parent)
        parent = lv_scr_act();

    tabview = lv_tabview_create(parent);
    lv_tabview_set_tab_bar_position(tabview, LV_DIR_TOP);
    lv_tabview_set_tab_bar_size(tabview, 30);

    lv_obj_t *t_modo   = lv_tabview_add_tab(tabview, "Modo");
    lv_obj_t *t_br     = lv_tabview_add_tab(tabview, "Barramento");
    lv_obj_t *t_master = lv_tabview_add_tab(tabview, "Master");
    lv_obj_t *t_slave  = lv_tabview_add_tab(tabview, "Slave");
    lv_obj_t *t_log    = lv_tabview_add_tab(tabview, "Log");

    create_tab_modo(t_modo);
    create_tab_barramento(t_br);
    create_tab_master(t_master);
    create_tab_slave(t_slave);
    create_tab_log(t_log);

    update_mode_status_label();
}
