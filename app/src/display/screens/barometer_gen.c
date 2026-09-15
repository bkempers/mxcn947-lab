/**
 * @file barometer_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "barometer_gen.h"
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

lv_obj_t * barometer_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if DISPLAY_CHECK_COMPILE_TARGET(DISPLAY_TARGET_ALL)
    if (display_check_target(DISPLAY_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "barometer_#");
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
        lv_label_set_text(lv_label_0, "Barometer");
        lv_obj_add_style(lv_label_0, &title, 0);

        lv_obj_t * lv_obj_3 = lv_obj_create(lv_obj_2);
        lv_obj_add_style(lv_obj_3, &dot, 0);

        lv_obj_t * lv_obj_4 = lv_obj_create(lv_obj_1);
        lv_obj_set_width(lv_obj_4, lv_pct(100));
        lv_obj_set_height(lv_obj_4, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(lv_obj_4, LV_FLEX_FLOW_COLUMN);
        lv_obj_add_style(lv_obj_4, &card_style, 0);
        lv_obj_t * lv_label_1 = lv_label_create(lv_obj_4);
        lv_label_set_text(lv_label_1, "ALTITUDE");
        lv_obj_add_style(lv_label_1, &label_xs, 0);

        lv_obj_t * lv_obj_5 = lv_obj_create(lv_obj_4);
        lv_obj_set_width(lv_obj_5, lv_pct(100));
        lv_obj_set_height(lv_obj_5, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(lv_obj_5, LV_FLEX_FLOW_ROW);
        lv_obj_set_style_flex_cross_place(lv_obj_5, LV_FLEX_ALIGN_END, 0);
        lv_obj_set_style_pad_column(lv_obj_5, 6, 0);
        lv_obj_set_style_bg_opa(lv_obj_5, 0, 0);
        lv_obj_set_style_border_width(lv_obj_5, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_5, 0, 0);
        lv_obj_set_style_pad_top(lv_obj_5, 4, 0);
        lv_obj_t * lv_label_2 = lv_label_create(lv_obj_5);
        lv_label_bind_text(lv_label_2, &alt_str, NULL);
        lv_obj_add_style(lv_label_2, &hero, 0);

        lv_obj_t * lv_label_3 = lv_label_create(lv_obj_5);
        lv_label_bind_text(lv_label_3, &alt_unit, NULL);
        lv_obj_set_style_pad_bottom(lv_label_3, 6, 0);
        lv_obj_add_style(lv_label_3, &unit, 0);

        lv_obj_t * lv_obj_6 = lv_obj_create(lv_obj_1);
        lv_obj_set_width(lv_obj_6, lv_pct(100));
        lv_obj_set_height(lv_obj_6, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(lv_obj_6, LV_FLEX_FLOW_ROW);
        lv_obj_set_style_pad_column(lv_obj_6, 8, 0);
        lv_obj_set_style_bg_opa(lv_obj_6, 0, 0);
        lv_obj_set_style_border_width(lv_obj_6, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_6, 0, 0);
        metric_tile_create(lv_obj_6, "TEMP", &temp_str, "F");

        metric_tile_create(lv_obj_6, "PRESSURE", &press_str, "hPa");

        switcher_create(lv_obj_0);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

