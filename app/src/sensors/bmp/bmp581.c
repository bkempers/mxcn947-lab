#include <zephyr/shell/shell.h>

#include "bmp581.h"
#include "../../common.h"

static const struct device *const bmp581_dev = DEVICE_DT_GET_ONE(bosch_bmp581);

static int cmd_bmp_read(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc); ARG_UNUSED(argv);

    struct sensor_value temp, press;

    if (!device_is_ready(bmp581_dev)) {
        shell_error(sh, "BMP581 not ready");
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(bmp581_dev);
    if (ret) {
        shell_error(sh, "Fetch failed: %d", ret);
        return ret;
    }

    sensor_channel_get(bmp581_dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
    sensor_channel_get(bmp581_dev, SENSOR_CHAN_PRESS, &press);

    shell_print(sh, "Temp: %d.%03d °C", temp.val1, abs(temp.val2));
    shell_print(sh, "Temp: %03f °F", (double)celcius_to_farhenheit(temp));
    shell_print(sh, "Press: %d.%06d kPa", press.val1, abs(press.val2));

    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(bmp_cmds,
    SHELL_CMD(read, NULL, "Read temp & pressure", cmd_bmp_read),
    SHELL_SUBCMD_SET_END
);
SHELL_CMD_REGISTER(bmp, &bmp_cmds, "BMP581 sensor commands", NULL);
