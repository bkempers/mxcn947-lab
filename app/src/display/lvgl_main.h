#ifndef LVGL_MAIN_H
#define LVGL_MAIN_H

#include <lvgl.h>

int lvgl_display_init(void);
void display_tick(void);
void ui_request_screen(int idx);

#endif
