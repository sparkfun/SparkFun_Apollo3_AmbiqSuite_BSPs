



#include <stdio.h>
#include <string.h>
#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
#include "HM01B0.h"
#include "HM01B0_RAW8_QVGA_8bits_lsb_5fps.h"


#define DEMO_HM01B0_TEST_MODE_ENABLE
#define DEMO_HM01B0_FRAMEBUFFER_DUMP_ENABLE

//*****************************************************************************
//
// HM01B0 Configuration
//
//*****************************************************************************
static hm01b0_cfg_t         s_HM01B0Cfg =
{
    // i2c settings
    .ui16SlvAddr                = HM01B0_DEFAULT_ADDRESS,
    .eIOMMode                   = AM_HAL_IOM_I2C_MODE,
    .ui32IOMModule              = AM_BSP_CAMERA_HM01B0_I2C_IOM,
    .sIOMCfg                    =   {
                                        .eInterfaceMode     = AM_HAL_IOM_I2C_MODE,
                                        .ui32ClockFreq      = AM_HAL_IOM_100KHZ,
                                    },
    .pIOMHandle                 = NULL,
    .ui8PinSCL                  = AM_BSP_CAMERA_HM01B0_I2C_SCL_PIN,
    .ui8PinSDA                  = AM_BSP_CAMERA_HM01B0_I2C_SDA_PIN,

    // MCLK settings
    .ui32CTimerModule           = 0 /*HM01B0_MCLK_GENERATOR_MOD*/, // todo: I think these are unused
    .ui32CTimerSegment          = 0 /*HM01B0_MCLK_GENERATOR_SEG*/, // todo: I think these are unused
    .ui32CTimerOutputPin        = AM_BSP_CAMERA_HM01B0_MCLK_PIN,

    // data interface
    .ui8PinD0                   = AM_BSP_GPIO_CAMERA_HM01B0_D0,
    .ui8PinD1                   = AM_BSP_GPIO_CAMERA_HM01B0_D1,
    .ui8PinD2                   = AM_BSP_GPIO_CAMERA_HM01B0_D2,
    .ui8PinD3                   = AM_BSP_GPIO_CAMERA_HM01B0_D3,
    .ui8PinD4                   = AM_BSP_GPIO_CAMERA_HM01B0_D4,
    .ui8PinD5                   = AM_BSP_GPIO_CAMERA_HM01B0_D5,
    .ui8PinD6                   = AM_BSP_GPIO_CAMERA_HM01B0_D6,
    .ui8PinD7                   = AM_BSP_GPIO_CAMERA_HM01B0_D7,
    .ui8PinVSYNC                = AM_BSP_GPIO_CAMERA_HM01B0_VSYNC,
    .ui8PinHSYNC                = AM_BSP_GPIO_CAMERA_HM01B0_HSYNC,
    .ui8PinPCLK                 = AM_BSP_GPIO_CAMERA_HM01B0_PCLK,
    .ui8PinTrig                 = AM_BSP_GPIO_CAMERA_HM01B0_TRIG,
    .ui8PinInt                  = AM_BSP_GPIO_CAMERA_HM01B0_INT,
    .pfnGpioIsr                 = NULL,
};

static uint8_t s_ui8FrameBuffer[HM01B0_PIXEL_X_NUM * HM01B0_PIXEL_Y_NUM];

//*****************************************************************************
//
// Forward declarations
//
//*****************************************************************************
void boost_mode_enable(bool bEnable);
void am_gpio_isr(void);
void framebuffer_dump(uint8_t *pui8Buffer, uint32_t ui32BufferLen);






