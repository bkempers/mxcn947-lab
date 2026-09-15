#ifndef NAV_H
#define NAV_H

#include <lvgl.h>

#define NAV_MAX_SCREENS 8

struct nav_screen {
	int index;
	const char *name;
	uint32_t icon;
	const char *group;
	lv_obj_t *(*create)(void);
	lv_obj_t *screen;
};

/**
 * Register a screen into the global registry.
 * Call during init, before nav_init().
 */
int nav_register(int index, const char *name, uint32_t icon,
		 const char *group, lv_obj_t *(*create)(void));

/**
 * Initialize the navigation overlay.
 * Must be called after display_init_gen() and after all nav_register() calls.
 * Creates all registered screens, builds the floating trigger and overlay
 * on lv_layer_top(), and attaches the active_screen observer.
 */
void nav_init(void);

/**
 * Get the screen object for a given index.
 * Returns NULL if index is not registered.
 */
lv_obj_t *nav_get_screen(int index);

/** Get the number of registered screens. */
int nav_screen_count(void);

#endif /* NAV_H */
