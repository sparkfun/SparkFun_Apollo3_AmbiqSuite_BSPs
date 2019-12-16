/*
Tests / verifies startup

Checks:
- data segment copy
- bss segment zero-fill
- C++ global static constructor initialization
*/

#include "main.h"
#include "test_framework.h"
#include "tests.h"

// main
int main()
{
  // Setup system clocks
  am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);

  // Set the default cache configuration
  am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
  am_hal_cachectrl_enable();

  // Configure the board for low power operation.
  am_bsp_low_power_init();

  // Enable the UART print interface.
  am_bsp_uart_printf_enable();

  // Clear the terminal and print the banner.
  am_util_stdio_terminal_clear();
  am_util_stdio_printf("Startup Tests\n");
  am_util_stdio_printf("=============\n");
  am_util_stdio_printf("\n");

  // run tests
  run_tests(tests);

  // Loop forever while sleeping.
  while (1){
    // Go to Deep Sleep.
    am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
  }
}