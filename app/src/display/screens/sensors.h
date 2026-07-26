#ifndef SENSORS_SCREEN_H
#define SENSORS_SCREEN_H

#include <lvgl.h>

#include "../../sensors/baro/bmp581.h"

lv_obj_t *sensors_create(lv_obj_t *parent);
void sensors_baro_update(const struct baro_data *baro);

#endif 
