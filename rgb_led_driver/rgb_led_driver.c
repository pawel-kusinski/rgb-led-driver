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

#include "rgb_led_driver.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct _PwmSetDutyCycleFunctions {
    SetPwmDutyCycleFunction set_duty_cycle_r;
    SetPwmDutyCycleFunction set_duty_cycle_g;
    SetPwmDutyCycleFunction set_duty_cycle_b;
} PwmSetDutyCycleFunctions;

typedef struct _DutyCycle {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} DutyCycle;

typedef struct _Rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Rgb;

struct RgbLedDrvHandle {
    RgbLedCfg cfg;
    PwmSetDutyCycleFunctions set_pwm_duty_cycle;
    DutyCycle duty_cycle;
    bool is_turned_on;
};

static const Rgb color_definitions[RGB_LED_COLOR_CUSTOM] = {
    {255, 0,   0  },
    {0,   255, 0  },
    {0,   0,   255},
    {255, 255, 0  },
    {0,   255, 255},
    {255, 0,   255},
    {255, 255, 255},
};

static void  setDutyCycleForAllComponents(RgbLed led, uint8_t duty_cycle);
static uint8_t convertRgbComponentValueToDutyCycle(uint8_t component, RgbLedCfg cfg);

static void setDutyCycleForAllComponents(RgbLed led, uint8_t duty_cycle) {
    led->set_pwm_duty_cycle.set_duty_cycle_r(duty_cycle);
    led->set_pwm_duty_cycle.set_duty_cycle_g(duty_cycle);
    led->set_pwm_duty_cycle.set_duty_cycle_b(duty_cycle);
}

static uint8_t convertRgbComponentValueToDutyCycle(uint8_t component, RgbLedCfg cfg) {
    uint8_t duty_cycle = component * 100 / 255;

    if (RGB_LED_CFG_COMM_ANODE == cfg) {
        duty_cycle = 100 - duty_cycle;
    }

    return duty_cycle; 
}

RgbLed RgbLedDrv_create(SetPwmDutyCycleFunction set_pwm_r, SetPwmDutyCycleFunction set_pwm_g, SetPwmDutyCycleFunction set_pwm_b,
                        RgbLedCfg cfg, RgbLedColor color, uint8_t r, uint8_t g, uint8_t b, bool initial_state) {
    if (NULL == set_pwm_r || NULL == set_pwm_g || NULL == set_pwm_b) {
        return RGB_LED_DRV_INVALID_OBJECT;
    }

    if (cfg != RGB_LED_CFG_COMM_ANODE && cfg != RGB_LED_CFG_COMM_CATHODE) {
        return RGB_LED_DRV_INVALID_OBJECT;
    }

    DutyCycle initial_duty_cycle;

    if (RGB_LED_COLOR_CUSTOM == color) {
        initial_duty_cycle.r = convertRgbComponentValueToDutyCycle(r, cfg);
        initial_duty_cycle.g = convertRgbComponentValueToDutyCycle(g, cfg);
        initial_duty_cycle.b = convertRgbComponentValueToDutyCycle(b, cfg);
    } else {
        if (color >= sizeof(color_definitions) / sizeof(*color_definitions)) {
            return RGB_LED_DRV_INVALID_OBJECT;
        } else {
            initial_duty_cycle.r = convertRgbComponentValueToDutyCycle(color_definitions[color].r, cfg);
            initial_duty_cycle.g = convertRgbComponentValueToDutyCycle(color_definitions[color].g, cfg);
            initial_duty_cycle.b = convertRgbComponentValueToDutyCycle(color_definitions[color].b, cfg);
        }
    }

    RgbLed led = calloc(1, sizeof(struct RgbLedDrvHandle));

    if (led) {
        led->is_turned_on = initial_state;
        led->cfg = cfg;
        led->set_pwm_duty_cycle.set_duty_cycle_r = set_pwm_r;
        led->set_pwm_duty_cycle.set_duty_cycle_g = set_pwm_g;
        led->set_pwm_duty_cycle.set_duty_cycle_b = set_pwm_b;
        led->duty_cycle = initial_duty_cycle;
        if (led->is_turned_on) {
            led->set_pwm_duty_cycle.set_duty_cycle_r(led->duty_cycle.r);
            led->set_pwm_duty_cycle.set_duty_cycle_g(led->duty_cycle.g);
            led->set_pwm_duty_cycle.set_duty_cycle_b(led->duty_cycle.b);
        } else {
            uint8_t led_inactive_duty_cycle = RGB_LED_CFG_COMM_CATHODE == led->cfg ? 0 : 100;
            setDutyCycleForAllComponents(led, led_inactive_duty_cycle);
        }
    } else {
        return RGB_LED_DRV_INVALID_OBJECT;
    }

    return led;
}

void RgbLedDrv_destroy(RgbLed led) {
    free(led);
}

void RgbLedDrv_turnOn(RgbLed led) {
    if (RGB_LED_DRV_INVALID_OBJECT == led) {
        return;
    }

    led->set_pwm_duty_cycle.set_duty_cycle_r(led->duty_cycle.r);
    led->set_pwm_duty_cycle.set_duty_cycle_g(led->duty_cycle.g);
    led->set_pwm_duty_cycle.set_duty_cycle_b(led->duty_cycle.b);
    led->is_turned_on = true;
}

void RgbLedDrv_turnOff(RgbLed led) {
    if (RGB_LED_DRV_INVALID_OBJECT == led) {
        return;
    }

    uint8_t led_inactive_duty_cycle = RGB_LED_CFG_COMM_CATHODE == led->cfg ? 0 : 100;
    setDutyCycleForAllComponents(led, led_inactive_duty_cycle);
    led->is_turned_on = false;
}

void RgbLedDrv_setPredefinedColor(RgbLed led, RgbLedColor color) {
    if (RGB_LED_DRV_INVALID_OBJECT == led) {
        return;
    }

    if (color >= RGB_LED_COLOR_CUSTOM || color < RGB_LED_COLOR_RED) {
        return;
    }

    led->duty_cycle.r = convertRgbComponentValueToDutyCycle(color_definitions[color].r, led->cfg);
    led->duty_cycle.g = convertRgbComponentValueToDutyCycle(color_definitions[color].g, led->cfg);
    led->duty_cycle.b = convertRgbComponentValueToDutyCycle(color_definitions[color].b, led->cfg);

    if (led->is_turned_on) {
        led->set_pwm_duty_cycle.set_duty_cycle_r(led->duty_cycle.r);
        led->set_pwm_duty_cycle.set_duty_cycle_g(led->duty_cycle.g);
        led->set_pwm_duty_cycle.set_duty_cycle_b(led->duty_cycle.b);
    }
}

void RgbLedDrv_setCustomColor(RgbLed led, uint8_t r, uint8_t g, uint8_t b) {
    if (RGB_LED_DRV_INVALID_OBJECT == led) {
        return;
    }

    led->duty_cycle.r = convertRgbComponentValueToDutyCycle(r, led->cfg);
    led->duty_cycle.g = convertRgbComponentValueToDutyCycle(g, led->cfg);
    led->duty_cycle.b = convertRgbComponentValueToDutyCycle(b, led->cfg);

    if (led->is_turned_on) {
        led->set_pwm_duty_cycle.set_duty_cycle_r(led->duty_cycle.r);
        led->set_pwm_duty_cycle.set_duty_cycle_g(led->duty_cycle.g);
        led->set_pwm_duty_cycle.set_duty_cycle_b(led->duty_cycle.b);
    }
}
