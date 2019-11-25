// A starting point for projects with the Ambiqsuite SDK

#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"


//*****************************************************************************
//
// Main
//
//*****************************************************************************
int
main(void)
{
  // Startup boilerplate
  am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0); // Set the clock frequency.
  am_hal_cachectrl_config(&am_hal_cachectrl_defaults);        // Set the default cache configuration
  am_hal_cachectrl_enable();
  am_bsp_low_power_init();                                    // Configure the board for low power operation.


  // Loop forever while sleeping.
  while (1)
  {
    // Go to Deep Sleep.
    am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
  }
}
