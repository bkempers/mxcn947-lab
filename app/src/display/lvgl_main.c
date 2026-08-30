#include <lvgl.h>
#include <stdio.h>
#include <zephyr/app_version.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/version.h>

/* --- generated from the XML export: match these names to your ui/generated/
 * files --- */
#include "display_gen.h"
#include "screens/barometer_gen.h"
#include "screens/home_gen.h"
#include "screens/motion_gen.h"
#include "screens/settings_gen.h"

#include "../sensors/baro/bmp581.h"
#include "screens/motion.h" /* needle + horizon helpers, below */

static const struct device *const display_dev =
    DEVICE_DT_GET(DT_NODELABEL(st7789v));

static lv_obj_t *screens[4]; /* 0 home, 1 barometer, 2 motion, 3 settings */
static atomic_t screen_request = ATOMIC_INIT(-1);

/* ---------- screen switching ---------- */
static void on_active_screen(lv_observer_t *o, lv_subject_t *s) {
  int i = lv_subject_get_int(s);
  if (i >= 0 && i < 4 && screens[i]) {
    lv_screen_load_anim(screens[i], LV_SCR_LOAD_ANIM_FADE_IN, 150, 0, false);
  }
}

/* safe to call from the shell thread — no LVGL touched here */
void ui_request_screen(int idx) { atomic_set(&screen_request, idx); }

/* ---------- home: static info + uptime ---------- */
static void home_bind_static(void) {
  lv_subject_copy_string(&board_str, CONFIG_BOARD);
  lv_subject_copy_string(&zephyr_str, KERNEL_VERSION_STRING);
  lv_subject_copy_string(&firmware_str, APP_VERSION_STRING);
  lv_subject_copy_string(&built_str, __DATE__);
}
static void home_tick(void) {
  uint32_t s = (uint32_t)(k_uptime_get() / 1000);
  char buf[16];
  snprintf(buf, sizeof(buf), "%u:%02u:%02u", s / 3600U, (s / 60U) % 60U,
           s % 60U);
  lv_subject_copy_string(&uptime_str, buf);
}

/* ---------- barometer: values + trend + units ---------- */
static struct baro_data last_baro;
static float prev_press_hpa;
static bool have_prev;

static void reformat(void) {
  char buf[24];

  /* temperature: always C -> F */
  float f = celcius_to_farhenheit(last_baro.temp_c);
  snprintf(buf, sizeof(buf), "%.1f", (double)f);
  lv_subject_copy_string(&temp_str, buf);

  /* pressure: hPa */
  snprintf(buf, sizeof(buf), "%.0f", (double)last_baro.pressure_hpa);
  lv_subject_copy_string(&press_str, buf);

  /* altitude: m */
  snprintf(buf, sizeof(buf), "%.0f", (double)last_baro.altitude_m);
  lv_subject_copy_string(&alt_str, buf);

  if (have_prev) {
    float dp = last_baro.pressure_hpa - prev_press_hpa;
    lv_subject_copy_string(&press_trend, (dp > 0.2f)    ? "Rising"
                                         : (dp < -0.2f) ? "Falling"
                                                        : "Steady");
  }
}

void sensors_baro_update(const struct baro_data *baro) {
  if (have_prev)
    prev_press_hpa = last_baro.pressure_hpa;
  last_baro = *baro;
  reformat();
  have_prev = true;
}

static void ui_service_timer(lv_timer_t *t) {
  int req = atomic_set(&screen_request, -1);
  if (req >= 0 && req < 4)
    lv_subject_set_int(&active_screen, req);

  struct baro_data baro;
  if (bmp581_get(&baro))
    sensors_baro_update(&baro);

  /* motion: pull your fused AHRS here, throttled to this 10 Hz tick */
  /* int hdg, roll, pitch; if (ahrs_get(&hdg, &roll, &pitch)) motion_update(hdg,
   * roll, pitch); */

  home_tick();
}

int lvgl_display_init(void) {
  if (!device_is_ready(display_dev))
    return -ENODEV;

  display_init_gen(NULL); /* registers all fonts, styles, subjects, consts */

  /* light theme base — only covers widget parts your XML styles don't set
     (switch knob, scrollbars). The false is the dark flag. */
  lv_display_t *disp = lv_display_get_default();
  lv_theme_t *th = lv_theme_default_init(disp, lv_color_hex(0x2E5BFF),
                                         lv_color_hex(0x2E5BFF), false,
                                         &lv_font_montserrat_14);
  lv_display_set_theme(disp, th);

  screens[0] = home_create();
  screens[1] = barometer_create();
  screens[2] = motion_create();
  screens[3] = settings_create();

  lv_subject_add_observer(&active_screen, on_active_screen, NULL);

  home_bind_static();
  motion_ui_init(screens[2]); /* builds needle + horizon */

  lv_subject_set_int(&active_screen, 0); /* load home */
  display_blanking_off(display_dev);
  lv_timer_create(ui_service_timer, 100, NULL);
  return 0;
}

void display_tick(void) { lv_timer_handler(); }

#ifdef CONFIG_SHELL
#include <stdlib.h>
#include <zephyr/shell/shell.h>
static int cmd_display(const struct shell *sh, size_t argc, char **argv) {
  ui_request_screen(strtoul(argv[1], NULL, 10));
  shell_print(sh, "requested screen %s", argv[1]);
  return 0;
}
SHELL_CMD_ARG_REGISTER(display, NULL, "display <index 0-3>", cmd_display, 2, 2);
#endif
