/**
 * @file nav_overlay_gen.c
 * @brief Template source file for LVGL objects
 */

/*********************
 *      INCLUDES
 *********************/

#include "nav_overlay_gen.h"
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

lv_obj_t * nav_overlay_create(lv_obj_t * parent)
{
    LV_TRACE_OBJ_CREATE("begin");


    lv_obj_t * the_root = NULL;

    #if DISPLAY_CHECK_COMPILE_TARGET(DISPLAY_TARGET_ALL)
    if (display_check_target(DISPLAY_TARGET_ALL)) {
        lv_obj_t * nav_backdrop = lv_obj_create(parent);
        lv_obj_set_name_static(nav_backdrop, "nav_overlay_#");
        lv_obj_set_name(nav_backdrop, "nav_backdrop");
        lv_obj_set_width(nav_backdrop, 320);
        lv_obj_set_height(nav_backdrop, 480);
        lv_obj_set_style_bg_color(nav_backdrop, lv_color_hex(0x000000), 0);
        lv_obj_set_style_bg_opa(nav_backdrop, 140, 0);
        lv_obj_set_style_border_width(nav_backdrop, 0, 0);
        lv_obj_set_style_radius(nav_backdrop, 0, 0);
        lv_obj_set_style_pad_all(nav_backdrop, 0, 0);

        lv_obj_t * nav_card = lv_obj_create(nav_backdrop);
        lv_obj_set_name(nav_card, "nav_card");
        lv_obj_set_width(nav_card, 280);
        lv_obj_set_height(nav_card, LV_SIZE_CONTENT);
        lv_obj_set_align(nav_card, LV_ALIGN_CENTER);
        lv_obj_set_flex_flow(nav_card, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_pad_all(nav_card, 16, 0);
        lv_obj_set_style_pad_row(nav_card, 8, 0);
        lv_obj_add_style(nav_card, &card_style, 0);
        lv_obj_t * lv_obj_0 = lv_obj_create(nav_card);
        lv_obj_set_width(lv_obj_0, lv_pct(100));
        lv_obj_set_height(lv_obj_0, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(lv_obj_0, LV_FLEX_FLOW_ROW);
        lv_obj_set_style_flex_main_place(lv_obj_0, LV_FLEX_ALIGN_SPACE_BETWEEN, 0);
        lv_obj_set_style_flex_cross_place(lv_obj_0, LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_bg_opa(lv_obj_0, 0, 0);
        lv_obj_set_style_border_width(lv_obj_0, 0, 0);
        lv_obj_set_style_pad_all(lv_obj_0, 0, 0);
        lv_obj_t * lv_label_0 = lv_label_create(lv_obj_0);
        lv_label_set_text(lv_label_0, "Navigate");
        lv_obj_add_style(lv_label_0, &title, 0);

        lv_obj_t * nav_close = lv_button_create(lv_obj_0);
        lv_obj_set_name(nav_close, "nav_close");
        lv_obj_set_width(nav_close, 32);
        lv_obj_set_height(nav_close, 32);
        lv_obj_set_style_bg_opa(nav_close, 0, 0);
        lv_obj_set_style_border_width(nav_close, 0, 0);
        lv_obj_t * lv_label_1 = lv_label_create(nav_close);
        lv_label_set_text(lv_label_1, "");
        lv_obj_set_align(lv_label_1, LV_ALIGN_CENTER);
        lv_obj_add_style(lv_label_1, &icon, 0);

        lv_obj_t * nav_grid = lv_obj_create(nav_card);
        lv_obj_set_name(nav_grid, "nav_grid");
        lv_obj_set_width(nav_grid, lv_pct(100));
        lv_obj_set_height(nav_grid, LV_SIZE_CONTENT);
        lv_obj_set_flex_flow(nav_grid, LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_pad_row(nav_grid, 8, 0);
        lv_obj_set_style_bg_opa(nav_grid, 0, 0);
        lv_obj_set_style_border_width(nav_grid, 0, 0);
        lv_obj_set_style_pad_all(nav_grid, 0, 0);

        the_root = nav_backdrop;
    }
    #endif

    LV_TRACE_OBJ_CREATE("finished");

    return the_root;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

