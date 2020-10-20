//*****************************************************************************
//
//  am_bsp.h
//! @file
//!
//! @brief Functions to aid with configuring the GPIOs.
//!
//! @addtogroup BSP Board Support Package (BSP)
//! @ingroup BSP
//! @{
//
//*****************************************************************************

#ifndef AM_BSP_H
#define AM_BSP_H

#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_apollo.h"
#include "am_bsp_pins.h"

//
// Make individual includes to not require full port before usage.
//#include "am_devices.h"
//
#include "am_devices_led.h"
#include "am_devices_button.h"

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Begin User Modifiable Area
//
//*****************************************************************************

//*****************************************************************************
//
// Primary I2C.
//
//*****************************************************************************
#define AM_BSP_PRIM_I2C_IOM                 4
#define AM_BSP_PRIM_I2C_SDA_PIN             AM_BSP_GPIO_SDA
#define AM_BSP_PRIM_I2C_SCL_PIN             AM_BSP_GPIO_SCL
#define g_AM_BSP_PRIM_I2C_SDA               g_AM_BSP_GPIO_SDA
#define g_AM_BSP_PRIM_I2C_SCL               g_AM_BSP_GPIO_SCL

//*****************************************************************************
//
// Primary SPI Pins
//
//*****************************************************************************
#define AM_BSP_PRIM_SPI_IOM                 4
#define AM_BSP_PRIM_SPI_CLK_PIN             AM_BSP_GPIO_SPI_SCK
#define AM_BSP_PRIM_SPI_SDO_PIN             AM_BSP_GPIO_SPI_SDO
#define AM_BSP_PRIM_SPI_SDI_PIN             AM_BSP_GPIO_SPI_SDI
#define g_AM_BSP_PRIM_SPI_CLK               g_AM_BSP_GPIO_SPI_SCK
#define g_AM_BSP_PRIM_SPI_SDO               g_AM_BSP_GPIO_SPI_SDO
#define g_AM_BSP_PRIM_SPI_SDI               g_AM_BSP_GPIO_SPI_SDI

//*****************************************************************************
//
// Primary UART Pins
//
//*****************************************************************************
#define AM_BSP_PRIM_UART_TX_PIN             AM_BSP_GPIO_COM_UART_TX
#define AM_BSP_PRIM_UART_RX_PIN             AM_BSP_GPIO_COM_UART_RX
#define g_AM_BSP_PRIM_UART_TX               g_AM_BSP_GPIO_COM_UART_TX
#define g_AM_BSP_PRIM_UART_RX               g_AM_BSP_GPIO_COM_UART_RX

//*****************************************************************************
//
// LED definitions.
//
//*****************************************************************************
#define AM_BSP_NUM_LEDS                   1
extern am_devices_led_t am_bsp_psLEDs[AM_BSP_NUM_LEDS];

// LED Device Array Indices
#define AM_BSP_LED0 0
#define AM_BSP_LED_BLUE      AM_BSP_LED0

// Corresponding GPIO Numbers
#define AM_BSP_GPIO_LED0         AM_BSP_GPIO_LED_BLUE
#define AM_BSP_GPIO_LED23        AM_BSP_GPIO_LED_BLUE
#define AM_BSP_GPIO_LED_STAT     AM_BSP_GPIO_LED_BLUE

//*****************************************************************************
//
// PWM_LED peripheral assignments.
//
//*****************************************************************************
//
// The Artemis MM PB LED0 is pad 19
//
#define AM_BSP_PIN_PWM_LED                  AM_BSP_GPIO_LED0
#define AM_BSP_PWM_LED_TIMER                1
#define AM_BSP_PWM_LED_TIMER_SEG            AM_HAL_CTIMER_TIMERB
#define AM_BSP_PWM_LED_TIMER_INT            AM_HAL_CTIMER_INT_TIMERB1C0

//*****************************************************************************
//
// UART definitions.
//
//*****************************************************************************
//
// Apollo3 has two UART instances.
// AM_BSP_UART_PRINT_INST should correspond to COM_UART.
//
#define AM_BSP_UART_IOS_INST                0
#define AM_BSP_UART_PRINT_INST              0
#define AM_BSP_UART_BOOTLOADER_INST         0

//*****************************************************************************
//
// Machine Learning Carrier Board Defs
//
//*****************************************************************************
#define AM_BSP_PDM_CHANNEL                  AM_HAL_PDM_CHANNEL_RIGHT
#define AM_BSP_PDM_DATA_PIN                 AM_BSP_GPIO_MIC_DATA
#define AM_BSP_PDM_CLOCK_PIN                AM_BSP_GPIO_MIC_CLK
#define g_AM_BSP_PDM_DATA                   g_AM_BSP_GPIO_MIC_DATA
#define g_AM_BSP_PDM_CLOCK                  g_AM_BSP_GPIO_MIC_CLK

#define AM_BSP_ACCELEROMETER_I2C_IOM        AM_BSP_PRIM_I2C_IOM
#define AM_BSP_ACCELEROMETER_I2C_ADDRESS    0x19
#define AM_BSP_ACCELEROMETER_I2C_SDA_PIN    AM_BSP_PRIM_I2C_SDA_PIN
#define AM_BSP_ACCELEROMETER_I2C_SCL_PIN    AM_BSP_PRIM_I2C_SCL_PIN
#define g_AM_BSP_ACCELEROMETER_I2C_SDA      g_AM_BSP_PRIM_I2C_SDA
#define g_AM_BSP_ACCELEROMETER_I2C_SCL      g_AM_BSP_PRIM_I2C_SCL

