#include "sensors.h"
#include "misc/lv_area.h"
#include "../theme/lvgl_theme.h"
#include "../widgets/lvgl_widgets.c"

static lv_obj_t *lbl_temp;
static lv_obj_t *lbl_press;
static lv_obj_t *lbl_alt;

void sensors_create(lv_obj_t *parent) 
{
    lv_obj_add_style(parent, &ui_st_screen, 0);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);

    lbl_temp  = stat_card(parent, "TEMPERATURE", "F");
    lbl_press = stat_card(parent, "PRESSURE", "hPa");
    lbl_alt   = stat_card(parent, "ALTITUDE", "m");
}

void sensors_baro_update(const struct baro_data *baro) 
{
    lv_label_set_text_fmt(lbl_temp, "%.2f", (double)celcius_to_farhenheit(baro->temp_c));
    lv_label_set_text_fmt(lbl_press, "%.1f", (double)baro->pressure_hpa);
    lv_label_set_text_fmt(lbl_alt, "%.2f", (double)baro->altitude_m);
}
