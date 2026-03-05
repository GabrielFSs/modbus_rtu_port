#ifndef LVGL_UI_H
#define LVGL_UI_H

#include "lvgl.h"

/** Cria o tabview e todas as abas (Modo, Barramento, Master, Slave, Log).
 *  Chamar após lv_init() e display/input drivers registados.
 *  parent: contêiner (ex.: lv_scr_act()) ou NULL para usar screen actual. */
void lvgl_modbus_ui_create(lv_obj_t *parent);

#endif
