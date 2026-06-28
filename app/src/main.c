#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#include "led/led.h"

LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

// static const struct gpio_dt_spec button    = GPIO_DT_SPEC_GET(DT_ALIAS(sw0),  gpios);
//
// /*
//  * GPIO interrupt callback — fires on button press.
//  * This is the Zephyr equivalent of an ISR for EXTI on STM32.
//  */
// static struct gpio_callback button_cb_data;
//
// static void button_pressed(const struct device *dev,
//                            struct gpio_callback *cb,
//                            uint32_t pins)
// {
//     /* Toggle blue LED on each press */
//     gpio_pin_toggle_dt(&blue_led);
//     LOG_INF("button pressed — blue LED toggled");
// }

// static int init_button(void)
// {
//     int ret;
//
//     if (!device_is_ready(button.port)) {
//         LOG_ERR("Button GPIO port not ready");
//         return -ENODEV;
//     }
//
//     ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
//     if (ret < 0) {
//         return ret;
//     }
//
//     /*
//      * Set up edge-triggered interrupt on the button pin.
//      * GPIO_INT_EDGE_TO_ACTIVE respects the ACTIVE_LOW flag
//      * from devicetree — so this fires on the physical press,
//      * not on a specific voltage edge. Nice abstraction.
//      */
//     ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
//     if (ret < 0) {
//         return ret;
//     }
//
//     gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
//     gpio_add_callback(button.port, &button_cb_data);
//
//     return 0;
// }

int main(void)
{
    LOG_INF("GPIO demo on %s", CONFIG_BOARD_TARGET);

    if (init_led_manager() < 0) {
        LOG_ERR("led manager init failed");
        return 0;
    }

    // if (init_leds() < 0) {
    //     LOG_ERR("init failed");
    //     return 0;
    // }

    // if (init_leds() < 0 || init_button() < 0) {
    //     LOG_ERR("init failed");
    //     return 0;
    // }

    while (1) {
        k_sleep(K_MSEC(500));
    }

    return 0;
}
