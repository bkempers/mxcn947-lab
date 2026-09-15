#include "nav.h"
#include "display_gen.h"

#include <string.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(nav, LOG_LEVEL_INF);

/* ---- registry ---- */

static struct nav_screen registry[NAV_MAX_SCREENS];
static int reg_count;

int nav_register(int index, const char *name, uint32_t icon,
		 const char *group, lv_obj_t *(*create)(void))
{
	if (reg_count >= NAV_MAX_SCREENS) {
		return -1;
	}
	struct nav_screen *s = &registry[reg_count++];
	s->index  = index;
	s->name   = name;
	s->icon   = icon;
	s->group  = group;
	s->create = create;
	s->screen = NULL;
	return 0;
}

lv_obj_t *nav_get_screen(int index)
{
	for (int i = 0; i < reg_count; i++) {
		if (registry[i].index == index) {
			return registry[i].screen;
		}
	}
	return NULL;
}

int nav_screen_count(void) { return reg_count; }

/* ---- helpers ---- */

static void codepoint_to_utf8(uint32_t cp, char *buf)
{
	if (cp < 0x80) {
		buf[0] = (char)cp;
		buf[1] = 0;
	} else if (cp < 0x800) {
		buf[0] = 0xC0 | (cp >> 6);
		buf[1] = 0x80 | (cp & 0x3F);
		buf[2] = 0;
	} else {
		buf[0] = 0xE0 | (cp >> 12);
		buf[1] = 0x80 | ((cp >> 6) & 0x3F);
		buf[2] = 0x80 | (cp & 0x3F);
		buf[3] = 0;
	}
}

static lv_obj_t *find_by_name(lv_obj_t *parent, const char *name)
{
	uint32_t cnt = lv_obj_get_child_count(parent);
	for (uint32_t i = 0; i < cnt; i++) {
		lv_obj_t *child = lv_obj_get_child(parent, i);
		const char *n = lv_obj_get_name(child);
		if (n && strcmp(n, name) == 0) {
			return child;
		}
		lv_obj_t *found = find_by_name(child, name);
		if (found) {
			return found;
		}
	}
	return NULL;
}

/* ---- overlay state ---- */

static lv_obj_t *overlay_bg;
static lv_obj_t *nav_grid;
static lv_obj_t *trigger_btn;
static bool overlay_visible;

static char grid_icon_str[4]; /* U+F58D fa-grip, 3-byte UTF-8 + nul */

/* ---- show / hide ---- */

static void show_overlay(void)
{
	if (overlay_visible) {
		return;
	}
	overlay_visible = true;

	/* update checked highlights */
	int cur = lv_subject_get_int(&active_screen);
	uint32_t cnt = lv_obj_get_child_count(nav_grid);
	for (uint32_t i = 0; i < cnt; i++) {
		lv_obj_t *child = lv_obj_get_child(nav_grid, i);
		uint32_t inner = lv_obj_get_child_count(child);
		for (uint32_t j = 0; j < inner; j++) {
			lv_obj_t *item = lv_obj_get_child(child, j);
			void *ud = lv_obj_get_user_data(item);
			if (!ud) {
				continue;
			}
			if ((int)(intptr_t)ud == cur) {
				lv_obj_add_state(item, LV_STATE_CHECKED);
			} else {
				lv_obj_remove_state(item, LV_STATE_CHECKED);
			}
		}
	}

	lv_obj_remove_flag(overlay_bg, LV_OBJ_FLAG_HIDDEN);
}

static void hide_overlay(void)
{
	if (!overlay_visible) {
		return;
	}
	overlay_visible = false;
	lv_obj_add_flag(overlay_bg, LV_OBJ_FLAG_HIDDEN);
}

/* ---- event handlers ---- */

static void on_trigger_click(lv_event_t *e)
{
	ARG_UNUSED(e);
	overlay_visible ? hide_overlay() : show_overlay();
}

static void on_dismiss(lv_event_t *e)
{
	ARG_UNUSED(e);
	hide_overlay();
}

static void on_item_click(lv_event_t *e)
{
	int idx = (int)(intptr_t)lv_obj_get_user_data(lv_event_get_target(e));
	hide_overlay();
	lv_subject_set_int(&active_screen, idx);
}

/* ---- active_screen observer ---- */

static void on_active_screen(lv_observer_t *o, lv_subject_t *s)
{
	ARG_UNUSED(o);
	int idx = lv_subject_get_int(s);
	lv_obj_t *scr = nav_get_screen(idx);
	if (!scr || lv_screen_active() == scr) {
		return;
	}
	lv_screen_load(scr);
}

/* ---- populate grid from registry ---- */

