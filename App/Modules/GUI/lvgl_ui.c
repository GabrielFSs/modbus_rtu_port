/**
 * UI Modbus (LVGL v9) — Landscape 320x240, left tabview.
 * Tabs: Config, Operate (Master/Slave dynamic), Monitor, Log.
 */

#include "lvgl_ui.h"
#include "modbus_manager.h"
#include "mbm.h"
#include "modbus_map.h"
#include "mb_sniffer.h"
#include "hal_time.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MASTER_SEND_COOLDOWN_MS  150   /* Evita múltiplos envios por evento duplicado (ex.: 8x) */

/* ================================================================
 * LAYOUT CONSTANTS
 * ================================================================ */

/* Custom number keyboard: X (close) instead of keyboard-mode icon */
static const char * const custom_kb_num_map[] = {
    "1", "2", "3", LV_SYMBOL_CLOSE, "\n",
    "4", "5", "6", LV_SYMBOL_OK,    "\n",
    "7", "8", "9", LV_SYMBOL_BACKSPACE, "\n",
    "+/-", "0", ".", LV_SYMBOL_LEFT, LV_SYMBOL_RIGHT, ""
};
static const lv_buttonmatrix_ctrl_t custom_kb_num_ctrl[] = {
    1, 1, 1, LV_KEYBOARD_CTRL_BUTTON_FLAGS | 2,
    1, 1, 1, LV_KEYBOARD_CTRL_BUTTON_FLAGS | 2,
    1, 1, 1, 2,
    1, 1, 1, 1, 1
};

#define SCREEN_W       320
#define SCREEN_H       240
#define TAB_BAR_W       52
#define CONTENT_W      (SCREEN_W - TAB_BAR_W)
#define GUI_PAD          4
#define GUI_BTN_H       28
#define ROW_H           30
#define LBL_W           62
#define GUI_KB_H       100
#define KB_PREVIEW_H    24
#define MAX_WRITE_VALS   8
#define MONITOR_MAX      8

/* ================================================================
 * LOOKUP TABLES
 * ================================================================ */

static const uint32_t bauds[]     = {9600, 19200, 38400, 57600, 115200, 230400};
static const uint32_t repeat_ms[] = {0, 100, 200, 500, 1000, 2000, 5000};
#define N_BAUDS   (sizeof(bauds)    / sizeof(bauds[0]))
#define N_REPEATS (sizeof(repeat_ms)/ sizeof(repeat_ms[0]))

typedef struct { uint8_t code; const char *name; } func_entry_t;
static const func_entry_t mbm_funcs[] = {
    {0x03, "FC03 Rd Hold"},
    {0x04, "FC04 Rd Input"},
    {0x01, "FC01 Rd Coils"},
    {0x02, "FC02 Rd Disc"},
    {0x06, "FC06 Wr 1 Reg"},
    {0x10, "FC16 Wr N Reg"},
    {0x05, "FC05 Wr 1 Coil"},
    {0x0F, "FC15 Wr N Coil"},
};
#define N_FUNCS (sizeof(mbm_funcs) / sizeof(mbm_funcs[0]))

/* ================================================================
 * GLOBAL WIDGETS
 * ================================================================ */

static lv_obj_t *tabview;
static lv_obj_t *kb;
static lv_obj_t *kb_preview_cont;
static lv_obj_t *kb_preview_lbl;

/* --- Config tab --- */
static lv_obj_t *dd_mode;
static lv_obj_t *dd_iface;
static lv_obj_t *dd_baud;
static lv_obj_t *dd_databits;
static lv_obj_t *dd_parity;
static lv_obj_t *dd_stopbits;
static lv_obj_t *ta_device_id;
static lv_obj_t *row_device_id;
static lv_obj_t *lbl_cfg_status;
static lv_obj_t *btn_stop_global;

/* --- Operate tab (Master OR Slave) --- */
static lv_obj_t *operate_tab;
static lv_obj_t *operate_master_cont;
static lv_obj_t *operate_slave_cont;
static lv_obj_t *operate_blocked_cont;

static lv_obj_t *lbl_master_status;
static lv_obj_t *dd_master_func;
static lv_obj_t *ta_master_slave;
static lv_obj_t *ta_master_addr;
static lv_obj_t *ta_master_qty;
static lv_obj_t *dd_master_repeat;
static lv_obj_t *cont_master_vals;
static lv_obj_t *ta_master_vals[MAX_WRITE_VALS];
static int       master_val_count;

static lv_obj_t *dd_slave_type;
static lv_obj_t *ta_slave_addr;
static lv_obj_t *ta_slave_val;

/* --- Monitor tab --- */
typedef struct {
    bool         active;
    uint32_t     job_id;
    uint8_t      slave_id;
    uint8_t      function;
    uint16_t     address;
    uint16_t     quantity;
    uint32_t     period;
    mbm_status_t last_status;
    bool         has_response;
    lv_obj_t    *card;
    lv_obj_t    *lbl_info;
    lv_obj_t    *lbl_status;
} monitor_entry_t;

static monitor_entry_t mon_entries[MONITOR_MAX];
static uint32_t        mon_job_counter = 1;
static lv_obj_t       *mon_list;

typedef struct {
    monitor_entry_t *me;
    lv_obj_t *panel;
    lv_obj_t *dd_func;
    lv_obj_t *ta_slave;
    lv_obj_t *ta_addr;
    lv_obj_t *ta_qty;
    lv_obj_t *dd_repeat;
} mon_edit_ctx_t;
static mon_edit_ctx_t edit_ctx;

/* --- Log tab --- */
static lv_obj_t *list_log;

/* ================================================================
 * HELPERS: NUMERIC TEXTAREA (no internal scrollbar)
 * ================================================================ */

static char  kb_old_value[8];
static bool  kb_handled;

static void kb_preview_show(void)
{
    if (kb_preview_cont) lv_obj_remove_flag(kb_preview_cont, LV_OBJ_FLAG_HIDDEN);
}

static void kb_preview_hide(void)
{
    if (kb_preview_cont) lv_obj_add_flag(kb_preview_cont, LV_OBJ_FLAG_HIDDEN);
}

