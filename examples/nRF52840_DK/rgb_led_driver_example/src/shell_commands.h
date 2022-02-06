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

#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H

/* include directives --------------------------------------------------------------------------------------*/

#include <zephyr.h>
#include <sys/printk.h>
#include <shell/shell.h>

/* exported defines for constants --------------------------------------------------------------------------*/

/* exported typedef declarations ---------------------------------------------------------------------------*/

/* exported object declarations ----------------------------------------------------------------------------*/

/* exported function prototypes ----------------------------------------------------------------------------*/

int ShellCmd_runExampleRgbLedSequence(const struct shell *shell, size_t argc, char **argv);
int ShellCmd_pauseExampleRgbLedSequence(const struct shell *shell, size_t argc, char **argv);
int ShellCmd_setCustomColor(const struct shell *shell, size_t argc, char **argv);
int ShellCmd_setPredefinedColor(const struct shell *shell, size_t argc, char **argv);
int ShellCmd_turnOn(const struct shell *shell, size_t argc, char **argv);
int ShellCmd_turnOff(const struct shell *shell, size_t argc, char **argv);

SHELL_STATIC_SUBCMD_SET_CREATE(sub_rgb_led,
        SHELL_CMD(run_example_sequence, NULL, "Run example RGB LED sequence.", ShellCmd_runExampleRgbLedSequence),
        SHELL_CMD(pause_example_sequence, NULL, "Pause example RGB LED sequence.", ShellCmd_pauseExampleRgbLedSequence),
        SHELL_CMD(set_custom_color, NULL, "Set custom color by passing RGB components values.", ShellCmd_setCustomColor),
        SHELL_CMD(set_predefined_color, NULL, "Set one of predefined colors: RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE.", ShellCmd_setPredefinedColor),
        SHELL_CMD(on, NULL, "Turn the RGB LED on.", ShellCmd_turnOn),
        SHELL_CMD(off, NULL, "Turn the RGB LED off.", ShellCmd_turnOff),
        SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(rgb_led, &sub_rgb_led, "RGB LED commands", NULL);

/* exported macros and inline functions --------------------------------------------------------------------*/

#endif
