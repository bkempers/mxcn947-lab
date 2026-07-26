#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <stdio.h>

#include "lvgl_main.h"
#include "screens/sensors.h"

static const struct device *const display_dev = DEVICE_DT_GET(DT_NODELABEL(st7789v));

static lv_obj_t *tabview;
static lv_obj_t *tab_home, *tab_sensors;

int lvgl_display_init(void)
{
    if (!device_is_ready(display_dev)) {
        return -ENODEV;
    }

    tabview = lv_tabview_create(lv_screen_active());
    lv_tabview_set_tab_bar_size(tabview, 16);
    lv_tabview_set_tab_bar_position(tabview, LV_DIR_BOTTOM);
    tab_home     = lv_tabview_add_tab(tabview, "Home");
    tab_sensors  = lv_tabview_add_tab(tabview, "Sensors");

    home_create(tab_home);
    // sensors_create(tab_sensors);

    lv_tabview_set_active(tabview, 0, LV_ANIM_OFF);

    display_blanking_off(display_dev);
    return 0;
}

lv_obj_t *home_create(lv_obj_t *parent) {
    lv_obj_t *root = lv_obj_create(parent);
    lv_obj_set_size(root, LV_PCT(100), LV_PCT(100));
    lv_obj_t *title = lv_label_create(root);
    lv_label_set_text(title, "MXCN947-LAB");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
    /* layout with flex/grid, apply theme styles... */
    return root;
}

// void display_update(int temp_c, int temp_frac, int press_kpa, int press_frac)
// {
//     char buf[32];
//
//     snprintf(buf, sizeof(buf), "Temp: %d.%d C", temp_c, temp_frac);
//     lv_label_set_text(temp_label, buf);
//
//     snprintf(buf, sizeof(buf), "Press: %d.%d kPa", press_kpa, press_frac);
//     lv_label_set_text(press_label, buf);
// }

void display_tick(void)
{
    lv_timer_handler();
}


    // /* Title */
    // lv_obj_t *title = lv_label_create(lv_scr_act());
    // lv_label_set_text(title, "BMP581 Monitor");
    // lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
    //
    // /* Temperature readout */
    // temp_label = lv_label_create(lv_scr_act());
    // lv_label_set_text(temp_label, "Temp: --.- C");
    // lv_obj_align(temp_label, LV_ALIGN_CENTER, 0, -20);
    //
    // /* Pressure readout */
    // press_label = lv_label_create(lv_scr_act());
    // lv_label_set_text(press_label, "Press: --.- kPa");
    // lv_obj_align(press_label, LV_ALIGN_CENTER, 0, 20);