static void kb_preview_update(void)
{
    if (!kb || !kb_preview_lbl) return;
    lv_obj_t *ta = lv_keyboard_get_textarea(kb);
    if (ta) {
        const char *txt = lv_textarea_get_text(ta);
        lv_label_set_text(kb_preview_lbl, txt ? txt : "");
    }
}

static void kb_dismiss(void)
{
    if (!kb) return;
    lv_keyboard_set_textarea(kb, NULL);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    kb_preview_hide();
}

static void ta_focus_cb(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
    const char *cur = lv_textarea_get_text(ta);
    strncpy(kb_old_value, cur ? cur : "", sizeof(kb_old_value) - 1);
    kb_old_value[sizeof(kb_old_value) - 1] = '\0';
    kb_handled = false;
    lv_textarea_set_text(ta, "");
    if (!kb) return;
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_remove_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_to_index(kb, -1);
    kb_preview_show();
    if (kb_preview_cont) lv_obj_move_to_index(kb_preview_cont, -1);
    kb_preview_update();
}

static void ta_value_changed_cb(lv_event_t *e)
{
    (void)e;
    kb_preview_update();
}

static void kb_ready_cb(lv_event_t *e)
{
    (void)e;
    kb_handled = true;
    kb_dismiss();
}

static void kb_cancel_cb(lv_event_t *e)
{
    (void)e;
    kb_handled = true;
    lv_obj_t *ta = kb ? lv_keyboard_get_textarea(kb) : NULL;
    if (ta)
        lv_textarea_set_text(ta, kb_old_value);
    kb_dismiss();
}

static void ta_fix_scroll(lv_obj_t *ta)
{
    lv_obj_scroll_to(ta, 0, 0, LV_ANIM_OFF);
}

static void ta_defocus_cb(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
    if (!kb_handled) {
        lv_textarea_set_text(ta, kb_old_value);
        kb_dismiss();
    }
    kb_handled = false;
    ta_fix_scroll(ta);
}

static void ta_scroll_fix_cb(lv_event_t *e)
{
    lv_obj_t *ta = lv_event_get_target(e);
    ta_fix_scroll(ta);
}

