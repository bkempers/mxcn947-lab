/**
 * @file settings_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "settings_gen.h"
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

lv_obj_t * settings_create(void)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if DISPLAY_CHECK_COMPILE_TARGET(DISPLAY_TARGET_ALL)
    if (display_check_target(DISPLAY_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(NULL);
        lv_obj_set_name_static(lv_obj_0, "settings_#");
        lv_obj_set_flex_flow(lv_obj_0, LV_FLEX_FLOW_COLUMN);

        lv_obj_add_style(lv_obj_0, &page, 0);
        lv_obj_t * lv_obj_1 = lv_obj_create(lv_obj_0);
        lv_obj_set_flex_grow(lv_obj_1, 1);
        lv_obj_set_width(lv_obj_1, lv_pct(100));
        lv_obj_set_flex_flow(lv_obj_1, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_cross_place(lv_obj_1, LV_FLEX_ALIGN_START, 0);
        lv_obj_add_style(lv_obj_1, &content, 0);
        lv_obj_t * lv_label_0 = lv_label_create(lv_obj_1);
        lv_label_set_text(lv_label_0, "Settings");
        lv_obj_set_style_pad_bottom(lv_label_0, 4, 0);
        lv_obj_add_style(lv_label_0, &title, 0);

        lv_obj_t * lv_obj_2 = lv_obj_create(lv_obj_1);
        lv_obj_set_width(lv_obj_2, lv_pct(100));
        lv_obj_set_height(lv_obj_2, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(lv_obj_2, LV_FLEX_FLOW_COLUMN);
        lv_obj_add_style(lv_obj_2, &card_style, 0);
        lv_obj_t * lv_label_1 = lv_label_create(lv_obj_2);
        lv_label_set_text(lv_label_1, "UPTIME");
        lv_obj_add_style(lv_label_1, &label_xs, 0);

        lv_obj_t * lv_label_2 = lv_label_create(lv_obj_2);
        lv_label_bind_text(lv_label_2, &uptime_str, NULL);
        lv_obj_set_style_pad_top(lv_label_2, 2, 0);
        lv_obj_add_style(lv_label_2, &hero, 0);

        lv_obj_t * lv_obj_3 = lv_obj_create(lv_obj_1);
        lv_obj_set_flex_flow(lv_obj_3, LV_FLEX_FLOW_COLUMN);
        lv_obj_add_style(lv_obj_3, &card_style, 0);
        info_row_create(lv_obj_3, "Board", &board_str);

        info_row_create(lv_obj_3, "Zephyr", &zephyr_str);

        info_row_create(lv_obj_3, "Firmware", &firmware_str);

        info_row_create(lv_obj_3, "Built", &built_str);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

