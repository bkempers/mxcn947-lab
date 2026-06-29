#include "led.h"

#include <zephyr/shell/shell.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(led_manager, LOG_LEVEL_INF);

struct led_blink_ctx {
    struct k_work_delayable work;
    const struct gpio_dt_spec *spec;
    bool active;
    k_timeout_t period;
};

static struct led_blink_ctx blue_blink_ctx;
static struct led_blink_ctx green_blink_ctx;
static struct led_blink_ctx red_blink_ctx;

static void blink_work_handler(struct k_work *work)
{
    struct k_work_delayable *dwork = k_work_delayable_from_work(work);
    struct led_blink_ctx *ctx = CONTAINER_OF(dwork, struct led_blink_ctx, work);

    if (!ctx->active) {
        return; /* stop re-submitting */
    }

    gpio_pin_toggle_dt(ctx->spec);
    k_work_reschedule(&ctx->work, ctx->period);
}

static void blink_init(struct led_blink_ctx *ctx, const struct gpio_dt_spec *spec)
{
    ctx->spec   = spec;
    ctx->active = false;
    ctx->period = K_MSEC(500);
    k_work_init_delayable(&ctx->work, blink_work_handler);
}

static void blink_start(struct led_blink_ctx *ctx)
{
    ctx->active = true;
    k_work_reschedule(&ctx->work, K_NO_WAIT);
}

static void blink_stop(struct led_blink_ctx *ctx)
{
    ctx->active = false;
    k_work_cancel_delayable_sync(&ctx->work, &(struct k_work_sync){});
    /* turn LED off so it's in a known state */
    gpio_pin_set_dt(ctx->spec, 0);
}

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
    ret  = gpio_pin_configure_dt(&red_led,   GPIO_OUTPUT_INACTIVE);
    ret |= gpio_pin_configure_dt(&green_led, GPIO_OUTPUT_INACTIVE);
    ret |= gpio_pin_configure_dt(&blue_led,  GPIO_OUTPUT_INACTIVE);

    blink_init(&blue_blink_ctx, &blue_led);
    blink_init(&green_blink_ctx, &green_led);
    blink_init(&red_blink_ctx, &red_led);

    return ret;
}

void led_error()
{
    LOG_ERR("error...");
}

/** LED command functions */

// BLUE
static int cmd_blue_toggle(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc); ARG_UNUSED(argv);
    gpio_pin_toggle_dt(&blue_led);
    shell_print(sh, "Blue toggled");
    return 0;
}
static int cmd_blue_blink(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc); ARG_UNUSED(argv);
    blink_start(&blue_blink_ctx);
    shell_print(sh, "Blue blinking (500ms)");
    return 0;
}
static int cmd_blue_stop(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc); ARG_UNUSED(argv);
    blink_stop(&blue_blink_ctx);
    shell_print(sh, "Blue stopped");
    return 0;
}

// GREEN
static int cmd_green_toggle(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc); ARG_UNUSED(argv);
    gpio_pin_toggle_dt(&green_led);
    shell_print(sh, "Green toggled");
    return 0;
}
static int cmd_green_blink(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc); ARG_UNUSED(argv);
    blink_start(&green_blink_ctx);
    shell_print(sh, "Green blinking (500ms)");
    return 0;
}
static int cmd_green_stop(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc); ARG_UNUSED(argv);
    blink_stop(&green_blink_ctx);
    shell_print(sh, "Green stopped");
    return 0;
}

// RED
static int cmd_red_toggle(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc); ARG_UNUSED(argv);
    gpio_pin_toggle_dt(&red_led);
    shell_print(sh, "Red toggled");
    return 0;
}
static int cmd_red_blink(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc); ARG_UNUSED(argv);
    blink_start(&red_blink_ctx);
    shell_print(sh, "Red blinking (500ms)");
    return 0;
}
static int cmd_red_stop(const struct shell *sh, size_t argc, char **argv)
{
    ARG_UNUSED(argc); ARG_UNUSED(argv);
    blink_stop(&red_blink_ctx);
    shell_print(sh, "Red stopped");
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(blue_cmds,
    SHELL_CMD(toggle, NULL, "Toggle blue LED",       cmd_blue_toggle),
    SHELL_CMD(blink,  NULL, "Start blinking blue",   cmd_blue_blink),
    SHELL_CMD(stop,   NULL, "Stop blinking blue",    cmd_blue_stop),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(green_cmds,
    SHELL_CMD(toggle, NULL, "Toggle green LED",       cmd_green_toggle),
    SHELL_CMD(blink,  NULL, "Start blinking green",   cmd_green_blink),
    SHELL_CMD(stop,   NULL, "Stop blinking green",    cmd_green_stop),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(red_cmds,
    SHELL_CMD(toggle, NULL, "Toggle red LED",       cmd_red_toggle),
    SHELL_CMD(blink,  NULL, "Start blinking red",   cmd_red_blink),
    SHELL_CMD(stop,   NULL, "Stop blinking red",    cmd_red_stop),
    SHELL_SUBCMD_SET_END
);

SHELL_STATIC_SUBCMD_SET_CREATE(led_cmd,
    SHELL_CMD(blue,  &blue_cmds,  "Blue LED management",  NULL),
    SHELL_CMD(green, &green_cmds, "Green LED management",  NULL),
    SHELL_CMD(red,   &red_cmds,   "Red LED management",   NULL),
    SHELL_SUBCMD_SET_END
);
SHELL_CMD_REGISTER(led, &led_cmd, "LED commands", NULL);
