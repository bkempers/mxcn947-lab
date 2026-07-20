#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <math.h>

#include "bmp581.h"
#include "../../common.h"

LOG_MODULE_REGISTER(baro_bmp581, CONFIG_APP_LOG_LEVEL);

#define BMP581_PERIOD K_MSEC(2000)
#define BMP581_STALE_MS 5000

static const struct device *const dev = DEVICE_DT_GET(DT_ALIAS(baro_bmp581));

static struct baro_data data;
static K_MUTEX_DEFINE(data_lock);

static float sea_level_hpa = 1018.5f;

void bmp581_set_sea_level_hpa(float hpa)
{
	if (hpa > 800.0f && hpa < 1200.0f) {
		sea_level_hpa = hpa;
		LOG_INF("sea level reference: %.2f hPa", (double)hpa);
	} else {
		LOG_WRN("ignoring implausible reference %.2f hPa", (double)hpa);
	}
}

static float to_altitude_m(float hpa)
{
	return 44330.0f * (1.0f - powf(hpa / sea_level_hpa, 0.1903f));
}

static int read_once(struct baro_data *out)
{
	struct sensor_value press, temp;
	int rc;

	rc = sensor_sample_fetch(dev);
	if (rc != 0) {
		return rc;
	}

	rc = sensor_channel_get(dev, SENSOR_CHAN_PRESS, &press);
	if (rc != 0) {
		return rc;
	}

	out->pressure_hpa = sensor_value_to_float(&press) * 10.0f;
	out->altitude_m   = to_altitude_m(out->pressure_hpa);

	/* Temperature is a bonus - a failure here should not throw away a
	 * perfectly good pressure reading.
	 */
	if (sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp) == 0) {
		out->temp_c = sensor_value_to_float(&temp);
	}

	out->timestamp_ms = k_uptime_get();
	return 0;
}

static void poll_work_fn(struct k_work *work);
static K_WORK_DELAYABLE_DEFINE(poll_work, poll_work_fn);

static void poll_work_fn(struct k_work *work)
{
	struct baro_data fresh = {0};
	static uint32_t consecutive_fails;
	int rc = read_once(&fresh);

	if (rc == 0) {
		if (consecutive_fails > 0) {
			LOG_INF("recovered after %u failures", consecutive_fails);
			consecutive_fails = 0;
		}

		k_mutex_lock(&data_lock, K_FOREVER);
		data = fresh;
		k_mutex_unlock(&data_lock);
	} else if (++consecutive_fails == 5) {
		/* Log once on the way down, not every cycle. */
		LOG_ERR("read failing (%d)", rc);
	}

	/* Reschedule regardless: a sensor that comes back should recover on
	 * its own without a reboot.
	 */
	k_work_schedule(&poll_work, BMP581_PERIOD);
}

int bmp581_init(void)
{
	struct baro_data probe = {0};
	int rc;

	if (!device_is_ready(dev)) {
		LOG_ERR("%s not ready", dev->name);
		return -ENODEV;
	}

	/* device_is_ready() only proves the driver bound. Prove the chip
	 * answers, and that what it says is physically possible - a
	 * marginal pull-up or a half-seated cable shows up right here
	 * rather than as odd numbers on screen an hour later.
	 */
	rc = read_once(&probe);
	if (rc != 0) {
		LOG_ERR("probe read failed (%d)", rc);
		return rc;
	}
	if (probe.pressure_hpa < 300.0f || probe.pressure_hpa > 1200.0f) {
		LOG_ERR("implausible probe reading %.1f hPa",
			(double)probe.pressure_hpa);
		return -EIO;
	}

	k_mutex_lock(&data_lock, K_FOREVER);
	data = probe;
	k_mutex_unlock(&data_lock);

	LOG_INF("ready: %.1f hPa, %.0f m, %.1f C",
		(double)probe.pressure_hpa, (double)probe.altitude_m,
		(double)probe.temp_c);

	k_work_schedule(&poll_work, BMP581_PERIOD);
	return 0;
}

bool bmp581_get(struct baro_data *out)
{
	k_mutex_lock(&data_lock, K_FOREVER);
	*out = data;
	k_mutex_unlock(&data_lock);

	return out->timestamp_ms != 0 && (k_uptime_get() - out->timestamp_ms) < BMP581_STALE_MS;
}

#ifdef CONFIG_SHELL

#include <zephyr/shell/shell.h>
#include <stdlib.h>

static int cmd_baro(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	struct baro_data d;
	bool fresh = bmp581_get(&d);

	if (d.timestamp_ms == 0) {
		shell_warn(sh, "no reading yet");
		return 0;
	}

	shell_print(sh, "%.1f hPa   %.0f m   %.1f C",
		    (double)d.pressure_hpa, (double)d.altitude_m,
		    (double)d.temp_c);
	shell_print(sh, "age %lld ms%s   sea level %.2f hPa",
		    k_uptime_get() - d.timestamp_ms, fresh ? "" : "  [STALE]",
		    (double)sea_level_hpa);
	return 0;
}

static int cmd_sealevel(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);

	bmp581_set_sea_level_hpa(strtof(argv[1], NULL));
	shell_print(sh, "sea level now %.2f hPa", (double)sea_level_hpa);
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_baro,
    SHELL_CMD(read, NULL, "Read BMP581 sensor", cmd_baro),
	SHELL_CMD_ARG(sealevel, NULL, "Set reference: baro sealevel <hPa>", cmd_sealevel, 2, 0),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(baro, &sub_baro, "Barometer BMP581 commands", NULL);

#endif /* CONFIG_SHELL */
