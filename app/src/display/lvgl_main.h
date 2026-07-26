#ifndef LVGL_MAIN_H
#define LVGL_MAIN_H

#include <lvgl.h>

int lvgl_display_init(void);
void display_update(int temp_c, int temp_frac, int press_kpa, int press_frac);
void display_tick(void);

lv_obj_t *home_create(lv_obj_t *parent);

#endif