static lv_obj_t *num_field_create(lv_obj_t *parent, const char *init_val, uint8_t max_len)
{
    lv_obj_t *ta = lv_textarea_create(parent);
    lv_textarea_set_one_line(ta, true);
    lv_textarea_set_accepted_chars(ta, "0123456789");
    lv_textarea_set_max_length(ta, max_len);
    lv_textarea_set_text(ta, init_val);
    lv_obj_set_flex_grow(ta, 1);
    lv_obj_set_height(ta, ROW_H);

    /* Kill internal scrolling that causes the jumping */
    lv_obj_set_scrollbar_mode(ta, LV_SCROLLBAR_MODE_OFF);
    lv_obj_clear_flag(ta, LV_OBJ_FLAG_SCROLL_ELASTIC);
    lv_obj_clear_flag(ta, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    lv_obj_set_style_anim_duration(ta, 0, 0);

    lv_obj_add_event_cb(ta, ta_focus_cb,      LV_EVENT_FOCUSED,       NULL);
    lv_obj_add_event_cb(ta, ta_defocus_cb,     LV_EVENT_DEFOCUSED,    NULL);
    lv_obj_add_event_cb(ta, ta_value_changed_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ta, ta_scroll_fix_cb,  LV_EVENT_SCROLL,       NULL);

    lv_obj_scroll_to(ta, 0, 0, LV_ANIM_OFF);
    return ta;
}

static int32_t ta_get_int(lv_obj_t *ta, int32_t lo, int32_t hi, int32_t def)
{
    if (!ta) return def;
    const char *txt = lv_textarea_get_text(ta);
    if (!txt || txt[0] == '\0') return def;
    int32_t v = (int32_t)atoi(txt);
    if (v < lo) v = lo;
    if (v > hi) v = hi;
    return v;
}

/* ================================================================
 * HELPERS: ROW BUILDER
 * ================================================================ */

static lv_obj_t *row_create(lv_obj_t *parent)
{
    lv_obj_t *r = lv_obj_create(parent);
    lv_obj_remove_style_all(r);
    lv_obj_set_size(r, lv_pct(100), ROW_H);
    lv_obj_set_flex_flow(r, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(r, LV_FLEX_ALIGN_START,
                             LV_FLEX_ALIGN_CENTER,
                             LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(r, 4, 0);
    lv_obj_set_scrollbar_mode(r, LV_SCROLLBAR_MODE_OFF);
    return r;
}

static void row_label(lv_obj_t *row, const char *txt)
{
    lv_obj_t *l = lv_label_create(row);
    lv_label_set_text(l, txt);
    lv_obj_set_width(l, LBL_W);
}

/* ================================================================
 * HELPERS: CONFIRMATION MSGBOX
 * ================================================================ */

static void show_confirm(const char *msg)
{
    lv_obj_t *mbox = lv_msgbox_create(NULL);
    lv_msgbox_add_title(mbox, "Done");
    lv_msgbox_add_text(mbox, msg);
    lv_msgbox_add_close_button(mbox);
    lv_obj_center(mbox);
    lv_obj_move_to_index(mbox, -1);
}

/* ================================================================
 * OPERATE TAB: show/hide Master or Slave content
 * ================================================================ */

static void update_stop_button(void)
{
    if (!btn_stop_global) return;
    modbus_mode_t m = modbus_manager_get_mode();
    if (m != MODBUS_MODE_NONE) {
        lv_obj_remove_state(btn_stop_global, LV_STATE_DISABLED);
        lv_obj_set_style_bg_color(btn_stop_global, lv_color_hex(0xCC3333), 0);
        lv_obj_set_style_bg_opa(btn_stop_global, LV_OPA_COVER, 0);
    } else {
        lv_obj_add_state(btn_stop_global, LV_STATE_DISABLED);
        lv_obj_set_style_bg_color(btn_stop_global, lv_color_hex(0x8B6060), 0);
        lv_obj_set_style_bg_opa(btn_stop_global, LV_OPA_COVER, 0);
    }
}

static void operate_update_visibility(void)
{
    modbus_mode_t m = modbus_manager_get_mode();

    if (operate_master_cont)  lv_obj_add_flag(operate_master_cont,  LV_OBJ_FLAG_HIDDEN);
    if (operate_slave_cont)   lv_obj_add_flag(operate_slave_cont,   LV_OBJ_FLAG_HIDDEN);
    if (operate_blocked_cont) lv_obj_add_flag(operate_blocked_cont, LV_OBJ_FLAG_HIDDEN);

    if (m == MODBUS_MODE_MASTER && operate_master_cont)
        lv_obj_remove_flag(operate_master_cont, LV_OBJ_FLAG_HIDDEN);
    else if (m == MODBUS_MODE_SLAVE && operate_slave_cont)
        lv_obj_remove_flag(operate_slave_cont, LV_OBJ_FLAG_HIDDEN);
    else if (operate_blocked_cont)
        lv_obj_remove_flag(operate_blocked_cont, LV_OBJ_FLAG_HIDDEN);
}

/* ================================================================
 * SERIAL CONFIG HELPER
 * ================================================================ */

static void get_serial_cfg(modbus_serial_cfg_t *cfg)
{
    uint16_t sel;

    sel = lv_dropdown_get_selected(dd_iface);
    cfg->uart_dev = (uint8_t)sel;

    sel = lv_dropdown_get_selected(dd_baud);
    cfg->baudrate = (sel < N_BAUDS) ? bauds[sel] : 9600;

    sel = lv_dropdown_get_selected(dd_databits);
    cfg->databits = (uint8_t)(sel == 0 ? 7 : (sel == 1 ? 8 : 9));

    sel = lv_dropdown_get_selected(dd_parity);
    cfg->parity = (uint8_t)sel;

    sel = lv_dropdown_get_selected(dd_stopbits);
    cfg->stopbits = (uint8_t)(sel == 0 ? 1 : 2);

    cfg->slave_id = (uint8_t)ta_get_int(ta_device_id, 1, 247, 1);
}

/* ================================================================
 * TAB 1 — CONFIG  (Mode + Bus config)
 * ================================================================ */

static void mode_changed_cb(lv_event_t *e)
{
    (void)e;
    uint16_t sel = lv_dropdown_get_selected(dd_mode);
    if (sel == 0) {
        if (row_device_id) lv_obj_add_flag(row_device_id, LV_OBJ_FLAG_HIDDEN);
    } else {
        if (row_device_id) lv_obj_remove_flag(row_device_id, LV_OBJ_FLAG_HIDDEN);
    }
}

static void cfg_apply_cb(lv_event_t *e)
{
    (void)e;
    kb_dismiss();

    uint16_t mode_sel = lv_dropdown_get_selected(dd_mode);
    modbus_mode_t wanted = (mode_sel == 0) ? MODBUS_MODE_MASTER : MODBUS_MODE_SLAVE;

    modbus_manager_stop();

    modbus_serial_cfg_t cfg;
    get_serial_cfg(&cfg);

    if (wanted == MODBUS_MODE_MASTER)
        cfg.slave_id = 0;

    if (modbus_manager_start(wanted, &cfg)) {
        const char *s = (wanted == MODBUS_MODE_MASTER) ? "Master ON" : "Slave ON";
        if (lbl_cfg_status) lv_label_set_text(lbl_cfg_status, s);
        show_confirm("Config applied.");
    } else {
        if (lbl_cfg_status) lv_label_set_text(lbl_cfg_status, "Start failed");
    }
    operate_update_visibility();
    update_stop_button();
}

static void cfg_stop_cb(lv_event_t *e)
{
    (void)e;
    modbus_manager_stop();
    if (lbl_cfg_status) lv_label_set_text(lbl_cfg_status, "Stopped");
    show_confirm("Modbus stopped.");
    operate_update_visibility();
    update_stop_button();
}

static void create_tab_config(lv_obj_t *parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, GUI_PAD, 0);
    lv_obj_set_style_pad_gap(parent, 3, 0);
    lv_obj_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *row;

    row = row_create(parent);
    row_label(row, "Mode");
    dd_mode = lv_dropdown_create(row);
    lv_dropdown_set_options(dd_mode, "Master\nSlave");
    lv_obj_set_flex_grow(dd_mode, 1);
    lv_obj_set_height(dd_mode, ROW_H);
    lv_obj_add_event_cb(dd_mode, mode_changed_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lbl_cfg_status = lv_label_create(parent);
    {
        modbus_mode_t m = modbus_manager_get_mode();
        lv_label_set_text(lbl_cfg_status,
            m == MODBUS_MODE_MASTER ? "Master ON" :
            m == MODBUS_MODE_SLAVE  ? "Slave ON"  : "Stopped");
    }

    row = row_create(parent);
    row_label(row, "UART");
    dd_iface = lv_dropdown_create(row);
    lv_dropdown_set_options(dd_iface, "RS232\nRS485");
    lv_dropdown_set_selected(dd_iface, 1);
    lv_obj_set_flex_grow(dd_iface, 1);
    lv_obj_set_height(dd_iface, ROW_H);

    row = row_create(parent);
    row_label(row, "Baud");
    dd_baud = lv_dropdown_create(row);
    lv_dropdown_set_options(dd_baud, "9600\n19200\n38400\n57600\n115200\n230400");
    lv_obj_set_flex_grow(dd_baud, 1);
    lv_obj_set_height(dd_baud, ROW_H);

    row = row_create(parent);
    row_label(row, "Data");
    dd_databits = lv_dropdown_create(row);
    lv_dropdown_set_options(dd_databits, "7\n8\n9");
    lv_dropdown_set_selected(dd_databits, 1);
    lv_obj_set_flex_grow(dd_databits, 1);
    lv_obj_set_height(dd_databits, ROW_H);

    row = row_create(parent);
    row_label(row, "Parity");
    dd_parity = lv_dropdown_create(row);
    lv_dropdown_set_options(dd_parity, "None\nEven\nOdd");
    lv_obj_set_flex_grow(dd_parity, 1);
    lv_obj_set_height(dd_parity, ROW_H);

    row = row_create(parent);
    row_label(row, "Stop");
    dd_stopbits = lv_dropdown_create(row);
    lv_dropdown_set_options(dd_stopbits, "1\n2");
    lv_obj_set_flex_grow(dd_stopbits, 1);
    lv_obj_set_height(dd_stopbits, ROW_H);

    row_device_id = row_create(parent);
    row_label(row_device_id, "Dev ID");
    ta_device_id = num_field_create(row_device_id, "1", 3);
    lv_obj_add_flag(row_device_id, LV_OBJ_FLAG_HIDDEN);

    /* spacer */
    lv_obj_t *spacer = lv_obj_create(parent);
    lv_obj_remove_style_all(spacer);
    lv_obj_set_size(spacer, lv_pct(100), 6);

    lv_obj_t *btn_row = lv_obj_create(parent);
    lv_obj_remove_style_all(btn_row);
    lv_obj_set_size(btn_row, lv_pct(100), GUI_BTN_H);
    lv_obj_set_flex_flow(btn_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btn_row, LV_FLEX_ALIGN_START,
                                    LV_FLEX_ALIGN_CENTER,
                                    LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(btn_row, 6, 0);

    lv_obj_t *l;
    lv_obj_t *btn_apply = lv_button_create(btn_row);
    lv_obj_set_size(btn_apply, 90, GUI_BTN_H);
    l = lv_label_create(btn_apply);
    lv_label_set_text(l, "Apply");
    lv_obj_center(l);
    lv_obj_add_event_cb(btn_apply, cfg_apply_cb, LV_EVENT_CLICKED, NULL);

    btn_stop_global = lv_button_create(btn_row);
    lv_obj_set_size(btn_stop_global, 80, GUI_BTN_H);
    l = lv_label_create(btn_stop_global);
    lv_label_set_text(l, "Stop");
    lv_obj_center(l);
    lv_obj_add_event_cb(btn_stop_global, cfg_stop_cb, LV_EVENT_CLICKED, NULL);
    update_stop_button();
}

/* ================================================================
 * TAB 2 — OPERATE (Master content)
 * ================================================================ */

static void rebuild_val_fields(int count);

static void master_func_cb(lv_event_t *e)
{
    (void)e;
    uint16_t fi = lv_dropdown_get_selected(dd_master_func);
    if (fi >= N_FUNCS) return;
    uint8_t fc = mbm_funcs[fi].code;
    if      (fc == 0x06 || fc == 0x05) rebuild_val_fields(1);
    else if (fc == 0x10 || fc == 0x0F) {
        int qty = (int)ta_get_int(ta_master_qty, 1, MAX_WRITE_VALS, 1);
        rebuild_val_fields(qty);
    } else {
        rebuild_val_fields(0);
    }
}

static void rebuild_val_fields(int count)
{
    if (!cont_master_vals) return;
    lv_obj_clean(cont_master_vals);
    master_val_count = 0;
    for (int i = 0; i < count && i < MAX_WRITE_VALS; i++) {
        char buf[10];
        snprintf(buf, sizeof(buf), "Val %d", i + 1);
        lv_obj_t *row = row_create(cont_master_vals);
        row_label(row, buf);
        ta_master_vals[i] = num_field_create(row, "0", 5);
        master_val_count++;
    }
}

static void monitor_add_entry(const mbm_request_t *req);
static void monitor_mbm_callback(uint8_t *response, uint16_t length, mbm_status_t status, uint32_t job_id);

static void master_send_cb(lv_event_t *e)
{
    (void)e;
    kb_dismiss();

    /* Cooldown: evita múltiplos envios quando LVGL/input dispara o evento várias vezes */
    static uint32_t last_send_tick = 0;
    uint32_t now = hal_time_ms();
    if (now - last_send_tick < MASTER_SEND_COOLDOWN_MS)
        return;
    last_send_tick = now;

    if (modbus_manager_get_mode() != MODBUS_MODE_MASTER) {
        if (lbl_master_status) lv_label_set_text(lbl_master_status, "Start Master first");
        return;
    }

    if (mbm_is_busy()) {
        if (lbl_master_status) lv_label_set_text(lbl_master_status, "Busy");
        return;
    }

    uint16_t fi = lv_dropdown_get_selected(dd_master_func);
    if (fi >= N_FUNCS) return;

    mbm_request_t req = {0};
    req.slave_id   = (uint8_t) ta_get_int(ta_master_slave, 1,   247, 1);
    req.function   = mbm_funcs[fi].code;
    req.address    = (uint16_t)ta_get_int(ta_master_addr,  0, 65535, 0);
    req.quantity   = (uint16_t)ta_get_int(ta_master_qty,   1,   125, 1);
    req.timeout_ms = 500;
    req.retries    = 2;
    req.callback   = monitor_mbm_callback;
    req.job_id     = mon_job_counter++;

    uint16_t ri = lv_dropdown_get_selected(dd_master_repeat);
    req.period_ms = (ri < N_REPEATS) ? repeat_ms[ri] : 0;

    static uint16_t write_regs [MAX_WRITE_VALS];
    static uint8_t  write_coils[MAX_WRITE_VALS];

    if (req.function == 0x06) {
        write_regs[0] = (master_val_count > 0)
            ? (uint16_t)ta_get_int(ta_master_vals[0], 0, 65535, 0) : 0;
        req.data = write_regs;
        req.quantity = 1;
    } else if (req.function == 0x10) {
        int n = (int)req.quantity;
        if (n > master_val_count) n = master_val_count;
        if (n > MAX_WRITE_VALS)   n = MAX_WRITE_VALS;
        for (int i = 0; i < n; i++)
            write_regs[i] = (uint16_t)ta_get_int(ta_master_vals[i], 0, 65535, 0);
        req.data = write_regs;
        req.quantity = (uint16_t)n;
    } else if (req.function == 0x05) {
        write_coils[0] = (master_val_count > 0 &&
                          ta_get_int(ta_master_vals[0], 0, 1, 0)) ? 1 : 0;
        req.bit_data = write_coils;
        req.quantity = 1;
    } else if (req.function == 0x0F) {
        int n = (int)req.quantity;
        if (n > master_val_count) n = master_val_count;
        if (n > MAX_WRITE_VALS)   n = MAX_WRITE_VALS;
        for (int i = 0; i < n; i++)
            write_coils[i] = ta_get_int(ta_master_vals[i], 0, 1, 0) ? 1 : 0;
        req.bit_data = write_coils;
        req.quantity = (uint16_t)n;
    }

    mbm_status_t st = (req.period_ms > 0)
        ? mbm_schedule(&req) : mbm_add_request(&req);

    if (st == MBM_ERR_OK) {
        if (lbl_master_status) lv_label_set_text(lbl_master_status, "Sent OK");
        /* Adiciona card se houver slot; request já está na fila/scheduler */
        monitor_add_entry(&req);
    } else {
        if (lbl_master_status) lv_label_set_text(lbl_master_status, "Queue full");
    }
}

static void build_master_content(lv_obj_t *cont)
{
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(cont, GUI_PAD, 0);
    lv_obj_set_style_pad_gap(cont, 3, 0);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *row, *l;

    lbl_master_status = lv_label_create(cont);
    lv_label_set_text(lbl_master_status, "Status: -");

    row = row_create(cont);
    row_label(row, "Func");
    dd_master_func = lv_dropdown_create(row);
    lv_dropdown_set_options(dd_master_func,
        "FC03 Rd Hold\nFC04 Rd Input\nFC01 Rd Coils\nFC02 Rd Disc\n"
        "FC06 Wr 1 Reg\nFC16 Wr N Reg\nFC05 Wr 1 Coil\nFC15 Wr N Coil");
    lv_obj_set_flex_grow(dd_master_func, 1);
    lv_obj_set_height(dd_master_func, ROW_H);
    lv_obj_add_event_cb(dd_master_func, master_func_cb, LV_EVENT_VALUE_CHANGED, NULL);

    row = row_create(cont);
    row_label(row, "Slave");
    ta_master_slave = num_field_create(row, "1", 3);

    row = row_create(cont);
    row_label(row, "Addr");
    ta_master_addr = num_field_create(row, "0", 5);

    row = row_create(cont);
    row_label(row, "Qty");
    ta_master_qty = num_field_create(row, "1", 3);

    row = row_create(cont);
    row_label(row, "Repeat");
    dd_master_repeat = lv_dropdown_create(row);
    lv_dropdown_set_options(dd_master_repeat,
        "Once\n100ms\n200ms\n500ms\n1s\n2s\n5s");
    lv_obj_set_flex_grow(dd_master_repeat, 1);
    lv_obj_set_height(dd_master_repeat, ROW_H);

    cont_master_vals = lv_obj_create(cont);
    lv_obj_remove_style_all(cont_master_vals);
    lv_obj_set_size(cont_master_vals, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(cont_master_vals, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_gap(cont_master_vals, 3, 0);

    lv_obj_t *btn = lv_button_create(cont);
    lv_obj_set_size(btn, 90, GUI_BTN_H);
    l = lv_label_create(btn);
    lv_label_set_text(l, "Send");
    lv_obj_center(l);
    lv_obj_add_event_cb(btn, master_send_cb, LV_EVENT_CLICKED, NULL);
}

/* ================================================================
 * TAB 2 — OPERATE (Slave content)
 * ================================================================ */

static void slave_write_cb(lv_event_t *e)
{
    (void)e;
    kb_dismiss();

    uint16_t type = lv_dropdown_get_selected(dd_slave_type);
    int32_t  addr = ta_get_int(ta_slave_addr, 0, 255,   0);
    int32_t  val  = ta_get_int(ta_slave_val,  0, 65535, 0);

    if (type == 0) {
        if (addr < MB_HOLDING_REGS_N) mb_holding_regs[addr] = (USHORT)(val & 0xFFFF);
    } else if (type == 1) {
        if (addr < MB_INPUT_REGS_N)   mb_input_regs[addr]   = (USHORT)(val & 0xFFFF);
    } else {
        if (addr < MB_COILS_N)        mb_coils[addr]        = (UCHAR)(val ? 1 : 0);
    }
    show_confirm("Register written.");
}

static void build_slave_content(lv_obj_t *cont)
{
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(cont, GUI_PAD, 0);
    lv_obj_set_style_pad_gap(cont, 3, 0);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *row, *l;

    l = lv_label_create(cont);
    lv_label_set_text(l, "Slave Register Map");

    row = row_create(cont);
    row_label(row, "Type");
    dd_slave_type = lv_dropdown_create(row);
    lv_dropdown_set_options(dd_slave_type, "Holding\nInput\nCoils");
    lv_obj_set_flex_grow(dd_slave_type, 1);
    lv_obj_set_height(dd_slave_type, ROW_H);

    row = row_create(cont);
    row_label(row, "Addr");
    ta_slave_addr = num_field_create(row, "0", 3);

    row = row_create(cont);
    row_label(row, "Value");
    ta_slave_val = num_field_create(row, "0", 5);

    lv_obj_t *sp = lv_obj_create(cont);
    lv_obj_remove_style_all(sp);
    lv_obj_set_size(sp, lv_pct(100), 6);

    lv_obj_t *btn = lv_button_create(cont);
    lv_obj_set_size(btn, 90, GUI_BTN_H);
    l = lv_label_create(btn);
    lv_label_set_text(l, "Write");
    lv_obj_center(l);
    lv_obj_add_event_cb(btn, slave_write_cb, LV_EVENT_CLICKED, NULL);
}

/* ================================================================
 * TAB 2 — OPERATE (blocked placeholder)
 * ================================================================ */

static void build_blocked_content(lv_obj_t *cont)
{
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(cont, 20, 0);
    lv_obj_set_style_pad_gap(cont, 8, 0);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_CENTER,
                                 LV_FLEX_ALIGN_CENTER,
                                 LV_FLEX_ALIGN_CENTER);

    lv_obj_t *icon = lv_label_create(cont);
    lv_label_set_text(icon, LV_SYMBOL_WARNING);
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(icon, lv_color_hex(0x999999), 0);

    lv_obj_t *msg = lv_label_create(cont);
    lv_label_set_text(msg, "Go to Config and\nselect a mode first");
    lv_obj_set_style_text_align(msg, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_color(msg, lv_color_hex(0x888888), 0);
}

static void create_tab_operate(lv_obj_t *parent)
{
    operate_tab = parent;
    lv_obj_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);

    operate_master_cont = lv_obj_create(parent);
    lv_obj_remove_style_all(operate_master_cont);
    lv_obj_set_size(operate_master_cont, lv_pct(100), lv_pct(100));
    build_master_content(operate_master_cont);

    operate_slave_cont = lv_obj_create(parent);
    lv_obj_remove_style_all(operate_slave_cont);
    lv_obj_set_size(operate_slave_cont, lv_pct(100), lv_pct(100));
    build_slave_content(operate_slave_cont);

    operate_blocked_cont = lv_obj_create(parent);
    lv_obj_remove_style_all(operate_blocked_cont);
    lv_obj_set_size(operate_blocked_cont, lv_pct(100), lv_pct(100));
    build_blocked_content(operate_blocked_cont);

    operate_update_visibility();
}

/* ================================================================
 * TAB 3 — MONITOR
 * ================================================================ */

static const char *func_name_short(uint8_t fc)
{
    for (unsigned i = 0; i < N_FUNCS; i++)
        if (mbm_funcs[i].code == fc) return mbm_funcs[i].name;
    return "??";
}

static const char *status_str(mbm_status_t s)
{
    switch (s) {
        case MBM_ERR_OK:      return "OK";
        case MBM_ERR_BUSY:    return "Busy";
        case MBM_ERR_TIMEOUT: return "Timeout";
        case MBM_ERR_CRC:     return "CRC Err";
        default:              return "Error";
    }
}

static void monitor_update_card(monitor_entry_t *me)
{
    if (!me || !me->card || !me->active) return;
    char buf[56];
    snprintf(buf, sizeof(buf), "ID:%u %s A:%u Q:%u %s",
             me->slave_id, func_name_short(me->function),
             me->address, me->quantity,
             me->period > 0 ? "Rep" : "Once");
    lv_label_set_text(me->lbl_info, buf);
    if (me->has_response)
        lv_label_set_text_fmt(me->lbl_status, "St: %s", status_str(me->last_status));
    else
        lv_label_set_text(me->lbl_status, "St: -");
}

static void monitor_mbm_callback(uint8_t *response, uint16_t length, mbm_status_t status, uint32_t job_id)
{
    (void)response; (void)length;
    for (int i = 0; i < MONITOR_MAX; i++) {
        if (mon_entries[i].active && mon_entries[i].job_id == job_id) {
            mon_entries[i].last_status  = status;
            mon_entries[i].has_response = true;
            monitor_update_card(&mon_entries[i]);
            if (mon_entries[i].period == 0) {
                /* Once: libera slot para permitir novo envio */
                if (mon_entries[i].card) {
                    lv_obj_delete(mon_entries[i].card);
                    mon_entries[i].card = NULL;
                }
                mon_entries[i].active = false;
                mon_entries[i].job_id = 0;  /* evita match acidental */
            }
            break;
        }
    }
    if (lbl_master_status)
        lv_label_set_text_fmt(lbl_master_status, "Resp: %s", status_str(status));
}

static void mon_edit_confirm_cb(lv_event_t *e)
{
    (void)e;
    monitor_entry_t *me = edit_ctx.me;
    if (!me || !me->active) goto close;

    if (me->period > 0) mbm_cancel_scheduled(me->job_id);
    else                mbm_cancel_job(me->job_id);

    uint16_t fi = lv_dropdown_get_selected(edit_ctx.dd_func);
    if (fi < N_FUNCS) me->function = mbm_funcs[fi].code;

    me->slave_id = (uint8_t) ta_get_int(edit_ctx.ta_slave, 1,   247, me->slave_id);
    me->address  = (uint16_t)ta_get_int(edit_ctx.ta_addr,  0, 65535, me->address);
    me->quantity = (uint16_t)ta_get_int(edit_ctx.ta_qty,   1,   125, me->quantity);

    uint16_t ri = lv_dropdown_get_selected(edit_ctx.dd_repeat);
    me->period  = (ri < N_REPEATS) ? repeat_ms[ri] : 0;
    me->job_id  = mon_job_counter++;

    mbm_request_t req = {0};
    req.slave_id   = me->slave_id;
    req.function   = me->function;
    req.address    = me->address;
    req.quantity   = me->quantity;
    req.timeout_ms = 500;
    req.retries    = 2;
    req.callback   = monitor_mbm_callback;
    req.job_id     = me->job_id;
    req.period_ms  = me->period;

    if (req.period_ms > 0) mbm_schedule(&req);
    else                   mbm_add_request(&req);

    me->has_response = false;
    monitor_update_card(me);
    show_confirm("Request updated.");

close:
    if (edit_ctx.panel) { lv_obj_delete(edit_ctx.panel); edit_ctx.panel = NULL; }
}

static void mon_edit_cancel_cb(lv_event_t *e)
{
    (void)e;
    if (edit_ctx.panel) { lv_obj_delete(edit_ctx.panel); edit_ctx.panel = NULL; }
}

static void mon_gear_cb(lv_event_t *e)
{
    monitor_entry_t *me = (monitor_entry_t *)lv_event_get_user_data(e);
    if (!me || !me->active) return;

    if (edit_ctx.panel) { lv_obj_delete(edit_ctx.panel); edit_ctx.panel = NULL; }
    edit_ctx.me = me;

    lv_obj_t *panel = lv_obj_create(lv_screen_active());
    lv_obj_set_size(panel, 258, 196);
    lv_obj_center(panel);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(panel, 6, 0);
    lv_obj_set_style_pad_gap(panel, 3, 0);
    lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_OFF);
    lv_obj_move_to_index(panel, -1);
    edit_ctx.panel = panel;

    lv_obj_t *title = lv_label_create(panel);
    lv_label_set_text(title, "Edit Request");

    lv_obj_t *row;
    char buf[8];

    row = row_create(panel);
    row_label(row, "Func");
    edit_ctx.dd_func = lv_dropdown_create(row);
    lv_dropdown_set_options(edit_ctx.dd_func,
        "FC03 Rd Hold\nFC04 Rd Input\nFC01 Rd Coils\nFC02 Rd Disc\n"
        "FC06 Wr 1 Reg\nFC16 Wr N Reg\nFC05 Wr 1 Coil\nFC15 Wr N Coil");
    for (unsigned i = 0; i < N_FUNCS; i++)
        if (mbm_funcs[i].code == me->function)
            { lv_dropdown_set_selected(edit_ctx.dd_func, i); break; }
    lv_obj_set_flex_grow(edit_ctx.dd_func, 1);
    lv_obj_set_height(edit_ctx.dd_func, ROW_H);

    row = row_create(panel);
    row_label(row, "Slave");
    snprintf(buf, sizeof(buf), "%u", me->slave_id);
    edit_ctx.ta_slave = num_field_create(row, buf, 3);

    row = row_create(panel);
    row_label(row, "Addr");
    snprintf(buf, sizeof(buf), "%u", me->address);
    edit_ctx.ta_addr = num_field_create(row, buf, 5);

    row = row_create(panel);
    row_label(row, "Qty");
    snprintf(buf, sizeof(buf), "%u", me->quantity);
    edit_ctx.ta_qty = num_field_create(row, buf, 3);

    row = row_create(panel);
    row_label(row, "Repeat");
    edit_ctx.dd_repeat = lv_dropdown_create(row);
    lv_dropdown_set_options(edit_ctx.dd_repeat,
        "Once\n100ms\n200ms\n500ms\n1s\n2s\n5s");
    for (unsigned i = 0; i < N_REPEATS; i++)
        if (repeat_ms[i] == me->period)
            { lv_dropdown_set_selected(edit_ctx.dd_repeat, i); break; }
    lv_obj_set_flex_grow(edit_ctx.dd_repeat, 1);
    lv_obj_set_height(edit_ctx.dd_repeat, ROW_H);

    lv_obj_t *btn_row = lv_obj_create(panel);
    lv_obj_remove_style_all(btn_row);
    lv_obj_set_size(btn_row, lv_pct(100), GUI_BTN_H);
    lv_obj_set_flex_flow(btn_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(btn_row, LV_FLEX_ALIGN_START,
                                    LV_FLEX_ALIGN_CENTER,
                                    LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_gap(btn_row, 4, 0);

    lv_obj_t *l;
    lv_obj_t *btn_ok = lv_button_create(btn_row);
    lv_obj_set_size(btn_ok, 90, GUI_BTN_H);
    l = lv_label_create(btn_ok);
    lv_label_set_text(l, "Confirm");
    lv_obj_center(l);
    lv_obj_add_event_cb(btn_ok, mon_edit_confirm_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *btn_cancel = lv_button_create(btn_row);
    lv_obj_set_size(btn_cancel, 80, GUI_BTN_H);
    lv_obj_set_style_bg_color(btn_cancel, lv_color_hex(0x777777), 0);
    l = lv_label_create(btn_cancel);
    lv_label_set_text(l, "Cancel");
    lv_obj_center(l);
    lv_obj_add_event_cb(btn_cancel, mon_edit_cancel_cb, LV_EVENT_CLICKED, NULL);
}

static void mon_delete_cb(lv_event_t *e)
{
    monitor_entry_t *me = (monitor_entry_t *)lv_event_get_user_data(e);
    if (!me || !me->active) return;

    if (me->period > 0) mbm_cancel_scheduled(me->job_id);
    else                mbm_cancel_job(me->job_id);

    if (me->card) { lv_obj_delete(me->card); me->card = NULL; }
    me->active = false;
    me->job_id = 0;  /* libera slot para novo request */
}

static void monitor_add_entry(const mbm_request_t *req)
{
    int slot = -1;
    for (int i = 0; i < MONITOR_MAX; i++)
        if (!mon_entries[i].active) { slot = i; break; }
    if (slot < 0) return;

    monitor_entry_t *me = &mon_entries[slot];
    me->active       = true;
    me->job_id       = req->job_id;
    me->slave_id     = req->slave_id;
    me->function     = req->function;
    me->address      = req->address;
    me->quantity     = req->quantity;
    me->period       = req->period_ms;
    me->last_status  = MBM_ERR_OK;
    me->has_response = false;

    lv_obj_t *card = lv_obj_create(mon_list);
    lv_obj_set_size(card, lv_pct(100), 50);
    lv_obj_set_flex_flow(card, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(card, LV_FLEX_ALIGN_START,
                                 LV_FLEX_ALIGN_CENTER,
                                 LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(card, 4, 0);
    lv_obj_set_style_pad_gap(card, 4, 0);
    lv_obj_set_style_border_width(card, 1, 0);
    lv_obj_set_style_border_color(card, lv_color_hex(0x555555), 0);
    lv_obj_set_scrollbar_mode(card, LV_SCROLLBAR_MODE_OFF);
    me->card = card;

    lv_obj_t *info = lv_obj_create(card);
    lv_obj_remove_style_all(info);
    lv_obj_set_flex_grow(info, 1);
    lv_obj_set_height(info, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(info, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_gap(info, 1, 0);

    me->lbl_info   = lv_label_create(info);
    me->lbl_status = lv_label_create(info);
    monitor_update_card(me);

    lv_obj_t *l;
    lv_obj_t *btn_gear = lv_button_create(card);
    lv_obj_set_size(btn_gear, 36, 36);
    l = lv_label_create(btn_gear);
    lv_label_set_text(l, LV_SYMBOL_SETTINGS);
    lv_obj_center(l);
    lv_obj_add_event_cb(btn_gear, mon_gear_cb, LV_EVENT_CLICKED, me);

    lv_obj_t *btn_del = lv_button_create(card);
    lv_obj_set_size(btn_del, 36, 36);
    lv_obj_set_style_bg_color(btn_del, lv_color_hex(0xCC3333), 0);
    l = lv_label_create(btn_del);
    lv_label_set_text(l, LV_SYMBOL_CLOSE);
    lv_obj_center(l);
    lv_obj_add_event_cb(btn_del, mon_delete_cb, LV_EVENT_CLICKED, me);
}

static void create_tab_monitor(lv_obj_t *parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, GUI_PAD, 0);
    lv_obj_set_style_pad_gap(parent, 4, 0);
    lv_obj_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scroll_dir(parent, LV_DIR_VER);

    lv_obj_t *l = lv_label_create(parent);
    lv_label_set_text(l, "Active Requests");

    mon_list = lv_obj_create(parent);
    lv_obj_remove_style_all(mon_list);
    lv_obj_set_size(mon_list, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(mon_list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_gap(mon_list, 4, 0);
    lv_obj_set_flex_grow(mon_list, 1);
    lv_obj_set_scrollbar_mode(mon_list, LV_SCROLLBAR_MODE_OFF);

    memset(mon_entries, 0, sizeof(mon_entries));
}

/* ================================================================
 * TAB 4 — LOG
 * ================================================================ */

static void refresh_log_cb(lv_event_t *e)
{
    (void)e;
    if (!list_log) return;
    lv_obj_clean(list_log);

    sniffer_frame_t buf[SNIFFER_LAST_FRAMES_N];
    uint8_t n = 0;
    mb_sniffer_get_last_frames(buf, SNIFFER_LAST_FRAMES_N, &n);

    char line[80];
    for (uint8_t i = 0; i < n; i++) {
        sniffer_frame_t *f = &buf[i];
        int pos = 0;
        pos += snprintf(line + pos, sizeof(line) - (size_t)pos,
                        "%02u:%02u:%02u %s ",
                        f->ts.hour, f->ts.min, f->ts.sec,
                        f->dir == SNIFFER_DIR_RX ? "RX" : "TX");
        for (uint16_t j = 0; j < f->length && (pos + 4) < (int)sizeof(line); j++)
            pos += snprintf(line + pos, sizeof(line) - (size_t)pos,
                            "%02X ", f->data[j]);
        lv_list_add_button(list_log, NULL, line);
    }
}

static void create_tab_log(lv_obj_t *parent)
{
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(parent, GUI_PAD, 0);
    lv_obj_set_style_pad_gap(parent, 4, 0);
    lv_obj_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *btn = lv_button_create(parent);
    lv_obj_set_size(btn, 100, GUI_BTN_H);
    lv_obj_t *l = lv_label_create(btn);
    lv_label_set_text(l, "Refresh");
    lv_obj_center(l);
    lv_obj_add_event_cb(btn, refresh_log_cb, LV_EVENT_CLICKED, NULL);

    list_log = lv_list_create(parent);
    lv_obj_set_flex_grow(list_log, 1);
    lv_obj_set_scrollbar_mode(list_log, LV_SCROLLBAR_MODE_OFF);
}

/* ================================================================
 * PUBLIC ENTRY POINT
 * ================================================================ */

void lvgl_modbus_ui_create(lv_obj_t *parent)
{
    if (!parent) parent = lv_screen_active();

    memset(&edit_ctx,   0, sizeof(edit_ctx));
    memset(mon_entries, 0, sizeof(mon_entries));

    tabview = lv_tabview_create(parent);
    lv_tabview_set_tab_bar_position(tabview, LV_DIR_LEFT);
    lv_tabview_set_tab_bar_size(tabview, TAB_BAR_W);
    lv_obj_set_size(tabview, SCREEN_W, SCREEN_H);
    lv_obj_set_pos(tabview, 0, 0);

    lv_obj_t *tab_bar = lv_tabview_get_tab_bar(tabview);
    lv_obj_set_style_bg_color(tab_bar, lv_color_hex(0x2C3E50), 0);
    lv_obj_set_style_text_color(tab_bar, lv_color_hex(0xECECEC), 0);
    lv_obj_set_scrollbar_mode(tab_bar, LV_SCROLLBAR_MODE_OFF);

    lv_obj_t *t1 = lv_tabview_add_tab(tabview, LV_SYMBOL_SETTINGS);
    lv_obj_t *t2 = lv_tabview_add_tab(tabview, LV_SYMBOL_PLAY);
    lv_obj_t *t3 = lv_tabview_add_tab(tabview, LV_SYMBOL_EYE_OPEN);
    lv_obj_t *t4 = lv_tabview_add_tab(tabview, LV_SYMBOL_LIST);

    uint32_t cnt = lv_tabview_get_tab_count(tabview);
    for (uint32_t i = 0; i < cnt; i++) {
        lv_obj_t *btn = lv_obj_get_child(tab_bar, (int32_t)i);
        lv_obj_set_style_border_side(btn, LV_BORDER_SIDE_RIGHT,
                                     LV_PART_MAIN | LV_STATE_CHECKED);
        lv_obj_set_style_border_width(btn, 3,
                                      LV_PART_MAIN | LV_STATE_CHECKED);
        lv_obj_set_style_border_color(btn, lv_color_hex(0x3498DB),
                                      LV_PART_MAIN | LV_STATE_CHECKED);
    }

    lv_obj_remove_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);

    create_tab_config(t1);
    create_tab_operate(t2);
    create_tab_monitor(t3);
    create_tab_log(t4);

    /* Preview bar above keyboard */
    kb_preview_cont = lv_obj_create(parent);
    lv_obj_set_size(kb_preview_cont, SCREEN_W, KB_PREVIEW_H);
    lv_obj_align(kb_preview_cont, LV_ALIGN_BOTTOM_MID, 0, -(GUI_KB_H));
    lv_obj_set_style_bg_color(kb_preview_cont, lv_color_hex(0xE0E0E0), 0);
    lv_obj_set_style_bg_opa(kb_preview_cont, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(kb_preview_cont, 0, 0);
    lv_obj_set_style_radius(kb_preview_cont, 0, 0);
    lv_obj_set_style_pad_left(kb_preview_cont, 12, 0);
    lv_obj_set_style_pad_ver(kb_preview_cont, 2, 0);
    lv_obj_set_scrollbar_mode(kb_preview_cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_add_flag(kb_preview_cont, LV_OBJ_FLAG_HIDDEN);

    kb_preview_lbl = lv_label_create(kb_preview_cont);
    lv_label_set_text(kb_preview_lbl, "");
    lv_obj_set_style_text_font(kb_preview_lbl, &lv_font_montserrat_14, 0);
    lv_obj_set_style_text_color(kb_preview_lbl, lv_color_hex(0x222222), 0);
    lv_obj_align(kb_preview_lbl, LV_ALIGN_LEFT_MID, 0, 0);

    /* Keyboard */
    kb = lv_keyboard_create(parent);
    lv_keyboard_set_mode(kb, LV_KEYBOARD_MODE_NUMBER);
    lv_keyboard_set_map(kb, LV_KEYBOARD_MODE_NUMBER,
                        custom_kb_num_map, custom_kb_num_ctrl);
    lv_obj_set_size(kb, SCREEN_W, GUI_KB_H);
    lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_to_index(kb, -1);

    lv_obj_add_event_cb(kb, kb_ready_cb,  LV_EVENT_READY,  NULL);
    lv_obj_add_event_cb(kb, kb_cancel_cb, LV_EVENT_CANCEL, NULL);
}
