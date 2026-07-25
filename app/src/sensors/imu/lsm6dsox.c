#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <math.h>

#include "lsm6dsox.h"
#include "../../common.h"

LOG_MODULE_REGISTER(imu_lsm6dsox, CONFIG_APP_LOG_LEVEL);

static const struct device *const imu = DEVICE_DT_GET(DT_ALIAS(imu_lsm6dsox));
static struct imu_data data;
static K_MUTEX_DEFINE(data_lock);

#define LSM6DSOX_PERIOD K_MSEC(100)
#define LSM6DSOX_STALE_MS 1000


static int read(struct imu_data *out)
{
    struct sensor_value accel[3], gyro[3];
    int ret;

    ret = sensor_sample_fetch(imu);
    if (ret != 0) {
        return ret;
    }

    ret = sensor_channel_get(imu, SENSOR_CHAN_ACCEL_XYZ, accel);
    if (ret != 0) {
        return ret;
    }

    out->accel[0] = sensor_value_to_float(&accel[0]);
    out->accel[1] = sensor_value_to_float(&accel[1]);
    out->accel[2] = sensor_value_to_float(&accel[2]);


    ret = sensor_channel_get(imu, SENSOR_CHAN_GYRO_XYZ, gyro);
    if (ret != 0) {
        return ret;
    }

    out->gyro[0] = sensor_value_to_float(&gyro[0]);
    out->gyro[1] = sensor_value_to_float(&gyro[1]);
    out->gyro[2] = sensor_value_to_float(&gyro[2]);

    out->timestamp_ms = k_uptime_get();
    return 0;
}


static void imu_read_fn(struct k_work *work);
static K_WORK_DELAYABLE_DEFINE(imu_read, imu_read_fn);

static void imu_read_fn(struct k_work *work)
{
    struct imu_data fresh = {0};
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

    k_work_schedule(&imu_read, LSM6DSOX_PERIOD);
}


int lsm6dsox_init(void)
{
    struct imu_data probe = {0};
    int ret;

    if (!device_is_ready(imu)) {
        LOG_ERR("%s not ready", imu->name);
        return -ENODEV;
    }

    ret = read(&probe);
    if (ret != 0) {
        LOG_ERR("probe read failed (%d)", ret);
        return ret;
    }

    k_mutex_lock(&data_lock, K_FOREVER);
    data = probe;
    k_mutex_unlock(&data_lock);

    LOG_INF("ready: accel {%.3f, %.3f, %.3f}, gyro {%.3f, %.3f, %.3f}",
            (double)probe.accel[0], (double)probe.accel[1], (double)probe.accel[2],
            (double)probe.gyro[0], (double)probe.gyro[1], (double)probe.gyro[2]);

    k_work_schedule(&imu_read, LSM6DSOX_PERIOD);
    return 0;
}

bool lsm6dsox_get(struct imu_data *out)
{
    k_mutex_lock(&data_lock, K_FOREVER);
    *out = data;
    k_mutex_unlock(&data_lock);

    return out->timestamp_ms != 0 && (k_uptime_get() - out->timestamp_ms < LSM6DSOX_STALE_MS);
}


#ifdef CONFIG_SHELL

#include <zephyr/shell/shell.h>
#include <stdlib.h>

static int cmd_imu(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	struct imu_data d;
	bool fresh = lsm6dsox_get(&d);

	if (d.timestamp_ms == 0) {
		shell_warn(sh, "no reading yet");
		return 0;
	}

    shell_print(sh, "accel {%.3f, %.3f, %.3f}, gyro {%.3f, %.3f, %.3f}",
            (double)d.accel[0], (double)d.accel[1], (double)d.accel[2],
            (double)d.gyro[0], (double)d.gyro[1], (double)d.gyro[2]);
	shell_print(sh, "age %lld ms%s",
		    k_uptime_get() - d.timestamp_ms, fresh ? "" : "  [STALE]");
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_imu,
    SHELL_CMD(read, NULL, "Read LSM6DSOX sensor", cmd_imu),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(imu, &sub_imu, "Magnetomer LSM6DSOX commands", NULL);

#endif /* CONFIG_SHELL */
