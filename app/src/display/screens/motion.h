#ifndef MOTION_H
#define MOTION_H

#include <lvgl.h>

void motion_ui_init(lv_obj_t *screen);
void motion_update(int heading, int roll, int pitch);

#endif
