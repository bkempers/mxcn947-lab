#include "led.h"

#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(led_manager, LOG_LEVEL_INF);

int init_led_manager(void)
{
    int ret;

    /*
     * device_is_ready() checks that the GPIO controller driver
     * initialized successfully. Always check this.
     */
    if (!device_is_ready(red_led.port) ||
        !device_is_ready(green_led.port) ||
        !device_is_ready(blue_led.port)) {
        LOG_ERR("LED GPIO port not ready");
        return -ENODEV;
    }

    /*
     * gpio_pin_configure_dt() uses the flags from devicetree
     * (ACTIVE_LOW, etc.) and ORs in the direction you specify.
     * This replaces the STM32 GPIO_InitTypeDef setup.
     */
    ret  = gpio_pin_configure_dt(&red_led,   GPIO_OUTPUT_ACTIVE);
    ret |= gpio_pin_configure_dt(&green_led, GPIO_OUTPUT_INACTIVE);
    ret |= gpio_pin_configure_dt(&blue_led,  GPIO_OUTPUT_INACTIVE);

    return ret;
}

void led_error()
{
    LOG_ERR("error...");
}

static int cmd_led_blue(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    gpio_pin_toggle_dt(&blue_led);
    return 0;
}

static int cmd_led_green(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    gpio_pin_toggle_dt(&green_led);
    return 0;
}

static int cmd_led_red(const struct shell *sh, size_t argc, char **argv)
{

    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    gpio_pin_toggle_dt(&red_led);
    return 0;

    // int cnt;
    //
    // shell_print(sh, "argc = %d", argc);
    // for (cnt = 0; cnt < argc; cnt++) {
    //         shell_print(sh, "  argv[%d] = %s", cnt, argv[cnt]);
    // }
    // return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(led_cmd,
    SHELL_CMD(blue, NULL, "Blue LED management.", cmd_led_blue),
    SHELL_CMD(green, NULL, "Green LED management.", cmd_led_green),
    SHELL_CMD(red, NULL, "Red LED management.", cmd_led_red),
    SHELL_SUBCMD_SET_END
);
SHELL_CMD_REGISTER(led, &led_cmd, "LED command management", NULL);
