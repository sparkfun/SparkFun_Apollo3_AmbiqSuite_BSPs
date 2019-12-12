//*****************************************************************************
//
//! @file main.c
//!
//! @brief A simple LED blinking example.
//!
//
//*****************************************************************************

/*
Copyright (c) 2019 SparkFun Electronics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"

#define BLINK_PERIOD 500

//*****************************************************************************
//
// Main
//
//*****************************************************************************
int
main(void)
{
    // Set the clock frequency.
    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);

    // Set the default cache configuration
    am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
    am_hal_cachectrl_enable();

    // Configure the board for low power operation.
    am_bsp_low_power_init();

    // Set up BSP leds
#ifdef AM_BSP_NUM_LEDS
    uint32_t ux, ui32GPIONumber;
    for (ux = 0; ux < AM_BSP_NUM_LEDS; ux++) {
        ui32GPIONumber = am_bsp_psLEDs[ux].ui32GPIONumber;
        am_hal_gpio_pinconfig(ui32GPIONumber, g_AM_HAL_GPIO_OUTPUT);
        am_devices_led_off(am_bsp_psLEDs, ux);
    }
#endif // AM_BSP_NUM_LEDS

    bool led_state = false;

    // Blink forever
    while (1)
    {
        // Toggle LEDs
#ifdef AM_BSP_NUM_LEDS
        led_state = !led_state;
        uint32_t ux;
        for (ux = 0; ux < AM_BSP_NUM_LEDS; ux++) {
            ui32GPIONumber = am_bsp_psLEDs[ux].ui32GPIONumber;
            (led_state) ? am_devices_led_on(am_bsp_psLEDs, ux) : am_devices_led_off(am_bsp_psLEDs, ux);
        }
#endif // AM_BSP_NUM_LEDS

        // Delay
        am_util_delay_ms(BLINK_PERIOD);
    }
}
