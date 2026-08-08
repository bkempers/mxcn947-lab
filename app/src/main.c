#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/util.h>

#include "led/led.h"
#include "sensors/baro/bmp581.h"
#include "sensors/mag/lis3mdl_m.h"
#include "sensors/imu/lsm6dsox.h"
#include "display/lvgl_main.h"

LOG_MODULE_REGISTER(mxcn947_lab, LOG_LEVEL_INF);

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
    LOG_INF("starting zephyr lab on %s", CONFIG_BOARD_TARGET);

    if (bmp581_init() != 0) {
        LOG_ERR("bmp581 init failed");
        return 0;
    }

    if (lis3mdl_m_init() != 0) {
        LOG_ERR("lis3mdl init failed");
        return 0;
    }

    if (lsm6dsox_init() != 0) {
        LOG_ERR("lsm6dsox init failed");
        return 0;
    }

    if (led_init() != 0) {
        LOG_ERR("led manager init failed");
        return 0;
    }

    if (lvgl_display_init() != 0) {
        printk("LVGL UI init failed\n");
        return 0;
    }

    // if (init_leds() < 0 || init_button() < 0) {
    //     LOG_ERR("init failed");
    //     return 0;
    // }

    while (1) {
        display_tick();   /* process LVGL redraws */
        k_msleep(10);
    }

    return 0;
}
