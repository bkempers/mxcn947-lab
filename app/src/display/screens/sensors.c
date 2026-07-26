#include "sensors.h"
#include "misc/lv_area.h"

static lv_obj_t *lbl_temp;
static lv_obj_t *lbl_press;
static lv_obj_t *lbl_alt;

lv_obj_t *sensors_create(lv_obj_t *parent) {
    lv_obj_t *root = lv_obj_create(parent);
    lv_obj_set_size(root, LV_PCT(100), LV_PCT(100));
    lbl_temp  = lv_label_create(root);
    lbl_press = lv_label_create(root);
    lbl_alt = lv_label_create(root);

    /* layout with flex/grid, apply theme styles... */
    lv_obj_align(lbl_temp, LV_ALIGN_CENTER, 0, -20);
    lv_obj_align(lbl_press, LV_ALIGN_CENTER, 0, 20);
    lv_obj_align(lbl_alt, LV_ALIGN_CENTER, 0, 40);

    return root;
}

void sensors_baro_update(const struct baro_data *baro) {
    lv_label_set_text_fmt(lbl_temp, "%.2f C", (double)baro->temp_c);
    lv_label_set_text_fmt(lbl_press, "%.1f hPa", (double)baro->pressure_hpa);
    lv_label_set_text_fmt(lbl_alt, "%f m", (double)baro->altitude_m);
}
