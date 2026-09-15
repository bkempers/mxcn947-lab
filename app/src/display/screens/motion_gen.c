/**
 * @file motion_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "motion_gen.h"
#include "../display.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/***********************
 *  STATIC VARIABLES
 **********************/

/***********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_obj_t * motion_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if DISPLAY_CHECK_COMPILE_TARGET(DISPLAY_TARGET_ALL)
    if (display_check_target(DISPLAY_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "motion_#");
        lv_obj_set_flex_flow(lv_obj_0, LV_FLEX_FLOW_COLUMN);

        lv_obj_add_style(lv_obj_0, &page, 0);
        lv_obj_t * lv_obj_1 = lv_obj_create(lv_obj_0);
        lv_obj_set_flex_grow(lv_obj_1, 1);
        lv_obj_set_width(lv_obj_1, lv_pct(100));
        lv_obj_set_flex_flow(lv_obj_1, LV_FLEX_FLOW_COLUMN);
        lv_obj_add_style(lv_obj_1, &content, 0);
        lv_obj_t * lv_obj_2 = lv_obj_create(lv_obj_1);
        lv_obj_set_width(lv_obj_2, lv_pct(100));
        lv_obj_set_height(lv_obj_2, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(lv_obj_2, LV_FLEX_FLOW_ROW);
        lv_obj_set_style_flex_main_place(lv_obj_2, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_2, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_bg_opa(lv_obj_2, 0, 0);
        lv_obj_set_style_border_width(lv_obj_2, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_2, 0, 0);
        lv_obj_set_style_pad_bottom(lv_obj_2, 4, 0);
        lv_obj_t * lv_label_0 = lv_label_create(lv_obj_2);
        lv_label_set_text(lv_label_0, "Motion");
        lv_obj_add_style(lv_label_0, &title, 0);

        lv_obj_t * lv_obj_3 = lv_obj_create(lv_obj_2);
        lv_obj_add_style(lv_obj_3, &dot, 0);

        lv_obj_t * lv_obj_4 = lv_obj_create(lv_obj_1);
        lv_obj_set_width(lv_obj_4, lv_pct(100));
        lv_obj_set_height(lv_obj_4, LV_SIZE_CONTENT);
        lv_obj_set_style_pad_all(lv_obj_4, 8, 0);
        lv_obj_set_flex_flow(lv_obj_4, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_cross_place(lv_obj_4, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_add_style(lv_obj_4, &card_style, 0);
        lv_obj_t * compass = lv_scale_create(lv_obj_4);
        lv_obj_set_name(compass, "compass");
        lv_obj_set_width(compass, 98);
        lv_obj_set_height(compass, 98);
        lv_scale_set_mode(compass, LV_SCALE_MODE_ROUND_INNER);
        lv_scale_set_rotation(compass, 270);
        lv_scale_set_angle_range(compass, 360);
        lv_scale_set_min_value(compass, 0);
        lv_scale_set_max_value(compass, 360);
        lv_scale_set_total_tick_count(compass, 37);
        lv_scale_set_major_tick_every(compass, 9);
        lv_scale_set_label_show(compass, false);
        lv_obj_set_style_line_width(compass, 2, 0);
        lv_obj_set_style_line_color(compass, BORDER, 0);
        lv_obj_set_style_length(compass, 6, 0);
        lv_obj_t * needle = lv_line_create(compass);
        lv_obj_set_name(needle, "needle");
        lv_obj_set_style_line_width(needle, 3, 0);
        lv_obj_set_style_line_color(needle, ACCENT, 0);
        lv_obj_set_style_line_rounded(needle, true, 0);

        lv_obj_t * lv_label_1 = lv_label_create(compass);
        lv_label_bind_text(lv_label_1, &heading_str, NULL);
        lv_obj_set_align(lv_label_1, LV_ALIGN_CENTER);
        lv_obj_add_style(lv_label_1, &value, 0);

        lv_obj_t * lv_obj_5 = lv_obj_create(lv_obj_1);
        lv_obj_set_width(lv_obj_5, lv_pct(100));
        lv_obj_set_height(lv_obj_5, LV_SIZE_CONTENT);
        lv_obj_set_style_pad_all(lv_obj_5, 8, 0);
        lv_obj_set_flex_flow(lv_obj_5, LV_FLEX_FLOW_ROW);
        lv_obj_set_style_flex_cross_place(lv_obj_5, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_pad_column(lv_obj_5, 12, 0);
        lv_obj_add_style(lv_obj_5, &card_style, 0);
        lv_obj_t * horizon_slot = lv_obj_create(lv_obj_5);
        lv_obj_set_name(horizon_slot, "horizon_slot");
        lv_obj_set_width(horizon_slot, 84);
        lv_obj_set_height(horizon_slot, 84);
        lv_obj_set_style_radius(horizon_slot, 42, 0);
        lv_obj_set_style_clip_corner(horizon_slot, true, 0);
        lv_obj_set_style_border_width(horizon_slot, 1, 0);
        lv_obj_set_style_border_color(horizon_slot, BORDER, 0);
        lv_obj_set_style_bg_color(horizon_slot, SKY, 0);
        lv_obj_set_style_pad_all(horizon_slot, 0, 0);
        lv_obj_t * lv_obj_6 = lv_obj_create(horizon_slot);
        lv_obj_set_width(lv_obj_6, 32);
        lv_obj_set_height(lv_obj_6, 2);
        lv_obj_set_align(lv_obj_6, LV_ALIGN_CENTER);
        lv_obj_set_style_bg_color(lv_obj_6, TEXT, 0);
        lv_obj_set_style_border_width(lv_obj_6, 0, 0);
        lv_obj_set_style_radius(lv_obj_6, 0, 0);

        lv_obj_t * lv_obj_7 = lv_obj_create(lv_obj_5);
        lv_obj_set_flex_grow(lv_obj_7, 1);
        lv_obj_set_height(lv_obj_7, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(lv_obj_7, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_bg_opa(lv_obj_7, 0, 0);
        lv_obj_set_style_border_width(lv_obj_7, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_7, 0, 0);
        lv_obj_t * lv_label_2 = lv_label_create(lv_obj_7);
        lv_label_set_text(lv_label_2, "ROLL");
        lv_obj_add_style(lv_label_2, &label_xs, 0);

        lv_obj_t * lv_label_3 = lv_label_create(lv_obj_7);
        lv_label_bind_text(lv_label_3, &roll_str, NULL);
        lv_obj_add_style(lv_label_3, &value, 0);

        lv_obj_t * lv_label_4 = lv_label_create(lv_obj_7);
        lv_label_set_text(lv_label_4, "PITCH");
        lv_obj_set_style_pad_top(lv_label_4, 6, 0);
        lv_obj_add_style(lv_label_4, &label_xs, 0);

        lv_obj_t * lv_label_5 = lv_label_create(lv_obj_7);
        lv_label_bind_text(lv_label_5, &pitch_str, NULL);
        lv_obj_add_style(lv_label_5, &value, 0);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

