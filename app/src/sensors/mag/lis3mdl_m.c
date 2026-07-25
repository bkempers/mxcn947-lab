#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <math.h>

#include "lis3mdl_m.h"
#include "compass.h"
#include "../imu/lsm6dsox.h"
#include "../../common.h"

LOG_MODULE_REGISTER(mag_lis3mdl, CONFIG_APP_LOG_LEVEL);

static const struct device *const mag = DEVICE_DT_GET(DT_ALIAS(mag_lis3mdl));
static struct mag_data data;
static K_MUTEX_DEFINE(data_lock);

#define LIS3MDL_PERIOD K_MSEC(100)
#define LIS3MDL_STALE_MS 1000

static int read(struct mag_data *out)
{
    struct sensor_value mag_xyz[3], die_temp;
    int ret;

    ret = sensor_sample_fetch(mag);
    if (ret != 0) {
        return ret;
    }

    ret = sensor_channel_get(mag, SENSOR_CHAN_MAGN_XYZ, mag_xyz);
    if (ret != 0) {
        return ret;
    }

    out->mag[0] = sensor_value_to_float(&mag_xyz[0]);
    out->mag[1] = sensor_value_to_float(&mag_xyz[1]);
    out->mag[2] = sensor_value_to_float(&mag_xyz[2]);

    mag_cal_apply(out->mag);

    struct imu_data imu;
    if (lsm6dsox_get(&imu)) {
        float hdg = compass_heading(out->mag, imu.accel, 10.0f);
        out->heading = hdg;
    }

    if (sensor_channel_get(mag, SENSOR_CHAN_DIE_TEMP, &die_temp) == 0) {
        out->temp = sensor_value_to_float(&die_temp);
    }

    out->timestamp_ms = k_uptime_get();
    return 0;
}

static void mag_read_fn(struct k_work *work);
static K_WORK_DELAYABLE_DEFINE(mag_read, mag_read_fn);

static void mag_read_fn(struct k_work *work)
{
    struct mag_data fresh = {0};
    static uint32_t consecutive_fails;
    int ret = read(&fresh);

    if (ret == 0) {
        if (consecutive_fails > 0) {
            LOG_INF("recovered after %u failures", consecutive_fails);
            consecutive_fails = 0;
        }

        k_mutex_lock(&data_lock, K_FOREVER);
        data = fresh;
        k_mutex_unlock(&data_lock);
    } else if (++consecutive_fails == 5) {
        LOG_ERR("read failing (%d)", ret);
    }

    k_work_schedule(&mag_read, LIS3MDL_PERIOD);
}

int lis3mdl_m_init(void)
{
    struct mag_data probe = {0};
    int ret;

    if (!device_is_ready(mag)) {
        LOG_ERR("%s not ready", mag->name);
        return -ENODEV;
    }

    mag_cal_init();

    ret = read(&probe);
    if (ret != 0) {
        LOG_ERR("probe read failed (%d)", ret);
        return ret;
    }

    k_mutex_lock(&data_lock, K_FOREVER);
    data = probe;
    k_mutex_unlock(&data_lock);

    LOG_INF("ready: {%.3f, %.3f, %.3f}, %.1f C",
            (double)probe.mag[0], (double)probe.mag[1], (double)probe.mag[2],
            (double)probe.temp);

    k_work_schedule(&mag_read, LIS3MDL_PERIOD);
    return 0;
}

bool lis3mdl_m_get(struct mag_data *out)
{
    k_mutex_lock(&data_lock, K_FOREVER);
    *out = data;
    k_mutex_unlock(&data_lock);

    return out->timestamp_ms != 0 && (k_uptime_get() - out->timestamp_ms < LIS3MDL_STALE_MS);
}

#ifdef CONFIG_SHELL

#include <zephyr/shell/shell.h>
#include <stdlib.h>

static int cmd_cal_start(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc); ARG_UNUSED(argv);
    mag_cal_start();
    shell_print(sh, "collecting — rotate board through all orientations, then 'mag cal finish'");
    return 0;
}

static int cmd_cal_finish(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc); ARG_UNUSED(argv);
    struct mag_cal c;
    int ret = mag_cal_finish(&c);
    if (ret == -EAGAIN) { shell_warn(sh, "not enough samples — rotate more"); return 0; }
    if (ret == -EINVAL) { shell_warn(sh, "an axis never moved — rotate more fully"); return 0; }
    if (ret)            { shell_error(sh, "cal failed (%d)", ret); return ret; }
    shell_print(sh, "off {%.3f %.3f %.3f} scale {%.3f %.3f %.3f}",
                (double)c.off[0], (double)c.off[1], (double)c.off[2],
                (double)c.scale[0], (double)c.scale[1], (double)c.scale[2]);
    return 0;
}

static int cmd_mag(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	struct mag_data d;
	bool fresh = lis3mdl_m_get(&d);

	if (d.timestamp_ms == 0) {
		shell_warn(sh, "no reading yet");
		return 0;
	}

    shell_print(sh, "heading %f, {%.3f, %.3f, %.3f}, %.1f C",
            (double)d.heading,
            (double)d.mag[0], (double)d.mag[1], (double)d.mag[2],
            (double)d.temp);
	shell_print(sh, "age %lld ms%s",
		    k_uptime_get() - d.timestamp_ms, fresh ? "" : "  [STALE]");
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_cal,
    SHELL_CMD(start,  NULL, "Begin mag calibration", cmd_cal_start),
    SHELL_CMD(finish, NULL, "Compute, apply, persist", cmd_cal_finish),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(sub_mag,
    SHELL_CMD(read, NULL, "Read LIS3MDL sensor", cmd_mag),
    SHELL_CMD(cal, &sub_cal, "Calibration", NULL),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(mag, &sub_mag, "Magnetomer LIS3MDL commands", NULL);

#endif /* CONFIG_SHELL */
