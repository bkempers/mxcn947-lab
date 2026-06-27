#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

/*
 * GPIO_DT_SPEC_GET pulls pin number, port, and flags directly
 * from devicetree. No magic numbers in your source.
 *
 * DT_ALIAS(led0) resolves to whatever the board DTS maps led0 to.
 * On FRDM-MCXN947: red_led on gpio0.
 */
static const struct gpio_dt_spec red_led   = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec green_led = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);
static const struct gpio_dt_spec blue_led  = GPIO_DT_SPEC_GET(DT_ALIAS(led2), gpios);
static const struct gpio_dt_spec button    = GPIO_DT_SPEC_GET(DT_ALIAS(sw0),  gpios);

/*
 * GPIO interrupt callback — fires on button press.
 * This is the Zephyr equivalent of an ISR for EXTI on STM32.
 */
static struct gpio_callback button_cb_data;

static void button_pressed(const struct device *dev,
                           struct gpio_callback *cb,
                           uint32_t pins)
{
    /* Toggle blue LED on each press */
    gpio_pin_toggle_dt(&blue_led);
    LOG_INF("button pressed — blue LED toggled");
}

static int init_leds(void)
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

static int init_button(void)
{
    int ret;

    if (!device_is_ready(button.port)) {
        LOG_ERR("Button GPIO port not ready");
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret < 0) {
        return ret;
    }

    /*
     * Set up edge-triggered interrupt on the button pin.
     * GPIO_INT_EDGE_TO_ACTIVE respects the ACTIVE_LOW flag
     * from devicetree — so this fires on the physical press,
     * not on a specific voltage edge. Nice abstraction.
     */
    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret < 0) {
        return ret;
    }

    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
    gpio_add_callback(button.port, &button_cb_data);

    return 0;
}

int main(void)
{
    LOG_INF("GPIO demo on %s", CONFIG_BOARD_TARGET);

    if (init_leds() < 0 || init_button() < 0) {
        LOG_ERR("init failed");
        return 0;
    }

    /*
     * Main loop just blinks the red LED.
     * Button interrupt toggles blue independently.
     */
    while (1) {
        gpio_pin_toggle_dt(&red_led);
        k_sleep(K_MSEC(100));
    }

    return 0;
}
