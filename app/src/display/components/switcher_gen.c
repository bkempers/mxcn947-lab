/**
 * @file switcher_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "switcher_gen.h"
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

lv_obj_t * switcher_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if DISPLAY_CHECK_COMPILE_TARGET(DISPLAY_TARGET_ALL)
    if (display_check_target(DISPLAY_TARGET_ALL)) {
        lv_obj_t * lv_obj_0 = lv_obj_create(parent);
        lv_obj_set_name_static(lv_obj_0, "switcher_#");
        lv_obj_set_width(lv_obj_0, lv_pct(100));
        lv_obj_set_height(lv_obj_0, 40);
        lv_obj_set_flex_flow(lv_obj_0, LV_FLEX_FLOW_ROW);
        lv_obj_set_style_flex_main_place(lv_obj_0, LV_FLEX_ALIGN_SPACE_EVENLY, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_0, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_pad_all(lv_obj_0, 0, 0);
        lv_obj_set_style_pad_top(lv_obj_0, 4, 0);
        lv_obj_set_style_pad_bottom(lv_obj_0, 4, 0);

        lv_obj_add_style(lv_obj_0, &navbar, 0);
        lv_obj_t * lv_button_0 = lv_button_create(lv_obj_0);
        lv_obj_set_width(lv_button_0, 36);
        lv_obj_set_height(lv_button_0, 30);
        lv_obj_set_style_bg_opa(lv_button_0, 0, 0);
        lv_obj_set_style_border_width(lv_button_0, 0, 0);
        lv_obj_set_style_bg_color(lv_button_0, ACCENT, LV_STATE_CHECKED);
        lv_obj_set_style_bg_opa(lv_button_0, 40, LV_STATE_CHECKED);
        lv_obj_set_style_radius(lv_button_0, 8, 0);
        lv_obj_t * lv_label_0 = lv_label_create(lv_button_0);
        lv_label_set_text(lv_label_0, "");
        lv_obj_set_align(lv_label_0, LV_ALIGN_CENTER);
        lv_obj_add_style(lv_label_0, &icon, 0);

        lv_obj_bind_state_if_eq(lv_button_0, &active_screen, LV_STATE_CHECKED, 0);
        lv_obj_add_subject_set_int_event(lv_button_0, &active_screen, LV_EVENT_CLICKED, 0);

        lv_obj_t * lv_button_1 = lv_button_create(lv_obj_0);
        lv_obj_set_width(lv_button_1, 36);
        lv_obj_set_height(lv_button_1, 30);
        lv_obj_set_style_bg_opa(lv_button_1, 0, 0);
        lv_obj_set_style_border_width(lv_button_1, 0, 0);
        lv_obj_set_style_bg_color(lv_button_1, ACCENT, LV_STATE_CHECKED);
        lv_obj_set_style_bg_opa(lv_button_1, 40, LV_STATE_CHECKED);
        lv_obj_set_style_radius(lv_button_1, 8, 0);
        lv_obj_t * lv_label_1 = lv_label_create(lv_button_1);
        lv_label_set_text(lv_label_1, "");
        lv_obj_set_align(lv_label_1, LV_ALIGN_CENTER);
        lv_obj_add_style(lv_label_1, &icon, 0);

        lv_obj_bind_state_if_eq(lv_button_1, &active_screen, LV_STATE_CHECKED, 1);
        lv_obj_add_subject_set_int_event(lv_button_1, &active_screen, LV_EVENT_CLICKED, 1);

        lv_obj_t * lv_button_2 = lv_button_create(lv_obj_0);
        lv_obj_set_width(lv_button_2, 36);
        lv_obj_set_height(lv_button_2, 30);
        lv_obj_set_style_bg_opa(lv_button_2, 0, 0);
        lv_obj_set_style_border_width(lv_button_2, 0, 0);
        lv_obj_set_style_bg_color(lv_button_2, ACCENT, LV_STATE_CHECKED);
        lv_obj_set_style_bg_opa(lv_button_2, 40, LV_STATE_CHECKED);
        lv_obj_set_style_radius(lv_button_2, 8, 0);
        lv_obj_t * lv_label_2 = lv_label_create(lv_button_2);
        lv_label_set_text(lv_label_2, "");
        lv_obj_set_align(lv_label_2, LV_ALIGN_CENTER);
        lv_obj_add_style(lv_label_2, &icon, 0);

        lv_obj_bind_state_if_eq(lv_button_2, &active_screen, LV_STATE_CHECKED, 2);
        lv_obj_add_subject_set_int_event(lv_button_2, &active_screen, LV_EVENT_CLICKED, 2);

        lv_obj_t * lv_button_3 = lv_button_create(lv_obj_0);
        lv_obj_set_width(lv_button_3, 36);
        lv_obj_set_height(lv_button_3, 30);
        lv_obj_set_style_bg_opa(lv_button_3, 0, 0);
        lv_obj_set_style_border_width(lv_button_3, 0, 0);
        lv_obj_set_style_bg_color(lv_button_3, ACCENT, LV_STATE_CHECKED);
        lv_obj_set_style_bg_opa(lv_button_3, 40, LV_STATE_CHECKED);
        lv_obj_set_style_radius(lv_button_3, 8, 0);
        lv_obj_t * lv_label_3 = lv_label_create(lv_button_3);
        lv_label_set_text(lv_label_3, "");
        lv_obj_set_align(lv_label_3, LV_ALIGN_CENTER);
        lv_obj_add_style(lv_label_3, &icon, 0);

        lv_obj_bind_state_if_eq(lv_button_3, &active_screen, LV_STATE_CHECKED, 3);
        lv_obj_add_subject_set_int_event(lv_button_3, &active_screen, LV_EVENT_CLICKED, 3);

        the_root = lv_obj_0;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