#define AM_BSP_GPIO_CAMERA_HM01B0_DVDDEN    AM_BSP_GPIO_PWM1
#define AM_BSP_CAMERA_HM01B0_I2C_IOM        AM_BSP_PRIM_I2C_IOM
#define AM_BSP_CAMERA_HM01B0_I2C_SCL_PIN    AM_BSP_PRIM_I2C_SCL_PIN
#define AM_BSP_CAMERA_HM01B0_I2C_SDA_PIN    AM_BSP_PRIM_I2C_SDA_PIN
#define AM_BSP_GPIO_CAMERA_HM01B0_D0        AM_BSP_GPIO_G0
#define AM_BSP_GPIO_CAMERA_HM01B0_D1        AM_BSP_GPIO_G1
#define AM_BSP_GPIO_CAMERA_HM01B0_D2        AM_BSP_GPIO_G2
#define AM_BSP_GPIO_CAMERA_HM01B0_D3        AM_BSP_GPIO_G3
#define AM_BSP_GPIO_CAMERA_HM01B0_D4        AM_BSP_GPIO_G4
#define AM_BSP_GPIO_CAMERA_HM01B0_D5        AM_BSP_GPIO_G5
#define AM_BSP_GPIO_CAMERA_HM01B0_D6        AM_BSP_GPIO_G6
#define AM_BSP_GPIO_CAMERA_HM01B0_D7        AM_BSP_GPIO_G7
#define AM_BSP_CAMERA_HM01B0_MCLK_GEN_MOD   1
#define AM_BSP_CAMERA_HM01B0_MCLK_GEN_SEG   AM_HAL_CTIMER_TIMERA
#define AM_BSP_CAMERA_HM01B0_MCLK_PIN  AM_BSP_GPIO_AUD_OUT
#define AM_BSP_GPIO_CAMERA_HM01B0_VSYNC     AM_BSP_GPIO_G10
#define AM_BSP_GPIO_CAMERA_HM01B0_HSYNC     AM_BSP_GPIO_G9
#define AM_BSP_GPIO_CAMERA_HM01B0_PCLK      AM_BSP_GPIO_AUD_IN
#define AM_BSP_GPIO_CAMERA_HM01B0_TRIG      AM_BSP_GPIO_D1
#define AM_BSP_GPIO_CAMERA_HM01B0_INT       AM_BSP_GPIO_I2C_INT

//*****************************************************************************
//
// End User Modifiable Area
//
//*****************************************************************************

//*****************************************************************************
//
// Print interface type
//
//*****************************************************************************
#define AM_BSP_PRINT_INFC_NONE              0
#define AM_BSP_PRINT_INFC_SWO               1
#define AM_BSP_PRINT_INFC_UART0             2
#define AM_BSP_PRINT_INFC_BUFFERED_UART0    3


//*****************************************************************************
//
//! Structure containing UART configuration information while it is powered down.
//
//*****************************************************************************
typedef struct
{
    bool     bSaved;
    uint32_t ui32TxPinNum;
    uint32_t ui32TxPinCfg;
}
am_bsp_uart_pwrsave_t;

//*****************************************************************************
//
// External data definitions.
//
//*****************************************************************************
extern am_bsp_uart_pwrsave_t am_bsp_uart_pwrsave[AM_REG_UART_NUM_MODULES];

//*****************************************************************************
//
// External function definitions.
//
//*****************************************************************************
extern void am_bsp_low_power_init(void);
extern void am_bsp_iom_pins_enable(uint32_t ui32Module, am_hal_iom_mode_e eIOMMode);
extern void am_bsp_iom_pins_disable(uint32_t ui32Module, am_hal_iom_mode_e eIOMMode);
extern void am_bsp_mspi_pins_enable(am_hal_mspi_device_e eMSPIDevice);
extern void am_bsp_mspi_pins_disable(am_hal_mspi_device_e eMSPIDevice);

extern void am_bsp_ios_pins_enable(uint32_t ui32Module, uint32_t ui32IOSMode);   // SparkFun Edge does not expose IO Slave Clock signal, so hiding these functions
extern void am_bsp_ios_pins_disable(uint32_t ui32Module, uint32_t ui32IOSMode);

extern void am_bsp_debug_printf_enable(void);
extern void am_bsp_debug_printf_disable(void);

#ifdef AM_BSP_GPIO_ITM_SWO
extern void am_bsp_itm_printf_enable(void);
#else
extern void am_bsp_itm_printf_enable(uint32_t ui32Pin, am_hal_gpio_pincfg_t sPincfg);
#endif
extern void am_bsp_itm_string_print(char *pcString);
extern void am_bsp_itm_printf_disable(void);

extern void am_bsp_uart_string_print(char *pcString);
extern void am_bsp_uart_printf_enable(void);
extern void am_bsp_uart_printf_enable_custom(const am_hal_uart_config_t* p_config);
extern void am_bsp_uart_printf_disable(void);

extern void am_bsp_buffered_uart_printf_enable(void);
extern void am_bsp_buffered_uart_service(void);

extern uint32_t am_bsp_com_uart_transfer(const am_hal_uart_transfer_t *psTransfer);

#ifdef __cplusplus
}
#endif

#endif // AM_BSP_H
