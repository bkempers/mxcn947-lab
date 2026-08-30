/**
 * @file display_gen.h
 */

#ifndef LVGL_PRO_DISPLAY_GEN_H
#define LVGL_PRO_DISPLAY_GEN_H

#ifndef UI_SUBJECT_STRING_LENGTH
#define UI_SUBJECT_STRING_LENGTH 256
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
    #include "lvgl_private.h"
#else
    #include "lvgl/lvgl.h"
    #include "lvgl/lvgl_private.h"
#endif

#if defined(LV_USE_XML) && LV_USE_XML
    #include "lv_xml/lv_xml.h"
#endif



/* Prototypes for target functions, needed by responsive const definitions */

void display_set_target(uint32_t target);
uint32_t display_get_target(void);
bool display_check_target(uint32_t target);

/*********************
 *      DEFINES
 *********************/

#define DISPLAY_TARGET_UNDEFINED     (0 << 1)
#define DISPLAY_TARGET_FRDM_MCXN947  (1 << 1)
#define DISPLAY_TARGET_ALL           0x0FFFFFFF

/* By default compile for all targets, allowing to switch to any targets at runtime */
#ifndef DISPLAY_COMPILE_TARGET
#define DISPLAY_COMPILE_TARGET DISPLAY_TARGET_ALL
#endif

#define DISPLAY_CHECK_COMPILE_TARGET(target) (DISPLAY_COMPILE_TARGET & (target) ? 1 : 0)

#define BG lv_color_hex(0xEEF1F5)
#define CARD_COLOR lv_color_hex(0xFFFFFF)
#define BORDER lv_color_hex(0xE4E7EC)
#define TEXT lv_color_hex(0x1A1D22)
#define DIM lv_color_hex(0x667085)
#define ACCENT lv_color_hex(0x2E5BFF)
#define PILLBG lv_color_hex(0xEDF0F3)
#define DOCK_COLOR lv_color_hex(0x1A1D22)
#define DOCKDIM lv_color_hex(0x98A2B3)
#define SKY lv_color_hex(0xAFD4EE)
#define GROUND lv_color_hex(0xC9AD86)


#ifndef LV_XML_EVAL_STRING_BUF_SIZE
    #define LV_XML_EVAL_STRING_BUF_SIZE 256
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL VARIABLES
 **********************/

/*-------------------
 * Permanent screens
 *------------------*/

/*----------------
 * Global styles
 *----------------*/

extern lv_style_t page;
extern lv_style_t card_style;
extern lv_style_t tile;
extern lv_style_t title;
extern lv_style_t hero;
extern lv_style_t value;
extern lv_style_t value_sm;
extern lv_style_t label_xs;
extern lv_style_t unit;
extern lv_style_t pill;
extern lv_style_t dot;
extern lv_style_t dock_style;
extern lv_style_t icon;
extern lv_style_t navbar;
extern lv_style_t content;

/*----------------
 * Fonts
 *----------------*/

/* Targets: any */
extern lv_font_t * jetbrains_12;
extern lv_font_t * jetbrains_14;
extern lv_font_t * jetbrains_bold_18;
extern lv_font_t * jetbrains_bold_40;
extern lv_font_t * icons_24;


/*----------------
 * Images
 *----------------*/



/*----------------
 * Subjects
 *----------------*/

extern lv_subject_t active_screen;
extern lv_subject_t empty_str;
extern lv_subject_t board_str;
extern lv_subject_t zephyr_str;
extern lv_subject_t firmware_str;
extern lv_subject_t built_str;
extern lv_subject_t uptime_str;
extern lv_subject_t temp_str;
extern lv_subject_t press_str;
extern lv_subject_t alt_str;
extern lv_subject_t alt_unit;
extern lv_subject_t press_trend;
extern lv_subject_t heading_deg;
extern lv_subject_t heading_str;
extern lv_subject_t roll_deg;
extern lv_subject_t pitch_deg;
extern lv_subject_t roll_str;
extern lv_subject_t pitch_str;

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/*----------------
 * Event Callbacks
 *----------------*/

/**
 * Initialize the component library
 */

void display_init_gen(const char * asset_path);

/**********************
 *      MACROS
 **********************/

/**********************
 *   POST INCLUDES
 **********************/

/*Include all the widgets, components and screens of this library*/
#include "components/info_row_gen.h"
#include "components/metric_tile_gen.h"
#include "components/switcher_gen.h"
#include "screens/barometer_gen.h"
#include "screens/home_gen.h"
#include "screens/motion_gen.h"
#include "screens/settings_gen.h"

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LVGL_PRO_DISPLAY_GEN_H*/