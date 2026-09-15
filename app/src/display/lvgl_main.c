#include <lvgl.h>
#include <math.h>
#include <stdio.h>
#include <zephyr/app_version.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>
#include <zephyr/version.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(display, LOG_LEVEL_INF);

#include "display_gen.h"

#include "../sensors/baro/bmp581.h"
#include "../sensors/imu/lsm6dsox.h"
#include "../sensors/mag/lis3mdl_m.h"
#include "../sensors/mag/compass.h"
#include "screens/motion.h"
#include "nav.h"

const struct device *display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));

static atomic_t screen_request = ATOMIC_INIT(-1);

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
}

void sensors_baro_update(const struct baro_data *baro) {
  last_baro = *baro;
  reformat();
}

static void ui_service_timer(lv_timer_t *t) {
  int req = atomic_set(&screen_request, -1);
  if (req >= 0 && nav_get_screen(req) != NULL)
    lv_subject_set_int(&active_screen, req);

  struct baro_data baro;
  if (bmp581_get(&baro))
    sensors_baro_update(&baro);

  /* motion: fused compass heading + roll/pitch from IMU + mag */
  struct imu_data imu;
  struct mag_data mag;
  if (lsm6dsox_get(&imu) && lis3mdl_m_get(&mag)) {
    float hdg = compass_heading(mag.mag, imu.accel, 0.0f);
    float roll = atan2f(imu.accel[1], imu.accel[2]) * (180.0f / 3.14159f);
    float pitch =
        atan2f(-imu.accel[0],
               sqrtf(imu.accel[1] * imu.accel[1] + imu.accel[2] * imu.accel[2])) *
        (180.0f / 3.14159f);
    motion_update((int)hdg, (int)roll, (int)pitch);
  }

  home_tick();
}

int lvgl_display_init(void) {
  if (!device_is_ready(display_dev))
    return -ENODEV;

  display_init_gen(NULL); /* registers all fonts, styles, subjects, consts */

  /* light theme base — only covers widget parts your XML styles don't set
     (switch knob, scrollbars). The false is the dark flag. */
  lv_display_t *disp = lv_display_get_default();
  lv_theme_t *th = lv_theme_default_init(disp, lv_color_hex(0x2E5BFF), lv_color_hex(0x2E5BFF), false, &lv_font_montserrat_14);
  lv_display_set_theme(disp, th);

  /* register screens — nav_init() creates them, builds overlay & observer */
  nav_register(0, "Home",      0xf015, "Main",    home_create);
  nav_register(1, "Barometer", 0xf6fc, "Sensors", barometer_create);
  nav_register(2, "Motion",    0xf14e, "Sensors", motion_create);
  nav_register(3, "Settings",  0xf013, "System",  settings_create);
  nav_init();

  home_bind_static();
  motion_ui_init(nav_get_screen(2));

  /* render the first frame on the normal path, then reveal */
  lv_timer_handler();
  display_blanking_off(display_dev);

  lv_timer_create(ui_service_timer, 100, NULL);
  return 0;
}

#ifdef CONFIG_SHELL
#include <stdlib.h>
#include <zephyr/shell/shell.h>
static int cmd_display(const struct shell *sh, size_t argc, char **argv) {
  ui_request_screen(strtoul(argv[1], NULL, 10));
  shell_print(sh, "requested screen %s", argv[1]);
  return 0;
}
SHELL_CMD_ARG_REGISTER(display, NULL, "display <screen index>", cmd_display, 2, 2);
#endif
