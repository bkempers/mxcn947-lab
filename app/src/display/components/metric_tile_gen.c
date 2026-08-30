/**
 * @file metric_tile_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "metric_tile_gen.h"
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

lv_obj_t * metric_tile_create(lv_obj_t * parent, const char * caption, lv_subject_t * value_subject, const char * unit_str)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if DISPLAY_CHECK_COMPILE_TARGET(DISPLAY_TARGET_ALL)
    if (display_check_target(DISPLAY_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(parent);
        lv_obj_set_name_static(lv_obj_0, "metric_tile_#");
        lv_obj_set_height(lv_obj_0, LV_SIZE_CONTENT);
        lv_obj_set_style_flex_grow(lv_obj_0, 1, 0);
        lv_obj_set_flex_flow(lv_obj_0, LV_FLEX_FLOW_COLUMN);

        lv_obj_add_style(lv_obj_0, &tile, 0);
        lv_obj_t * lv_label_0 = lv_label_create(lv_obj_0);
        lv_label_set_text(lv_label_0, caption);
        lv_obj_add_style(lv_label_0, &label_xs, 0);

        lv_obj_t * lv_obj_1 = lv_obj_create(lv_obj_0);
        lv_obj_set_width(lv_obj_1, lv_pct(100));
        lv_obj_set_height(lv_obj_1, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(lv_obj_1, LV_FLEX_FLOW_ROW);
        lv_obj_set_style_flex_cross_place(lv_obj_1, LV_FLEX_ALIGN_END, 0);
        lv_obj_set_style_pad_column(lv_obj_1, 3, 0);
        lv_obj_set_style_bg_opa(lv_obj_1, 0, 0);
        lv_obj_set_style_border_width(lv_obj_1, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_1, 0, 0);
        lv_obj_t * lv_label_1 = lv_label_create(lv_obj_1);
        lv_label_bind_text(lv_label_1, value_subject, NULL);
        lv_obj_add_style(lv_label_1, &value, 0);

        lv_obj_t * lv_label_2 = lv_label_create(lv_obj_1);
        lv_label_set_text(lv_label_2, unit_str);
        lv_obj_add_style(lv_label_2, &unit, 0);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

