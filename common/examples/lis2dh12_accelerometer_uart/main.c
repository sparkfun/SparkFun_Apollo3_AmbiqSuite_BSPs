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

#include "lis2dh12_platform_apollo3.h"

//
// Defines
#define LOOP_DELAY_MS 0

//
// Function Declarations
uint32_t initAccel( void );

//
// Global Variables
axis3bit16_t data_raw_acceleration;
axis1bit16_t data_raw_temperature;
float acceleration_mg[3];
float temperature_degC;

lis2dh12_platform_apollo3_if_t dev_if = {
    .iomHandle  = NULL,                             // Needs to be initialized later
    .addCS      = AM_BSP_ACCELEROMETER_I2C_ADDRESS, // Gets the accelerometer I2C address for the board
    .useSPI     = false,                            // Using I2C in this example
};

lis2dh12_ctx_t dev_ctx = {
    .write_reg  = lis2dh12_write_platform_apollo3,  // write bytes function
    .read_reg   = lis2dh12_read_platform_apollo3,   // read bytes function
    .handle     = (void*)&dev_if,                   // Apollo3-specific interface information
};

int main( void ){

    uint32_t stat = AM_HAL_STATUS_SUCCESS;

    // Board-agnostic setup
    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);
    am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
    am_hal_cachectrl_enable();  

    // Board-specific setup
    am_bsp_low_power_init();
    am_bsp_uart_printf_enable();

    // Clear UART terminal
    am_util_stdio_terminal_clear();

    // Initialize accelerometer (uses board-specific code -- see function below)
    stat = initAccel();
    if( stat != 0 ){
        am_util_stdio_printf("Accelerometer initialization failed with code: %d\n", stat);
    }
    am_util_stdio_printf("Accelerometer initialization successful");

    while(1){

        
        lis2dh12_reg_t reg;

        // Check if accelerometer is ready with new temperature data
        lis2dh12_temp_data_ready_get(&dev_ctx, &reg.byte);
        if (reg.byte)    {
            /* Read temperature data */
            lis2dh12_temperature_raw_get(&dev_ctx, data_raw_temperature.u8bit);

            /* Convert to celsius */
            temperature_degC = lis2dh12_from_lsb_hr_to_celsius(data_raw_temperature.i16bit);
        }

        // Check if accelerometer is ready with new acceleration data
        lis2dh12_xl_data_ready_get(&dev_ctx, &reg.byte);
        if (reg.byte){
            /* Read acceleration data */
            lis2dh12_acceleration_raw_get(&dev_ctx, data_raw_acceleration.u8bit);

            /* convert to mg */
            acceleration_mg[0] = lis2dh12_from_fs2_hr_to_mg(data_raw_acceleration.i16bit[0]);
            acceleration_mg[1] = lis2dh12_from_fs2_hr_to_mg(data_raw_acceleration.i16bit[1]);
            acceleration_mg[2] = lis2dh12_from_fs2_hr_to_mg(data_raw_acceleration.i16bit[2]);

            // Print results if acceleration data was ready
            am_util_stdio_printf("Acc [mg] %04.2f x, %04.2f y, %04.2f z, Temp [deg C] %04.2f,\r\n",
                        acceleration_mg[0], acceleration_mg[1], acceleration_mg[2], temperature_degC);
        }


        
#ifdef LOOP_DELAY_MS
#if LOOP_DELAY_MS
        am_util_delay_ms(LOOP_DELAY_MS);
#endif
#endif
    }

    // Disable debug
    am_bsp_debug_printf_disable();
    
    // Go to Deep Sleep.
    am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
}

uint32_t initAccel( void ){

    uint32_t retVal32 = 0;
    static uint8_t whoamI = 0;

    am_hal_iom_config_t i2cConfig = {
        .eInterfaceMode = AM_HAL_IOM_I2C_MODE,
        .ui32ClockFreq = AM_HAL_IOM_100KHZ,
    };

    // Initialize the IOM.
    retVal32 = am_hal_iom_initialize(AM_BSP_ACCELEROMETER_I2C_IOM, &(dev_if.iomHandle)); // set the iomHandle of the device interface
    if(retVal32 != AM_HAL_STATUS_SUCCESS){ return retVal32; }

    retVal32 = am_hal_iom_power_ctrl((dev_if.iomHandle), AM_HAL_SYSCTRL_WAKE, false);
    if(retVal32 != AM_HAL_STATUS_SUCCESS){ return retVal32; }
    
    retVal32 = am_hal_iom_configure((dev_if.iomHandle), &i2cConfig);
    if(retVal32 != AM_HAL_STATUS_SUCCESS){ return retVal32; }

    //
    // Configure the IOM pins.
    am_hal_gpio_pinconfig(AM_BSP_ACCELEROMETER_I2C_SDA_PIN,  g_AM_BSP_ACCELEROMETER_I2C_SDA_PIN);
    am_hal_gpio_pinconfig(AM_BSP_ACCELEROMETER_I2C_SCL_PIN,  g_AM_BSP_ACCELEROMETER_I2C_SDA_PIN);

    //
    // Enable the IOM.
    //
    retVal32 = am_hal_iom_enable((dev_if.iomHandle));
    if(retVal32 != AM_HAL_STATUS_SUCCESS){ return retVal32; }

    //
    // Apply accelerometer configuration
    lis2dh12_device_id_get(&dev_ctx, &whoamI);
    if (whoamI != LIS2DH12_ID){
        return AM_HAL_STATUS_FAIL; 
    }

    am_util_stdio_printf("Whoami (should be 0x33): 0x%2x\n", whoamI);

    lis2dh12_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
    lis2dh12_temperature_meas_set(&dev_ctx, LIS2DH12_TEMP_ENABLE);
    lis2dh12_data_rate_set(&dev_ctx, LIS2DH12_ODR_25Hz);
    lis2dh12_full_scale_set(&dev_ctx, LIS2DH12_2g);
    lis2dh12_temperature_meas_set(&dev_ctx, LIS2DH12_TEMP_ENABLE);
    lis2dh12_operating_mode_set(&dev_ctx, LIS2DH12_HR_12bit);

    return 0;
}