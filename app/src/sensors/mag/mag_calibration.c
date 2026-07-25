#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <math.h>
#include "mag_calibration.h"
#ifdef CONFIG_SETTINGS
#include <zephyr/settings/settings.h>
#endif

LOG_MODULE_REGISTER(mag_cal, CONFIG_APP_LOG_LEVEL);

#define MAG_CAL_KEY         "mag/cal"
#define MAG_CAL_MIN_SAMPLES 200

static struct mag_cal cal = { .scale = {1, 1, 1} };
static bool collecting;
static float cmin[3], cmax[3];
static uint32_t csamples;

void mag_cal_apply(float mag[3])
{
    if (collecting) {
        for (int i = 0; i < 3; i++) {
            if (mag[i] < cmin[i]) cmin[i] = mag[i];
            if (mag[i] > cmax[i]) cmax[i] = mag[i];
        }
        csamples++;
    }
    for (int i = 0; i < 3; i++) {
        mag[i] = (mag[i] - cal.off[i]) * cal.scale[i];
    }
}

void mag_cal_start(void)
{
    for (int i = 0; i < 3; i++) { cmin[i] = INFINITY; cmax[i] = -INFINITY; }
    csamples = 0;
    collecting = true;
}

#ifdef CONFIG_SETTINGS
static int cal_load_cb(const char *key, size_t len, settings_read_cb read_cb,
                       void *cb_arg, void *param)
{
    ARG_UNUSED(key); ARG_UNUSED(param);
    if (len != sizeof(cal)) return -EINVAL;
    return read_cb(cb_arg, &cal, sizeof(cal)) < 0 ? -EIO : 0;
}
#endif

int mag_cal_finish(struct mag_cal *out)
{
    collecting = false;
    if (csamples < MAG_CAL_MIN_SAMPLES) return -EAGAIN;

    struct mag_cal fresh;
    float span[3], avg = 0.0f;
    for (int i = 0; i < 3; i++) {
        span[i] = (cmax[i] - cmin[i]) * 0.5f;
        if (span[i] < 1e-4f) return -EINVAL;
        fresh.off[i] = (cmax[i] + cmin[i]) * 0.5f;
        avg += span[i];
    }
    avg /= 3.0f;
    for (int i = 0; i < 3; i++) fresh.scale[i] = avg / span[i];

    cal = fresh;
    if (out) *out = fresh;

#ifdef CONFIG_SETTINGS
    int ret = settings_save_one(MAG_CAL_KEY, &cal, sizeof(cal));
    if (ret) LOG_WRN("cal save failed (%d)", ret);
#endif
    return 0;
}

int mag_cal_init(void)
{
#ifdef CONFIG_SETTINGS
    int ret = settings_subsys_init();
    if (ret) { LOG_WRN("settings init failed (%d)", ret); return ret; }
    ret = settings_load_subtree_direct(MAG_CAL_KEY, cal_load_cb, NULL);
    if (ret) LOG_WRN("cal load failed (%d)", ret);
#endif
    LOG_INF("cal off {%.3f %.3f %.3f} scale {%.3f %.3f %.3f}",
            (double)cal.off[0], (double)cal.off[1], (double)cal.off[2],
            (double)cal.scale[0], (double)cal.scale[1], (double)cal.scale[2]);
    return 0;
}
