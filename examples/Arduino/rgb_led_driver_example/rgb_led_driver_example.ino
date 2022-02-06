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

int pwm_r_pin = 9;
int pwm_g_pin = 10;
int pwm_b_pin = 11;

RgbLed my_led = NULL;

void setDutyCycleLedRed(uint8_t duty_cycle);
void setDutyCycleLedGreen(uint8_t duty_cycle);
void setDutyCycleLedBlue(uint8_t duty_cycle);
uint8_t scaleDutyCycleParamForAnalogWriteFunction(uint8_t duty_cycle);
void runRainbowSequence(RgbLed led);

void setup() {
    Serial.begin(9600);
    pinMode(pwm_r_pin, OUTPUT);
    pinMode(pwm_g_pin, OUTPUT);
    pinMode(pwm_b_pin, OUTPUT);
    my_led = RgbLedDrv_create(setDutyCycleLedRed, setDutyCycleLedGreen, setDutyCycleLedBlue,
                              RGB_LED_CFG_COMM_ANODE, RGB_LED_COLOR_RED, 0, 0, 0, false);
}

void loop() {
    static int i = 0;
    if (my_led) {  
        Serial.println("Setting red color for 1000 ms.");
        RgbLedDrv_setPredefinedColor(my_led, RGB_LED_COLOR_RED);
        RgbLedDrv_turnOn(my_led);
        delay(1000);

        Serial.println("Setting green color for 1000 ms.");
        RgbLedDrv_setPredefinedColor(my_led, RGB_LED_COLOR_GREEN);
        delay(1000);

        Serial.println("Setting blue color for 1000 ms.");
        RgbLedDrv_setPredefinedColor(my_led, RGB_LED_COLOR_BLUE);
        delay(1000);

        Serial.println("Setting yellow color for 1000 ms.");
        RgbLedDrv_setPredefinedColor(my_led, RGB_LED_COLOR_YELLOW);
        delay(1000);

        Serial.println("Setting cyan color for 1000 ms.");
        RgbLedDrv_setPredefinedColor(my_led, RGB_LED_COLOR_CYAN);
        delay(1000);

        Serial.println("Setting magenta color for 1000 ms.");
        RgbLedDrv_setPredefinedColor(my_led, RGB_LED_COLOR_MAGENTA);
        delay(1000);

        Serial.println("Setting white color for 1000 ms.");
        RgbLedDrv_setPredefinedColor(my_led, RGB_LED_COLOR_WHITE);
        delay(1000);

        Serial.println("Turning the LED off for 3000 ms.");
        RgbLedDrv_turnOff(my_led);
        delay(3000);

        Serial.println("Running rainbow sequence for 5 times.");
        RgbLedDrv_turnOn(my_led);
        for (unsigned i = 0; i < 5; i++) {
            runRainbowSequence(my_led); 
        }

        Serial.println("Turning the LED off for 3000 ms.");
        RgbLedDrv_turnOff(my_led);
        delay(3000);
    } else {
        for (;;) {
            Serial.println("RGB LED object has not been created successfully.");
            delay(1000);  
        }
    }
}

void setDutyCycleLedRed(uint8_t duty_cycle) {
    uint8_t analog_write_val = scaleDutyCycleParamForAnalogWriteFunction(duty_cycle);
    analogWrite(pwm_r_pin, analog_write_val);
}

void setDutyCycleLedGreen(uint8_t duty_cycle) {
    uint8_t analog_write_val = scaleDutyCycleParamForAnalogWriteFunction(duty_cycle);
    analogWrite(pwm_g_pin, analog_write_val);
}

void setDutyCycleLedBlue(uint8_t duty_cycle) {
    uint8_t analog_write_val = scaleDutyCycleParamForAnalogWriteFunction(duty_cycle);
    analogWrite(pwm_b_pin, analog_write_val);
}

uint8_t scaleDutyCycleParamForAnalogWriteFunction(uint8_t duty_cycle) {
    return round(255.0 / 100.0 * duty_cycle);
}

void runRainbowSequence(RgbLed led) {
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

    for (unsigned i = 0; i < 12; i++) {
        RgbLedDrv_setCustomColor(led, rainbow[i].r, rainbow[i].g, rainbow[i].b);
        delay(100);
    }
}
