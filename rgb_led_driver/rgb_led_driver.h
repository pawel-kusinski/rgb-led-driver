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

/**
 * @file
 * @brief RGB LED Driver APIs
 */

/**
 * @brief RGB LED Driver
 * @defgroup rgb_led_driver RGB LED Driver
 * @{
 */

#ifndef RGB_LED_DRIVER_H_
#define RGB_LED_DRIVER_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RGB_LED_DRV_INVALID_OBJECT NULL

/**
 * @brief RGB LED object. Stores state of an LED.
 */
typedef struct RgbLedDrvHandle* RgbLed;

/**
 * @brief Pointer to function for setting PWM duty cycle.
 * 
 * @details The function parameter specifies the duty cycle. Values from 0 to 100 must be accepted.
 *          The parameter value specifies the percentage of time the PWM pin is in high state.
 */
typedef void (*SetPwmDutyCycleFunction)(uint8_t);

/**
 * @brief Pre-defined RGB LED colors.
 */
typedef enum _RgbLedColor {
    RGB_LED_COLOR_RED = 0,
    RGB_LED_COLOR_GREEN,
    RGB_LED_COLOR_BLUE,
    RGB_LED_COLOR_YELLOW,
    RGB_LED_COLOR_CYAN,
    RGB_LED_COLOR_MAGENTA,
    RGB_LED_COLOR_WHITE,
    RGB_LED_COLOR_CUSTOM
} RgbLedColor;

/**
 * @brief RGB LED configuration; Specifies if it is in common anode or common cathode configuration.
 * 
 * @details Thanks to this information passed to the driver, the user does not need to invert
 *           PWM duty cycle when different RGM LED confifgurations are used in the same system.
 */
typedef enum _RgbLedCfg {
    RGB_LED_CFG_COMM_ANODE = 0,
    RGB_LED_CFG_COMM_CATHODE
} RgbLedCfg;

/**
 * @brief Create a new RgbLed object.
 * 
 * @details The object is created by dynamic memory allocation.
 *          The platform running this code must support @a malloc() and @a free(). 
 *          @p set_pwm_r, @p set_pwm_g, and @p set_pwm_b are mandatory and must not be NULL.
 *          Passing invalid values of @p cfg or @p color will result in failure.
 *
 * @param set_pwm_r Pointer to the function for setting PWM duty cycle of
 *                  the pin connected to RGB LED red color terminal.
 * @param set_pwm_g Pointer to the function for setting PWM duty cycle of
 *                  the pin connected to RGB LED green color terminal.
 * @param set_pwm_b Pointer to the function for setting PWM duty cycle of
 *                  the pin connected to RGB LED blue color terminal.
 * @param cfg RGB LED configuration (common anode or common cathode).
 * @param color Initial pre-defined color to set. If set to RGB_LED_COLOR_CUSTOM, 
 *              then the initial color is set basing on @p r, @p g, and @p b parameters.
 * @param r R component of initial color to set (ranges from 0 to 255). Ignored if @p color is not set to RGB_LED_COLOR_CUSTOM.
 * @param g G component of initial color to set (ranges from 0 to 255). Ignored if @p color is not set to RGB_LED_COLOR_CUSTOM.
 * @param b B component of initial color to set (ranges from 0 to 255). Ignored if @p color is not set to RGB_LED_COLOR_CUSTOM.
 * @param initial_state Initial state of the RGB LED. If set to true, then LED will be turned on and illuminate
 *                      with initial color. If set to false, the LED remains off, until @a RgbLedDrv_turnOn() is called.
 *
 * @return valid RgbLed object if successful.
 * @retval RGB_LED_DRV_INVALID_OBJECT if failure.
 */
RgbLed RgbLedDrv_create(SetPwmDutyCycleFunction set_pwm_r, SetPwmDutyCycleFunction set_pwm_g, SetPwmDutyCycleFunction set_pwm_b,
                        RgbLedCfg cfg, RgbLedColor color, uint8_t r, uint8_t g, uint8_t b, bool initial_state);

/**
 * @brief Destroy the RgbLed object.
 * 
 * @param led Valid RgbLed object. After calling this function @p led is set to RGB_LED_DRV_INVALID_OBJECT.
 *            This function has no effect if @p led is RGB_LED_DRV_INVALID_OBJECT.
 */
void RgbLedDrv_destroy(RgbLed led);

/**
 * @brief Turn the RGB LED on.
 * 
 * @details The LED will illuminate with recently set color
 *          (set by @a RgbLedDrv_create(), @a RgbLedDrv_setPredefinedColor(), or @a RgbLedDrv_setCustomColor()).
 *          This function has no effect when the LED is already turned on.
 * 
 * @param led Valid RgbLed object. This function has no effect if @p led is RGB_LED_DRV_INVALID_OBJECT.
 */
void RgbLedDrv_turnOn(RgbLed led);

/**
 * @brief Turn the RGB LED off.
 *        
 * @details This function has no effect when the LED is already turned off.
 * 
 * @param led Valid RgbLed object. This function has no effect if @p led is RGB_LED_DRV_INVALID_OBJECT.
 */
void RgbLedDrv_turnOff(RgbLed led);

/**
 * @brief Set pre-defined color of the RGB LED.
 * 
 * @details If the LED is turned off (becauyse either @a RgbLedDrv_turnOff() has been called or 
 *          @a initial_state was set to false when creating the object), this function will not tunr the LED on.
 *          It only updates the color which will be illuminated as soon as the LED is turned on (using @a RgbLedDrv_turnOn()).
 * 
 * @param led Valid RgbLed object. This function has no effect if @p led is RGB_LED_DRV_INVALID_OBJECT.
 * @param color Pre-defined color to set. This function has no effect if @p color is an invalid value or RGB_LED_COLOR_CUSTOM.
 */
void RgbLedDrv_setPredefinedColor(RgbLed led, RgbLedColor color);

/**
 * @brief Set custom color of the RGB LED.
 * 
 * @details If the LED is turned off (becauyse either @a RgbLedDrv_turnOff() has been called or 
 *          @a initial_state was set to false when creating the object), this function will not tunr the LED on.
 *          It only updates the color which will be illuminated as soon as the LED is turned on (using @a RgbLedDrv_turnOn()).
 * 
 * @param led Valid RgbLed object. This function has no effect if @p led is RGB_LED_DRV_INVALID_OBJECT.
 * @param r R component of initial color to set (ranges from 0 to 255).
 * @param g G component of initial color to set (ranges from 0 to 255).
 * @param b B component of initial color to set (ranges from 0 to 255).
 */
void RgbLedDrv_setCustomColor(RgbLed led, uint8_t r, uint8_t g, uint8_t b);

#ifdef __cplusplus
}
#endif

#endif /* RGB_LED_DRIVER_H */

/**
 * @}
 */
