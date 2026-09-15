/**
 * @file nav_item_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "nav_item_gen.h"
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

lv_obj_t * nav_item_create(lv_obj_t * parent, const char * icon_text, const char * label_text)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if DISPLAY_CHECK_COMPILE_TARGET(DISPLAY_TARGET_ALL)
    if (display_check_target(DISPLAY_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(parent);
        lv_obj_set_name_static(lv_obj_0, "nav_item_#");
        lv_obj_set_height(lv_obj_0, 56);
        lv_obj_set_style_flex_grow(lv_obj_0, 1, 0);
        lv_obj_set_flex_flow(lv_obj_0, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_cross_place(lv_obj_0, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_flex_main_place(lv_obj_0, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_pad_top(lv_obj_0, 6, 0);
        lv_obj_set_style_pad_bottom(lv_obj_0, 4, 0);
        lv_obj_set_style_pad_left(lv_obj_0, 2, 0);
        lv_obj_set_style_pad_right(lv_obj_0, 2, 0);
        lv_obj_set_style_pad_row(lv_obj_0, 2, 0);
        lv_obj_set_style_radius(lv_obj_0, 10, 0);
        lv_obj_set_style_bg_color(lv_obj_0, PILLBG, 0);
        lv_obj_set_style_bg_opa(lv_obj_0, 255, 0);
        lv_obj_set_style_border_width(lv_obj_0, 1, 0);
        lv_obj_set_style_border_color(lv_obj_0, BORDER, 0);
        lv_obj_set_style_bg_color(lv_obj_0, ACCENT, LV_STATE_CHECKED);
        lv_obj_set_style_bg_opa(lv_obj_0, 40, LV_STATE_CHECKED);
        lv_obj_set_style_border_color(lv_obj_0, ACCENT, LV_STATE_CHECKED);

        lv_obj_t * lv_label_0 = lv_label_create(lv_obj_0);
        lv_label_set_text(lv_label_0, icon_text);
        lv_obj_set_align(lv_label_0, LV_ALIGN_CENTER);
        lv_obj_add_style(lv_label_0, &icon, 0);

        lv_obj_t * lv_label_1 = lv_label_create(lv_obj_0);
        lv_label_set_text(lv_label_1, label_text);
        lv_obj_set_align(lv_label_1, LV_ALIGN_CENTER);
        lv_obj_set_style_text_align(lv_label_1, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_width(lv_label_1, lv_pct(100));
        lv_obj_add_style(lv_label_1, &label_xs, 0);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

