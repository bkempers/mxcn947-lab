#ifndef LVGL_MAIN_H
#define LVGL_MAIN_H

int lvgl_ui_init(void);
void lvgl_ui_update(int temp_c, int temp_frac, int press_kpa, int press_frac);
void lvgl_ui_tick(void);

#endif
