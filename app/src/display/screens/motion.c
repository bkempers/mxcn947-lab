#include <lvgl.h>
#include <math.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

#include <string.h>
#include <stdio.h>
#include "display_gen.h"
#include "motion.h"

LOG_MODULE_REGISTER(motion, CONFIG_APP_LOG_LEVEL);

static lv_obj_t *g_scale, *g_needle, *g_horizon;
static lv_obj_t *g_roll_line;

/* persistent point arrays — lv_line_set_points stores a pointer, not a copy */
static lv_point_precise_t needle_pts[2];
static lv_point_precise_t roll_pts[2];

/* recursive name search — lv_obj_get_child_by_name only checks direct children */
static lv_obj_t *find_obj_by_name(lv_obj_t *parent, const char *name)
{
    uint32_t cnt = lv_obj_get_child_count(parent);
    for (uint32_t i = 0; i < cnt; i++) {
        lv_obj_t *child = lv_obj_get_child(parent, i);
        const char *n = lv_obj_get_name(child);
        if (n && strcmp(n, name) == 0) return child;
        lv_obj_t *found = find_obj_by_name(child, name);
        if (found) return found;
    }
    return NULL;
}

/*
 * Compass needle: set line endpoints directly instead of lv_scale_set_line_needle_value.
 * The scale API invalidates the entire 98×98 scale (redrawing all 37 ticks with trig),
 * which is too expensive at 10 Hz. lv_line_set_points only invalidates the line itself.
 */
static void on_heading(lv_observer_t *o, lv_subject_t *s)
{
    int deg = lv_subject_get_int(s);
    /* 0° = North (top), CW. Convert to math angle: North = -90° from East. */
    float rad = (float)(deg - 90) * (3.14159265f / 180.0f);
    needle_pts[0].x = 49.0f;
    needle_pts[0].y = 49.0f;
    needle_pts[1].x = 49.0f + 40.0f * cosf(rad);
    needle_pts[1].y = 49.0f + 40.0f * sinf(rad);
    lv_line_set_points(g_needle, needle_pts, 2);
}

static void on_attitude(lv_observer_t *o, lv_subject_t *s)
{
    ARG_UNUSED(o);
    ARG_UNUSED(s);
    if (!g_horizon) return;

    int pitch = lv_subject_get_int(&pitch_deg);
    int roll  = lv_subject_get_int(&roll_deg);

    /* shift sky/ground vertically: +pitch = nose up = more sky = horizon down */
    lv_obj_align(g_horizon, LV_ALIGN_CENTER, 0, pitch);

    /* tilt the center reference line for roll */
    if (g_roll_line) {
        float rad = (float)roll * (3.14159265f / 180.0f);
        float cx = 42.0f, cy = 42.0f, half = 20.0f;
        roll_pts[0].x = cx - half * cosf(rad);
        roll_pts[0].y = cy + half * sinf(rad);
        roll_pts[1].x = cx + half * cosf(rad);
        roll_pts[1].y = cy - half * sinf(rad);
        lv_line_set_points(g_roll_line, roll_pts, 2);
    }
}

static const char *cardinal(int deg)
{
    static const char *pts[] = {"N","NE","E","SE","S","SW","W","NW"};
    return pts[((deg + 22) / 45) % 8];
}

void motion_ui_init(lv_obj_t *screen)
{
    g_scale = find_obj_by_name(screen, "compass");
    lv_obj_t *slot = find_obj_by_name(screen, "horizon_slot");

    if (!g_scale) {
        LOG_WRN("motion: compass widget not found");
        return;
    }

    /* needle is already created in motion_gen.c, find it by name */
    g_needle = find_obj_by_name(g_scale, "needle");
    if (!g_needle) {
        LOG_WRN("motion: needle not found, creating");
        g_needle = lv_line_create(g_scale);
        lv_obj_set_style_line_width(g_needle, 3, 0);
        lv_obj_set_style_line_color(g_needle, lv_color_hex(0x2E5BFF), 0);
        lv_obj_set_style_line_rounded(g_needle, true, 0);
    }

    /* initial needle pointing North — direct points, no scale API */
    needle_pts[0].x = 49.0f;
    needle_pts[0].y = 49.0f;
    needle_pts[1].x = 49.0f;
    needle_pts[1].y = 9.0f;
    lv_line_set_points(g_needle, needle_pts, 2);

    if (slot) {
        /* attitude indicator: sky/ground split shifts with pitch,
           roll line tilts over the top */
        g_horizon = lv_obj_create(slot);
        lv_obj_set_size(g_horizon, 84, 168);
        lv_obj_center(g_horizon);
        lv_obj_set_style_bg_color(g_horizon, GROUND, 0);
        lv_obj_set_style_bg_opa(g_horizon, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(g_horizon, 0, 0);
        lv_obj_set_style_radius(g_horizon, 0, 0);
        lv_obj_set_style_pad_all(g_horizon, 0, 0);

        lv_obj_t *sky = lv_obj_create(g_horizon);
        lv_obj_set_size(sky, 84, 84);
        lv_obj_align(sky, LV_ALIGN_TOP_LEFT, 0, 0);
        lv_obj_set_style_bg_color(sky, SKY, 0);
        lv_obj_set_style_bg_opa(sky, LV_OPA_COVER, 0);
        lv_obj_set_style_border_width(sky, 0, 0);
        lv_obj_set_style_radius(sky, 0, 0);
        lv_obj_set_style_pad_all(sky, 0, 0);

        /* roll indicator line — sits on top of the sky/ground in the slot */
        g_roll_line = lv_line_create(slot);
        lv_obj_set_style_line_width(g_roll_line, 2, 0);
        lv_obj_set_style_line_color(g_roll_line, TEXT, 0);
        lv_obj_set_style_line_rounded(g_roll_line, true, 0);
        /* initial: level */
        roll_pts[0].x = 22.0f;
        roll_pts[0].y = 42.0f;
        roll_pts[1].x = 62.0f;
        roll_pts[1].y = 42.0f;
        lv_line_set_points(g_roll_line, roll_pts, 2);
    }

    lv_subject_add_observer(&heading_deg, on_heading,  NULL);
    lv_subject_add_observer(&roll_deg,    on_attitude, NULL);
    lv_subject_add_observer(&pitch_deg,   on_attitude, NULL);

    LOG_INF("motion UI initialized (scale=%p needle=%p horizon=%p roll_line=%p)",
            g_scale, g_needle, g_horizon, g_roll_line);
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
