#ifndef LVGL_THEME_H
#define LVGL_THEME_H

#include <lvgl.h>

/* palette */
#define UI_COL_BG        lv_color_hex(0x0D0F12)   /* page */
#define UI_COL_CARD      lv_color_hex(0x171A1F)   /* surface */
#define UI_COL_ACCENT    lv_color_hex(0x2DD4BF)   /* teal */
#define UI_COL_TEXT      lv_color_hex(0xF2F4F5)   /* primary */
#define UI_COL_DIM       lv_color_hex(0x6B7280)   /* caption */

void ui_theme_init(void);
extern lv_style_t ui_st_screen, ui_st_card, ui_st_accentbar;
extern lv_style_t ui_st_caption, ui_st_value, ui_st_unit, ui_st_title;

#endif
