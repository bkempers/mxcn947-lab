#include <lvgl.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#include <stdio.h>
#include "display_gen.h"
#include "motion.h"

LOG_MODULE_REGISTER(motion, CONFIG_APP_LOG_LEVEL);

static lv_obj_t *g_scale, *g_needle, *g_horizon;

static void on_heading(lv_observer_t *o, lv_subject_t *s)
{
    lv_scale_set_line_needle_value(g_scale, g_needle, 40, lv_subject_get_int(s));
}

static void on_attitude(lv_observer_t *o, lv_subject_t *s)
{
    lv_image_set_rotation(g_horizon, lv_subject_get_int(&roll_deg) * 10); /* 0.1° units */
    lv_obj_set_y(g_horizon, lv_subject_get_int(&pitch_deg) * 2);          /* px/deg, tune */
}

static const char *cardinal(int deg)
{
    static const char *pts[] = {"N","NE","E","SE","S","SW","W","NW"};
    return pts[((deg + 22) / 45) % 8];
}

void motion_ui_init(lv_obj_t *screen)
{
    g_scale = lv_obj_get_child_by_name(screen, "compass");
    lv_obj_t *slot = lv_obj_get_child_by_name(screen, "horizon_slot");

    if (!g_scale || !slot) {
        LOG_INF("motion: named children not found (scale=%p slot=%p)", g_scale, slot);
        return;                       /* don't crash; screen still shows */
    }

    /* needle: built in C so we own the handle */
    g_needle = lv_line_create(g_scale);
    lv_obj_set_style_line_width(g_needle, 3, 0);
    lv_obj_set_style_line_color(g_needle, lv_color_hex(0x2E5BFF), 0);
    lv_obj_set_style_line_rounded(g_needle, true, 0);

    /* horizon: an image (a two-tone sky/ground bitmap) larger than the 84px slot,
       so rotation never exposes a corner. Simplest first pass: a plain object split
       in two colors via two child rects, rotated together. */
    g_horizon = lv_image_create(slot);
    /* lv_image_set_src(g_horizon, &img_horizon);  // your sky/ground asset */
    lv_image_set_pivot(g_horizon, 60, 60);          /* center of a 120px image */
    lv_obj_center(g_horizon);

    lv_subject_add_observer(&heading_deg, on_heading,  NULL);
    lv_subject_add_observer(&roll_deg,    on_attitude, NULL);
    lv_subject_add_observer(&pitch_deg,   on_attitude, NULL);
}

void motion_update(int heading, int roll, int pitch)
{
    char buf[16];
    lv_subject_set_int(&heading_deg, heading);
    lv_subject_set_int(&roll_deg, roll);
    lv_subject_set_int(&pitch_deg, pitch);
    snprintf(buf, sizeof(buf), "%d %s", heading, cardinal(heading));
    lv_subject_copy_string(&heading_str, buf);
    snprintf(buf, sizeof(buf), "%+d", roll);  lv_subject_copy_string(&roll_str, buf);
    snprintf(buf, sizeof(buf), "%+d", pitch); lv_subject_copy_string(&pitch_str, buf);
}