//*****************************************************************************
//
// Main function.
//
//*****************************************************************************
int main(void)
{

    uint32_t    ui32Err     = HM01B0_ERR_OK;
    uint16_t    ui16ModelId = 0x0000;
    uint8_t     ui8Mode     = 0xFF;

    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);

    //
    // Set the default cache configuration
    //
    am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
    am_hal_cachectrl_enable();

    //
    // Configure the board for low power operation.
    //
    am_bsp_low_power_init();

    //
    // Turn on the camera regulator
    //
    am_hal_gpio_pinconfig(AM_BSP_GPIO_CAMERA_HM01B0_DVDDEN, g_AM_HAL_GPIO_OUTPUT_12);
    am_hal_gpio_output_set(AM_BSP_GPIO_CAMERA_HM01B0_DVDDEN);

    //
    // Enable the UART print interface.
    //
    am_bsp_uart_printf_enable();

    //
    // Clear the terminal and print the banner.
    //
    am_util_stdio_terminal_clear();
    am_util_stdio_printf("HM01B0 Demo\n");
    am_util_stdio_printf(" SCL:\tPin %d\n",  s_HM01B0Cfg.ui8PinSCL);
    am_util_stdio_printf(" SDA:\tPin %d\n",  s_HM01B0Cfg.ui8PinSDA);
    am_util_stdio_printf(" MCLK:\tPin %d\n",  s_HM01B0Cfg.ui32CTimerOutputPin);
    am_util_stdio_printf(" VSYNC:\tPin %d\n", s_HM01B0Cfg.ui8PinVSYNC);
    am_util_stdio_printf(" HSYNC\tPin %d\n",  s_HM01B0Cfg.ui8PinHSYNC);
    am_util_stdio_printf(" PCLK:\tPin %d\n",  s_HM01B0Cfg.ui8PinPCLK);
    am_util_stdio_printf(" D0:\tPin %d\n",    s_HM01B0Cfg.ui8PinD0);
    am_util_stdio_printf(" D1:\tPin %d\n",    s_HM01B0Cfg.ui8PinD1);
    am_util_stdio_printf(" D2:\tPin %d\n",    s_HM01B0Cfg.ui8PinD2);
    am_util_stdio_printf(" D3:\tPin %d\n",    s_HM01B0Cfg.ui8PinD3);
    am_util_stdio_printf(" D4:\tPin %d\n",    s_HM01B0Cfg.ui8PinD4);
    am_util_stdio_printf(" D5:\tPin %d\n",    s_HM01B0Cfg.ui8PinD5);
    am_util_stdio_printf(" D6:\tPin %d\n",    s_HM01B0Cfg.ui8PinD6);
    am_util_stdio_printf(" D7:\tPin %d\n",    s_HM01B0Cfg.ui8PinD7);

    //
    // Enable interrupts so we can receive messages from the boot host.
    //
    am_hal_interrupt_master_enable();

    boost_mode_enable(true);

    hm01b0_power_up(&s_HM01B0Cfg);

    // todo: check the delay time to just fit the spec.
    am_util_delay_ms(1);

    hm01b0_mclk_enable(&s_HM01B0Cfg);

    // todo: check the delay time to just fit the spec.
    am_util_delay_ms(1);

    hm01b0_init_if(&s_HM01B0Cfg);

    hm01b0_get_modelid(&s_HM01B0Cfg, &ui16ModelId);

    am_util_stdio_printf("HM01B0 Model ID 0x%04X\n", ui16ModelId);
      
    hm01b0_init_system(&s_HM01B0Cfg, (hm_script_t *)sHM01B0InitScript, sizeof(sHM01B0InitScript)/sizeof(hm_script_t));

#ifdef DEMO_HM01B0_TEST_MODE_ENABLE
    am_util_stdio_printf("HM01B0 Enable walking 1s test mode\n");
    hm01b0_test_walking1s(&s_HM01B0Cfg);
#else
    hm01b0_cal_ae(&s_HM01B0Cfg, 10, s_ui8FrameBuffer, sizeof(s_ui8FrameBuffer));