static void populate_grid(void)
{
	const char *groups[NAV_MAX_SCREENS];
	int group_count = 0;

	for (int i = 0; i < reg_count; i++) {
		bool found = false;
		for (int g = 0; g < group_count; g++) {
			if (strcmp(registry[i].group, groups[g]) == 0) {
				found = true;
				break;
			}
		}
		if (!found) {
			groups[group_count++] = registry[i].group;
		}
	}

	for (int g = 0; g < group_count; g++) {
		lv_obj_t *hdr = lv_label_create(nav_grid);
		lv_label_set_text_static(hdr, groups[g]);
		lv_obj_add_style(hdr, &label_xs, 0);
		if (g > 0) {
			lv_obj_set_style_pad_top(hdr, 8, 0);
		}

		lv_obj_t *row = lv_obj_create(nav_grid);
		lv_obj_set_width(row, lv_pct(100));
		lv_obj_set_height(row, LV_SIZE_CONTENT);
		lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW_WRAP);
		lv_obj_set_style_pad_column(row, 8, 0);
		lv_obj_set_style_pad_row(row, 8, 0);
		lv_obj_set_style_bg_opa(row, 0, 0);
		lv_obj_set_style_border_width(row, 0, 0);
		lv_obj_set_style_pad_all(row, 0, 0);

		for (int i = 0; i < reg_count; i++) {
			if (strcmp(registry[i].group, groups[g]) != 0) {
				continue;
			}
			char utf8[4];
			codepoint_to_utf8(registry[i].icon, utf8);

			lv_obj_t *item = nav_item_create(row,
				utf8, registry[i].name);
			lv_obj_add_flag(item, LV_OBJ_FLAG_CLICKABLE);
			lv_obj_set_user_data(item,
				(void *)(intptr_t)registry[i].index);
			lv_obj_add_event_cb(item, on_item_click,
				LV_EVENT_CLICKED, NULL);
		}
	}
}

/* ---- build FAB trigger ---- */

static void build_trigger(void)
{
	trigger_btn = lv_button_create(lv_layer_top());
	lv_obj_set_size(trigger_btn, 44, 44);
	lv_obj_align(trigger_btn, LV_ALIGN_BOTTOM_RIGHT, -12, -12);
	lv_obj_set_style_radius(trigger_btn, 22, 0);
	lv_obj_set_style_bg_color(trigger_btn, DOCK_COLOR, 0);
	lv_obj_set_style_bg_opa(trigger_btn, 200, 0);
	lv_obj_set_style_border_width(trigger_btn, 0, 0);
	lv_obj_set_style_shadow_width(trigger_btn, 12, 0);
	lv_obj_set_style_shadow_opa(trigger_btn, 80, 0);
	lv_obj_set_style_shadow_color(trigger_btn, lv_color_black(), 0);
	lv_obj_set_style_pad_all(trigger_btn, 0, 0);

	lv_obj_t *lbl = lv_label_create(trigger_btn);
	lv_label_set_text_static(lbl, grid_icon_str);
	lv_obj_set_style_text_font(lbl, icons_24, 0);
	lv_obj_set_style_text_color(lbl, lv_color_white(), 0);
	lv_obj_set_align(lbl, LV_ALIGN_CENTER);

	lv_obj_add_event_cb(trigger_btn, on_trigger_click,
		LV_EVENT_CLICKED, NULL);
}

/* ---- public ---- */

void nav_init(void)
{
	codepoint_to_utf8(0xf58d, grid_icon_str);

	for (int i = 0; i < reg_count; i++) {
		registry[i].screen = registry[i].create();
	}

	/* trigger first (lower z), overlay on top */
	build_trigger();

	/* XML-generated overlay on lv_layer_top() */
	overlay_bg = nav_overlay_create(lv_layer_top());
	lv_obj_add_flag(overlay_bg, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_event_cb(overlay_bg, on_dismiss, LV_EVENT_CLICKED, NULL);

	nav_grid = find_by_name(overlay_bg, "nav_grid");
	lv_obj_t *close_btn = find_by_name(overlay_bg, "nav_close");
	if (close_btn) {
		lv_obj_add_event_cb(close_btn, on_dismiss,
			LV_EVENT_CLICKED, NULL);
	}
	if (nav_grid) {
		populate_grid();
	}

	/* load first screen */
	if (reg_count > 0 && registry[0].screen) {
		lv_obj_set_style_bg_color(registry[0].screen, BG, 0);
		lv_obj_set_style_bg_opa(registry[0].screen, LV_OPA_COVER, 0);
		lv_screen_load(registry[0].screen);
	}

	lv_subject_set_int(&active_screen, registry[0].index);
	lv_subject_add_observer(&active_screen, on_active_screen, NULL);

	LOG_INF("nav: %d screens registered", reg_count);
}
