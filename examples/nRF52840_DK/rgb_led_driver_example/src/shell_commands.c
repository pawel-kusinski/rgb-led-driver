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

#include "shell_commands.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "rgb_led_driver.h"

extern bool run_example_sequence;
extern RgbLed my_led;

int ShellCmd_runExampleRgbLedSequence(const struct shell *shell, size_t argc, char **argv) {
    run_example_sequence = true;
    return 0;
}

int ShellCmd_pauseExampleRgbLedSequence(const struct shell *shell, size_t argc, char **argv) {
    run_example_sequence = false;
    return 0;
}

int ShellCmd_setCustomColor(const struct shell *shell, size_t argc, char **argv) {
    if (argc != 4) {
        shell_print(shell, "Invalid number of arguments.");
        return 0;
    }

    uint8_t red = atoi(argv[1]);
    uint8_t green = atoi(argv[2]);
    uint8_t blue = atoi(argv[3]);
    shell_print(shell, "Setting RGB: %u %u %u.", red, green, blue);
    RgbLedDrv_setCustomColor(my_led, red, green, blue);
    return 0;
}

int ShellCmd_setPredefinedColor(const struct shell *shell, size_t argc, char **argv) {
    const char* colors[7] = {"red", "green", "blue", "yellow", "cyan", "magenta", "white"};

    if (argc != 2) {
        goto invalid_command;
    }

    int color_idx;

    for (color_idx = 0; color_idx < 7; ++color_idx) {
        if (0 == strcmp(colors[color_idx], argv[1])) {
            /* Second parameter should be RgbLedColor enum type, but using index is good enough for this demo code. */
            RgbLedDrv_setPredefinedColor(my_led, color_idx);
            return 0;
        }
    }

invalid_command:
    shell_print(shell, "Invalid usage. This command requires one parameter and it should one of the following options: "
                "%s, %s, %s, %s, %s, %s, %s.", colors[0], colors[1], colors[2], colors[3], colors[4], colors[5], colors[6]);

    return 0;
}

int ShellCmd_turnOn(const struct shell *shell, size_t argc, char **argv) {
    RgbLedDrv_turnOn(my_led);
    return 0;
}

int ShellCmd_turnOff(const struct shell *shell, size_t argc, char **argv) {
    RgbLedDrv_turnOff(my_led);
    return 0;
}