#endif

    while(1)
    {
        hm01b0_ae_cfg_t sAECfg;

        hm01b0_get_mode(&s_HM01B0Cfg, &ui8Mode);

        am_util_stdio_printf("HM01B0 current mode %d\n", ui8Mode);

        ui32Err = hm01b0_get_ae(&s_HM01B0Cfg, &sAECfg);
        am_util_stdio_printf("AE convergance(0x%02X) TargetMean 0x%02X, ConvergeInTh 0x%02X, AEMean 0x%02X\n", ui32Err, sAECfg.ui8AETargetMean, sAECfg.ui8ConvergeInTh, sAECfg.ui8AEMean);

        hm01b0_cmd_update(&s_HM01B0Cfg);

        hm01b0_set_mode(&s_HM01B0Cfg, HM01B0_REG_MODE_SELECT_STREAMING_NFRAMES, 1);

        hm01b0_blocking_read_oneframe(&s_HM01B0Cfg, s_ui8FrameBuffer, sizeof(s_ui8FrameBuffer));

#ifdef DEMO_HM01B0_TEST_MODE_ENABLE
        hm01b0_test_walking1s_check_data_sanity(s_ui8FrameBuffer, sizeof(s_ui8FrameBuffer), 10);
        am_util_delay_ms(3000);
#endif

#ifdef DEMO_HM01B0_FRAMEBUFFER_DUMP_ENABLE
        framebuffer_dump(s_ui8FrameBuffer, sizeof(s_ui8FrameBuffer));
#endif
        memset(s_ui8FrameBuffer, 0x00, sizeof(s_ui8FrameBuffer));

        // give some time for user to stop the external itm logging.
        am_util_delay_ms(5000);

    }

    hm01b0_deinit_if(&s_HM01B0Cfg);

    hm01b0_mclk_disable(&s_HM01B0Cfg);

    hm01b0_power_down(&s_HM01B0Cfg);

    boost_mode_enable(false);

    //
    // Loop forever.
    //
    while (1)
    {
        //
        // Go to Deep Sleep.
        //
        am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
    }
}





//*****************************************************************************
//
// Helper Functions
//
//*****************************************************************************

// GPIO ISR
void
am_gpio_isr(void)
{
    if (s_HM01B0Cfg.pfnGpioIsr)
        s_HM01B0Cfg.pfnGpioIsr();
}

// frame buffer dump
void framebuffer_dump(uint8_t *pui8Buffer, uint32_t ui32BufferLen)
{
    am_util_stdio_printf("+++ frame +++");

    for (uint32_t ui32Idx = 0; ui32Idx < ui32BufferLen; ui32Idx++)
    {
        if ((ui32Idx & 0xF) == 0x00)
        {
            am_util_stdio_printf("\n0x%08X ", ui32Idx);
            // this delay is to let itm have time to flush out data.
            am_util_delay_ms(1);
        }

        am_util_stdio_printf("%02X ", *(pui8Buffer + ui32Idx));
    }

    am_util_stdio_printf("\n--- frame ---\n");
    am_util_delay_ms(1);
}

// burst mode enable
void boost_mode_enable(bool bEnable){
    am_hal_burst_avail_e          eBurstModeAvailable;
    am_hal_burst_mode_e           eBurstMode;

    // Check that the Burst Feature is available.
    if (AM_HAL_STATUS_SUCCESS == am_hal_burst_mode_initialize(&eBurstModeAvailable)){
        if (AM_HAL_BURST_AVAIL == eBurstModeAvailable){
            am_util_stdio_printf("Apollo3 Burst Mode is Available\n");
        }
        else{
            am_util_stdio_printf("Apollo3 Burst Mode is Not Available\n");
            while(1){};
        }
    }
    else{
        am_util_stdio_printf("Failed to Initialize for Burst Mode operation\n");
    }

    // Make sure we are in "Normal" mode.
    if (AM_HAL_STATUS_SUCCESS == am_hal_burst_mode_disable(&eBurstMode)){
        if (AM_HAL_NORMAL_MODE == eBurstMode){
            am_util_stdio_printf("Apollo3 operating in Normal Mode (48MHz)\n");
        }
    }
    else{
        am_util_stdio_printf("Failed to Disable Burst Mode operation\n");
    }

    // Put the MCU into "Burst" mode.
    if (bEnable)
    {
        if (AM_HAL_STATUS_SUCCESS == am_hal_burst_mode_enable(&eBurstMode)){
            if (AM_HAL_BURST_MODE == eBurstMode){
                am_util_stdio_printf("Apollo3 operating in Burst Mode (96MHz)\n");
            }
        }
        else{
            am_util_stdio_printf("Failed to Enable Burst Mode operation\n");
        }
    }
}