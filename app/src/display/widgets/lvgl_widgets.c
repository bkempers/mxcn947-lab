#include <lvgl.h>

#include "../theme/lvgl_theme.h"

static lv_obj_t *stat_card(lv_obj_t *parent, const char *caption, const char *unit)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_add_style(card, &ui_st_card, 0);
    lv_obj_set_scrollbar_mode(card, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(card, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(card, LV_FLEX_ALIGN_START,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(card, 10, 0);

    lv_obj_t *bar = lv_obj_create(card);   /* accent stripe */
    lv_obj_add_style(bar, &ui_st_accentbar, 0);
    lv_obj_set_style_border_width(bar, 0, 0);

    lv_obj_t *col = lv_obj_create(card);   /* text column, transparent */
    lv_obj_remove_style_all(col);
    lv_obj_set_flex_flow(col, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_width(col, lv_pct(100));
    lv_obj_set_height(col, LV_SIZE_CONTENT);

    lv_obj_t *cap = lv_label_create(col);
    lv_obj_add_style(cap, &ui_st_caption, 0);
    lv_label_set_text(cap, caption);

    lv_obj_t *vrow = lv_obj_create(col);   /* value + unit, baseline-ish */
    lv_obj_remove_style_all(vrow);
    lv_obj_set_flex_flow(vrow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(vrow, LV_FLEX_ALIGN_START,
                          LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_set_style_pad_column(vrow, 4, 0);
    lv_obj_set_width(vrow, lv_pct(100));
    lv_obj_set_height(vrow, LV_SIZE_CONTENT);

    lv_obj_t *val = lv_label_create(vrow);
    lv_obj_add_style(val, &ui_st_value, 0);
    lv_label_set_text(val, "--");

    lv_obj_t *u = lv_label_create(vrow);
    lv_obj_add_style(u, &ui_st_unit, 0);
    lv_label_set_text(u, unit);

    return val;   /* screens keep this to update live */
}
