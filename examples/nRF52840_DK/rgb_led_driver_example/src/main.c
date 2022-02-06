/*==========================================================================================================*\
 * MIT License
 *
 * Copyright (c) 2022 Pawel Kusinski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
\*==========================================================================================================*/

#include <stdint.h>
#include <stdbool.h>

#include <zephyr.h>
#include <sys/printk.h>
#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include <drivers/pwm.h>
#include <kernel.h>

#include "rgb_led_driver.h"

extern bool run_example_sequence;
extern RgbLed my_led;
RgbLed my_led = NULL;
bool run_example_sequence = true;
static const struct device *pwm_dev;

static bool initializePwm0(void);
static void setDutyCycleLedRed(uint8_t duty_cycle);
static void setDutyCycleLedGreen(uint8_t duty_cycle);
static void setDutyCycleLedBlue(uint8_t duty_cycle);
static void runExampleSequence(RgbLed led);

void main(void) {
    if (!initializePwm0()) {
        printk("Failed to initialized PWM_0.\n");
        goto fail;
    }

    setDutyCycleLedRed(100);
    setDutyCycleLedGreen(100);
    setDutyCycleLedBlue(100);

    my_led = RgbLedDrv_create(setDutyCycleLedRed, setDutyCycleLedGreen, setDutyCycleLedBlue,
                              RGB_LED_CFG_COMM_ANODE, RGB_LED_COLOR_CUSTOM, 255, 0, 0, true);

    if (RGB_LED_DRV_INVALID_OBJECT == my_led) {
        printk("Failed to create LED1.\n");
        goto fail;
    }

    while (1) {
        if (run_example_sequence) {
            runExampleSequence(my_led);
        } else {
            k_sleep(K_MSEC(100));
        }
        
    }

fail:
    while(1) {
        k_sleep(K_MSEC(100));
    }

    RgbLedDrv_destroy(my_led);
}

static bool initializePwm0(void) {
    pwm_dev = device_get_binding("PWM_0");

    if (!pwm_dev) {
        return false;
    }

    return true;
}

static void setDutyCycleLedRed(uint8_t duty_cycle) {
    if (duty_cycle > 100) {
        printk("%s: Invalid duty cycle %u.\n", __func__, duty_cycle);
        return;
    }

    if (pwm_pin_set_usec(pwm_dev, 14, 100, duty_cycle, 0)) {
        printk("%s: pwm_pin_set_usec failed.\n", __func__);
    }
}

static void setDutyCycleLedGreen(uint8_t duty_cycle) {
    if (duty_cycle > 100) {
        printk("%s: Invalid duty cycle %u.\n", __func__, duty_cycle);
        return;
    }

    if (pwm_pin_set_usec(pwm_dev, 15, 100, duty_cycle, 0)) {
        printk("%s: pwm_pin_set_usec failed.\n", __func__);
    }
}

static void setDutyCycleLedBlue(uint8_t duty_cycle) {
    if (duty_cycle > 100) {
        printk("%s: Invalid duty cycle %u.\n", __func__, duty_cycle);
        return;
    }

    if (pwm_pin_set_usec(pwm_dev, 16, 100, duty_cycle, 0)) {
        printk("%s: pwm_pin_set_usec failed.\n", __func__);
    }
}

static void runExampleSequence(RgbLed led) {
    struct rgb_t {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    static const struct rgb_t rainbow[12] = {
        {.r = 255, .g = 0,   .b = 0},
        {.r = 255, .g = 127, .b = 0},
        {.r = 255, .g = 255, .b = 0},
        {.r = 127, .g = 255, .b = 0},
        {.r = 0,   .g = 255, .b = 0},
        {.r = 0,   .g = 255, .b = 127},
        {.r = 0,   .g = 255, .b = 255},
        {.r = 0,   .g = 127, .b = 255},
        {.r = 0,   .g = 0,   .b = 255},
        {.r = 127, .g = 0,   .b = 255},
        {.r = 255, .g = 0,   .b = 255},
        {.r = 255, .g = 0,   .b = 127},
    };

    unsigned i;

    for (i = 0; i < 12; ++i) {
        RgbLedDrv_setCustomColor(led, rainbow[i].r, rainbow[i].g, rainbow[i].b);
        k_sleep(K_MSEC(100));
    }
}
