#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <stdio.h>

#include <lvgl.h>

#include "lvgl_main.h"

static const struct device *const display_dev = DEVICE_DT_GET(DT_NODELABEL(st7789v));

static lv_obj_t *temp_label;
static lv_obj_t *press_label;

int lvgl_ui_init(void)
{
    if (!device_is_ready(display_dev)) {
        return -ENODEV;
    }

    /* Title */
    lv_obj_t *title = lv_label_create(lv_scr_act());
    lv_label_set_text(title, "BMP581 Monitor");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    /* Temperature readout */
    temp_label = lv_label_create(lv_scr_act());
    lv_label_set_text(temp_label, "Temp: --.- C");
    lv_obj_align(temp_label, LV_ALIGN_CENTER, 0, -20);

    /* Pressure readout */
    press_label = lv_label_create(lv_scr_act());
    lv_label_set_text(press_label, "Press: --.- kPa");
    lv_obj_align(press_label, LV_ALIGN_CENTER, 0, 20);

    display_blanking_off(display_dev);
    return 0;
}

/* Call this whenever you have fresh sensor data */
void lvgl_ui_update(int temp_c, int temp_frac, int press_kpa, int press_frac)
{
    char buf[32];

    snprintf(buf, sizeof(buf), "Temp: %d.%d C", temp_c, temp_frac);
    lv_label_set_text(temp_label, buf);

    snprintf(buf, sizeof(buf), "Press: %d.%d kPa", press_kpa, press_frac);
    lv_label_set_text(press_label, buf);
}

/* The LVGL heartbeat — needs to run periodically */
void lvgl_ui_tick(void)
{
    lv_timer_handler();
}
