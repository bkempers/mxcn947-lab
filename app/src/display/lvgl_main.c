#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/sys/atomic.h>
#include <stdio.h>

#include "lvgl_main.h"
#include "screens/home.h"
#include "screens/sensors.h"
#include "theme/lvgl_theme.h"
#include "../sensors/baro/bmp581.h"

static const struct device *const display_dev = DEVICE_DT_GET(DT_NODELABEL(st7789v));

static lv_obj_t *tabview;
static lv_obj_t *tab_home, *tab_sensors;
static atomic_t tab_request = ATOMIC_INIT(-1);

/* called from any thread (the shell) — sets a request, touches no lvgl */
void ui_request_tab(int idx)
{
    atomic_set(&tab_request, idx);
}

/* runs on the display thread (fires inside lv_timer_handler) */
static void ui_service_timer(lv_timer_t *timer)
{
    int req = atomic_set(&tab_request, -1);   /* read-and-clear, returns prior */
    if (req >= 0 && (uint32_t)req < lv_tabview_get_tab_count(tabview)) {
        lv_tabview_set_active(tabview, req, LV_ANIM_OFF);
    }

    /* sensors screen refresh */
    struct baro_data baro;
    if (bmp581_get(&baro)) {
        sensors_baro_update(&baro);
    }

    home_tick();
}

int lvgl_display_init(void)
{
    ui_theme_init();

    if (!device_is_ready(display_dev)) {
        return -ENODEV;
    }

    tabview = lv_tabview_create(lv_screen_active());
    lv_tabview_set_tab_bar_size(tabview, 30);
    lv_tabview_set_tab_bar_position(tabview, LV_DIR_BOTTOM);
    tab_home     = lv_tabview_add_tab(tabview, "home");
    tab_sensors  = lv_tabview_add_tab(tabview, "sensors");

    home_create(tab_home);
    sensors_create(tab_sensors);

    lv_tabview_set_active(tabview, 0, LV_ANIM_OFF);
    lv_timer_create(ui_service_timer,  100, NULL);

    display_blanking_off(display_dev);
    return 0;
}

void display_tick(void)
{
    lv_timer_handler();
}

#ifdef CONFIG_SHELL

#include <zephyr/shell/shell.h>
#include <stdlib.h>

static int cmd_display(const struct shell *sh, size_t argc, char **argv)
{
    ui_request_tab(strtoul(argv[1], NULL, 10));
    shell_print(sh, "requested tab %s", argv[1]);
    return 0;
}
SHELL_CMD_ARG_REGISTER(display, NULL, "display <index>", cmd_display, 2, 2);

#endif // CONFIG_SHELL
