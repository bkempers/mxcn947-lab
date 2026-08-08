#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/version.h>
#include <zephyr/app_version.h>
#include <zephyr/drivers/display.h>
#include <zephyr/sys/atomic.h>
#include <stdio.h>

#include "../theme/lvgl_theme.h"

static lv_obj_t *lbl_uptime;

static void kv_row(lv_obj_t *parent, const char *key, const char *val)
{
    lv_obj_t *row = lv_obj_create(parent);
    lv_obj_remove_style_all(row);
    lv_obj_set_width(row, lv_pct(100));
    lv_obj_set_height(row, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_ver(row, 3, 0);

    lv_obj_t *k = lv_label_create(row);
    lv_obj_add_style(k, &ui_st_caption, 0);
    lv_label_set_text(k, key);

    lv_obj_t *v = lv_label_create(row);
    lv_obj_add_style(v, &ui_st_unit, 0);
    lv_obj_set_style_text_color(v, UI_COL_TEXT, 0);   /* readable, not dim */
    lv_label_set_text(v, val);

    if (!strcmp(key, "UPTIME")) lbl_uptime = v;        /* keep for live update */
}

static void status_bar(lv_obj_t *parent)
{
    lv_obj_t *bar = lv_obj_create(parent);
    lv_obj_remove_style_all(bar);
    lv_obj_set_width(bar, lv_pct(100));
    lv_obj_set_height(bar, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(bar, LV_FLEX_ALIGN_SPACE_BETWEEN,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_all(bar, 5, 0);
    lv_obj_set_style_bg_color(bar, UI_COL_CARD, 0);
    lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(bar, 6, 0);

    lv_obj_t *left = lv_obj_create(bar);
    lv_obj_remove_style_all(left);
    lv_obj_set_height(left, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(left, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(left, LV_FLEX_ALIGN_START,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_column(left, 6, 0);

    lv_obj_t *dot = lv_obj_create(left);       /* status indicator */
    lv_obj_remove_style_all(dot);
    lv_obj_set_size(dot, 8, 8);
    lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_bg_color(dot, UI_COL_ACCENT, 0);
    lv_obj_set_style_bg_opa(dot, LV_OPA_COVER, 0);

    lv_obj_t *ver = lv_label_create(bar);
    lv_obj_add_style(ver, &ui_st_caption, 0);
    lv_label_set_text(ver, "zephyr " KERNEL_VERSION_STRING);
}

void home_create(lv_obj_t *parent)
{
    lv_obj_add_style(parent, &ui_st_screen, 0);
    lv_obj_set_flex_flow(parent, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scrollbar_mode(parent, LV_SCROLLBAR_MODE_OFF);

    status_bar(parent);

    lv_obj_t *title = lv_label_create(parent);
    lv_obj_add_style(title, &ui_st_title, 0);
    lv_obj_set_style_pad_top(title, 6, 0);
    lv_label_set_text(title, "MCXN947 LAB");

    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_add_style(card, &ui_st_card, 0);
    lv_obj_set_scrollbar_mode(card, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(card, LV_FLEX_FLOW_COLUMN);

    kv_row(card, "BOARD",  CONFIG_BOARD);
    kv_row(card, "ZEPHYR", KERNEL_VERSION_STRING);
    kv_row(card, "FIRMWARE", APP_VERSION_STRING);
    kv_row(card, "BUILT",  __DATE__);
    kv_row(card, "UPTIME", "0:00:00");
}

void home_tick(void)
{
    if (!lbl_uptime) return;
    uint32_t s = (uint32_t)(k_uptime_get() / 1000);
    lv_label_set_text_fmt(lbl_uptime, "%u:%02u:%02u", s / 3600U, (s / 60U) % 60U, s % 60U);
}
