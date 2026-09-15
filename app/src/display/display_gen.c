/**
 * @file display_gen.c
 */

/*********************
 *      INCLUDES
 *********************/

#include "display_gen.h"

#if defined(LV_USE_XML) && LV_USE_XML
#endif /* LV_USE_XML */

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static void check_font(lv_font_t ** font, const char * name);

/**********************
 *  STATIC VARIABLES
 **********************/

static uint32_t display_target = DISPLAY_TARGET_ALL;

/*----------------
 * Translations
 *----------------*/

/**********************
 *  GLOBAL VARIABLES
 **********************/

/*--------------------
 *  Permanent screens
 *-------------------*/

/*----------------
 * Fonts
 *----------------*/

lv_font_t * jetbrains_12;
extern lv_font_t jetbrains_12_data;
lv_font_t * jetbrains_14;
extern lv_font_t jetbrains_14_data;
lv_font_t * jetbrains_bold_18;
extern lv_font_t jetbrains_bold_18_data;
lv_font_t * jetbrains_bold_40;
extern lv_font_t jetbrains_bold_40_data;
lv_font_t * icons_24;
extern lv_font_t icons_24_data;

/*----------------
 * Images
 *----------------*/



/*----------------
 * Global styles
 *----------------*/

lv_style_t page;
lv_style_t card_style;
lv_style_t tile;
lv_style_t title;
lv_style_t hero;
lv_style_t value;
lv_style_t value_sm;
lv_style_t label_xs;
lv_style_t unit;
lv_style_t pill;
lv_style_t dot;
lv_style_t icon;
lv_style_t content;

/*----------------
 * Subjects
 *----------------*/

