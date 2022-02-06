# RGB-LED-DRIVER
A platform-independent driver for controlling RGB LEDs:
1. With the set of functions implemented in the driver you no longer need to write this tedious code over and over again.
2. All you need to do is to copy [the  driver files](rgb_led_driver) to your project, and to provide pointers to the functions responsible for setting PWM duty cycle.
3. With this driver you can control only one, or as many LEDs as you need.
4. Important: dynamic memory allocation is used to store each LED data. `malloc` and `free` must be available on your platform. There is room for improvement here. Instead of dynamic allocation, a fixed-size array can be used for systems where `malloc` is not available.
5. Refer to [examples](examples) for details of usage.
