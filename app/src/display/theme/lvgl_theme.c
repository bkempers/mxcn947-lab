#include "lvgl_theme.h"

lv_style_t ui_st_screen, ui_st_card, ui_st_accentbar;
lv_style_t ui_st_caption, ui_st_value, ui_st_unit, ui_st_title;

void ui_theme_init(void)
{
    lv_display_t *disp = lv_display_get_default();
    lv_theme_t *th = lv_theme_default_init(disp, UI_COL_ACCENT, UI_COL_ACCENT,
                                           true, /* dark */ &lv_font_montserrat_14);
    lv_display_set_theme(disp, th);

    /* page */
    lv_style_init(&ui_st_screen);
    lv_style_set_bg_color(&ui_st_screen, UI_COL_BG);
    lv_style_set_bg_opa(&ui_st_screen, LV_OPA_COVER);
    lv_style_set_border_width(&ui_st_screen, 0);
    lv_style_set_pad_all(&ui_st_screen, 8);
    lv_style_set_pad_row(&ui_st_screen, 8);

    /* card surface: rounded, faint 1px border, slightly lighter than page */
    lv_style_init(&ui_st_card);
    lv_style_set_bg_color(&ui_st_card, UI_COL_CARD);
    lv_style_set_bg_opa(&ui_st_card, LV_OPA_COVER);
    lv_style_set_radius(&ui_st_card, 10);
    lv_style_set_border_width(&ui_st_card, 1);
    lv_style_set_border_color(&ui_st_card, lv_color_hex(0x272B31));
    lv_style_set_border_opa(&ui_st_card, LV_OPA_COVER);
    lv_style_set_pad_all(&ui_st_card, 10);
    lv_style_set_width(&ui_st_card, lv_pct(100));
    lv_style_set_height(&ui_st_card, LV_SIZE_CONTENT);

    /* the accent bar down the left edge of a card */
    lv_style_init(&ui_st_accentbar);
    lv_style_set_bg_color(&ui_st_accentbar, UI_COL_ACCENT);
    lv_style_set_bg_opa(&ui_st_accentbar, LV_OPA_COVER);
    lv_style_set_radius(&ui_st_accentbar, 2);
    lv_style_set_width(&ui_st_accentbar, 3);
    lv_style_set_height(&ui_st_accentbar, lv_pct(100));

    /* type ramp */
    lv_style_init(&ui_st_caption);
    lv_style_set_text_font(&ui_st_caption, &lv_font_montserrat_14);
    lv_style_set_text_color(&ui_st_caption, UI_COL_DIM);
    lv_style_set_text_letter_space(&ui_st_caption, 2);   /* airy uppercase */

    lv_style_init(&ui_st_value);
    lv_style_set_text_font(&ui_st_value, &lv_font_montserrat_28);
    lv_style_set_text_color(&ui_st_value, UI_COL_TEXT);

    lv_style_init(&ui_st_unit);
    lv_style_set_text_font(&ui_st_unit, &lv_font_montserrat_14);
    lv_style_set_text_color(&ui_st_unit, UI_COL_DIM);

    lv_style_init(&ui_st_title);
    lv_style_set_text_font(&ui_st_title, &lv_font_montserrat_20);
    lv_style_set_text_color(&ui_st_title, UI_COL_TEXT);
}