lv_subject_t active_screen;
lv_subject_t empty_str;
lv_subject_t board_str;
lv_subject_t zephyr_str;
lv_subject_t firmware_str;
lv_subject_t built_str;
lv_subject_t uptime_str;
lv_subject_t temp_str;
lv_subject_t press_str;
lv_subject_t alt_str;
lv_subject_t alt_unit;
lv_subject_t press_trend;
lv_subject_t heading_deg;
lv_subject_t heading_str;
lv_subject_t roll_deg;
lv_subject_t pitch_deg;
lv_subject_t roll_str;
lv_subject_t pitch_str;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void display_init_gen(const char * asset_path)
{
    /* When running from the editor the theme set from the XML should overwrite this */
#if !defined(LV_EDITOR_PREVIEW)
#if LV_USE_THEME_SIMPLE
    lv_display_t * disp = lv_display_get_default();
    lv_theme_t * th = lv_theme_simple_init(disp);
    lv_display_set_theme(disp, th);
#else
    LV_LOG_WARN("Simple theme is selected in project.xml but LV_USE_THEME_SIMPLE is disabled");
#endif
#endif /*LV_EDITOR_PREVIEW*/


    /*----------------
     * Fonts
     *----------------*/

    /* Targets: any */

    #if DISPLAY_CHECK_COMPILE_TARGET(DISPLAY_TARGET_ALL)
    if (display_check_target(DISPLAY_TARGET_ALL)) {
        if (!jetbrains_12) {
            /* jetbrains_12 */
            /* get font 'jetbrains_12' from a C array */
            jetbrains_12 = &jetbrains_12_data;

        }
        if (!jetbrains_14) {
            /* jetbrains_14 */
            /* get font 'jetbrains_14' from a C array */
            jetbrains_14 = &jetbrains_14_data;

        }
        if (!jetbrains_bold_18) {
            /* jetbrains_bold_18 */
            /* get font 'jetbrains_bold_18' from a C array */
            jetbrains_bold_18 = &jetbrains_bold_18_data;

        }
        if (!jetbrains_bold_40) {
            /* jetbrains_bold_40 */
            /* get font 'jetbrains_bold_40' from a C array */
            jetbrains_bold_40 = &jetbrains_bold_40_data;

        }
        if (!icons_24) {
            /* icons_24 */
            /* get font 'icons_24' from a C array */
            icons_24 = &icons_24_data;

        }
    }
    #endif

    /*----------------
     * Images
     *----------------*/



    /*----------------
     * Global styles
     *----------------*/

    static bool style_inited = false;

    if (!style_inited) {
        /*Init all styles*/
        lv_style_init(&page);
        lv_style_init(&card_style);
        lv_style_init(&tile);
        lv_style_init(&title);
        lv_style_init(&hero);
        lv_style_init(&value);
        lv_style_init(&value_sm);
        lv_style_init(&label_xs);
        lv_style_init(&unit);
        lv_style_init(&pill);
        lv_style_init(&dot);
        lv_style_init(&icon);
        lv_style_init(&content);

        lv_style_set_bg_color(&page, BG);
        lv_style_set_bg_opa(&page, 255);
        lv_style_set_border_width(&page, 0);
        lv_style_set_pad_all(&page, 0);
        lv_style_set_width(&page, lv_pct(100));
        lv_style_set_height(&page, lv_pct(100));
        lv_style_set_bg_color(&card_style, CARD_COLOR);
        lv_style_set_bg_opa(&card_style, 255);
        lv_style_set_radius(&card_style, 14);
        lv_style_set_border_width(&card_style, 1);
        lv_style_set_border_color(&card_style, BORDER);
        lv_style_set_pad_all(&card_style, 14);
        lv_style_set_width(&card_style, lv_pct(100));
        lv_style_set_height(&card_style, LV_SIZE_CONTENT);
        lv_style_set_bg_color(&tile, CARD_COLOR);
        lv_style_set_bg_opa(&tile, 255);
        lv_style_set_radius(&tile, 12);
        lv_style_set_border_width(&tile, 1);
        lv_style_set_border_color(&tile, BORDER);
        lv_style_set_pad_all(&tile, 10);
        lv_style_set_height(&tile, LV_SIZE_CONTENT);
        lv_style_set_text_font(&title, jetbrains_bold_18);
        lv_style_set_text_color(&title, TEXT);
        lv_style_set_text_font(&hero, jetbrains_bold_40);
        lv_style_set_text_color(&hero, TEXT);
        lv_style_set_text_font(&value, jetbrains_bold_18);
        lv_style_set_text_color(&value, TEXT);
        lv_style_set_text_font(&value_sm, jetbrains_14);
        lv_style_set_text_color(&value_sm, TEXT);
        lv_style_set_text_font(&label_xs, jetbrains_12);
        lv_style_set_text_color(&label_xs, DIM);
        lv_style_set_text_letter_space(&label_xs, 1);
        lv_style_set_text_font(&unit, jetbrains_14);
        lv_style_set_text_color(&unit, DIM);
        lv_style_set_bg_color(&pill, PILLBG);
        lv_style_set_bg_opa(&pill, 255);
        lv_style_set_radius(&pill, 20);
        lv_style_set_pad_left(&pill, 8);
        lv_style_set_pad_right(&pill, 8);
        lv_style_set_pad_top(&pill, 2);
        lv_style_set_pad_bottom(&pill, 2);
        lv_style_set_text_font(&pill, jetbrains_12);
        lv_style_set_text_color(&pill, DIM);
        lv_style_set_bg_color(&dot, ACCENT);
        lv_style_set_bg_opa(&dot, 255);
        lv_style_set_radius(&dot, 4);
        lv_style_set_width(&dot, 8);
        lv_style_set_height(&dot, 8);
        lv_style_set_border_width(&dot, 0);
        lv_style_set_text_font(&icon, icons_24);
        lv_style_set_text_color(&icon, DOCKDIM);
        lv_style_set_bg_opa(&content, 0);
        lv_style_set_border_width(&content, 0);
        lv_style_set_pad_left(&content, 12);
        lv_style_set_pad_right(&content, 12);
        lv_style_set_pad_top(&content, 12);
        lv_style_set_pad_bottom(&content, 8);
        lv_style_set_pad_row(&content, 8);
        lv_style_set_width(&content, lv_pct(100));

        style_inited = true;
    }

    /*----------------
     * Subjects
     *----------------*/
    lv_subject_init_int(&active_screen, 1);
    static char empty_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char empty_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&empty_str,
                           empty_str_buf,
                           empty_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           ""
                          );
    static char board_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char board_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&board_str,
                           board_str_buf,
                           board_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "---"
                          );
    static char zephyr_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char zephyr_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&zephyr_str,
                           zephyr_str_buf,
                           zephyr_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "---"
                          );
    static char firmware_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char firmware_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&firmware_str,
                           firmware_str_buf,
                           firmware_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "---"
                          );
    static char built_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char built_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&built_str,
                           built_str_buf,
                           built_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "---"
                          );
    static char uptime_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char uptime_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&uptime_str,
                           uptime_str_buf,
                           uptime_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "0:00:00"
                          );
    static char temp_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char temp_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&temp_str,
                           temp_str_buf,
                           temp_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "--"
                          );
    static char press_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char press_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&press_str,
                           press_str_buf,
                           press_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "--"
                          );
    static char alt_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char alt_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&alt_str,
                           alt_str_buf,
                           alt_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "--"
                          );
    static char alt_unit_buf[UI_SUBJECT_STRING_LENGTH];
    static char alt_unit_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&alt_unit,
                           alt_unit_buf,
                           alt_unit_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "m"
                          );
    static char press_trend_buf[UI_SUBJECT_STRING_LENGTH];
    static char press_trend_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&press_trend,
                           press_trend_buf,
                           press_trend_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "--"
                          );
    lv_subject_init_int(&heading_deg, 0);
    static char heading_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char heading_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&heading_str,
                           heading_str_buf,
                           heading_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "---"
                          );
    lv_subject_init_int(&roll_deg, 0);
    lv_subject_init_int(&pitch_deg, 0);
    static char roll_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char roll_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&roll_str,
                           roll_str_buf,
                           roll_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "--"
                          );
    static char pitch_str_buf[UI_SUBJECT_STRING_LENGTH];
    static char pitch_str_prev_buf[UI_SUBJECT_STRING_LENGTH];
    lv_subject_init_string(&pitch_str,
                           pitch_str_buf,
                           pitch_str_prev_buf,
                           UI_SUBJECT_STRING_LENGTH,
                           "--"
                          );

    /*----------------
     * Translations
     *----------------*/

#if defined(LV_USE_XML) && LV_USE_XML
    /* Register widgets */

    /* Check all fonts / default if needed. This prevents fonts that are used in one target but
       defined in another from causing assertion failures during rendering of the Preview. */
    check_font(&jetbrains_12, "jetbrains_12");
    check_font(&jetbrains_14, "jetbrains_14");
    check_font(&jetbrains_bold_18, "jetbrains_bold_18");
    check_font(&jetbrains_bold_40, "jetbrains_bold_40");
    check_font(&icons_24, "icons_24");

    /* Register fonts */
    lv_xml_register_font(NULL, "jetbrains_12", jetbrains_12);
    lv_xml_register_font(NULL, "jetbrains_14", jetbrains_14);
    lv_xml_register_font(NULL, "jetbrains_bold_18", jetbrains_bold_18);
    lv_xml_register_font(NULL, "jetbrains_bold_40", jetbrains_bold_40);
    lv_xml_register_font(NULL, "icons_24", icons_24);

    /* Register subjects */
    lv_xml_register_subject(NULL, "active_screen", &active_screen);
    lv_xml_register_subject(NULL, "empty_str", &empty_str);
    lv_xml_register_subject(NULL, "board_str", &board_str);
    lv_xml_register_subject(NULL, "zephyr_str", &zephyr_str);
    lv_xml_register_subject(NULL, "firmware_str", &firmware_str);
    lv_xml_register_subject(NULL, "built_str", &built_str);
    lv_xml_register_subject(NULL, "uptime_str", &uptime_str);
    lv_xml_register_subject(NULL, "temp_str", &temp_str);
    lv_xml_register_subject(NULL, "press_str", &press_str);
    lv_xml_register_subject(NULL, "alt_str", &alt_str);
    lv_xml_register_subject(NULL, "alt_unit", &alt_unit);
    lv_xml_register_subject(NULL, "press_trend", &press_trend);
    lv_xml_register_subject(NULL, "heading_deg", &heading_deg);
    lv_xml_register_subject(NULL, "heading_str", &heading_str);
    lv_xml_register_subject(NULL, "roll_deg", &roll_deg);
    lv_xml_register_subject(NULL, "pitch_deg", &pitch_deg);
    lv_xml_register_subject(NULL, "roll_str", &roll_str);
    lv_xml_register_subject(NULL, "pitch_str", &pitch_str);

    /* Register callbacks */
#endif

    /* Register all the global assets so that they won't be created again when globals.xml is parsed.
     * While running in the editor skip this step to update the preview when the XML changes */
#if defined(LV_USE_XML) && LV_USE_XML && !defined(LV_EDITOR_PREVIEW)
    /* Register images */
#endif

#if !defined(LV_USE_XML) || LV_USE_XML == 0
    /*--------------------
     *  Permanent screens
     *-------------------*/
    /* If XML is enabled it's assumed that the permanent screens are created
     * manually from XML using lv_xml_create() */
#endif
}

void display_set_target(uint32_t target)
{
    display_target = target;
}

uint32_t display_get_target(void)
{
    return display_target;
}

bool display_check_target(uint32_t target)
{
    return (display_target & target) ? true : false;
}

/* Callbacks */

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void check_font(lv_font_t ** font, const char * name)
{
    if (!(*font)) {
        *font = (lv_font_t *)LV_FONT_DEFAULT;
        LV_LOG_WARN("font `%s` was not set. Using `LV_FONT_DEFAULT` instead", name);
    }
}