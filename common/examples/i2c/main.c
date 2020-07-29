#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"

// #define ASYNCH // comment out to use blocking methods

#define DEVICE_ADDR (0xEE)
#define DEVICE_ADDR_R (DEVICE_ADDR | 0x01)
#define DEVICE_ADDR_W (DEVICE_ADDR & 0xFE)

#define IOMN (2)
#define I2C_FREQ (AM_HAL_IOM_400KHZ)
void* iom_handle = NULL;
am_hal_iom_config_t iom_cfg = {0};
am_hal_iom_transfer_t xfer = {0};

#define report(s) am_util_stdio_printf("status: 0x%08X (function: %s, file: %s, line: %d)\n", s, __PRETTY_FUNCTION__, __FILE__, __LINE__)

volatile bool xfer_complete = false;
volatile uint32_t txn_stat = 0;
void xfer_complete_callback(void *pCallbackCtxt, uint32_t transactionStatus){
    (void)pCallbackCtxt;
    xfer_complete = true;
    txn_stat = transactionStatus;
}

void init_iom( void ){
    uint32_t status = AM_HAL_STATUS_SUCCESS;

    iom_cfg.eInterfaceMode = AM_HAL_IOM_I2C_MODE;
    iom_cfg.ui32ClockFreq = I2C_FREQ;
    iom_cfg.pNBTxnBuf = NULL;
    iom_cfg.ui32NBTxnBufLength = 0;

#ifdef ASYNCH
    // iom_cfg.pNMTxnBuf = ???
    // iom_cfg.ui32NBTxnBufLength = ???
#endif // ASYNCH

    status = am_hal_iom_initialize(IOMN, &iom_handle);
    if(status != AM_HAL_STATUS_SUCCESS){ report(status); }

    status = am_hal_iom_power_ctrl(iom_handle, AM_HAL_SYSCTRL_WAKE, false);
    if(status != AM_HAL_STATUS_SUCCESS){ report(status); }

    status = am_hal_iom_configure(iom_handle, &iom_cfg);
    if(status != AM_HAL_STATUS_SUCCESS){ report(status); }

    status = am_hal_iom_enable(iom_handle);
    if(status != AM_HAL_STATUS_SUCCESS){ report(status); }

    // config pins
    status = am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_SCL, g_AM_BSP_GPIO_IOM2_SCL);
    if(status != AM_HAL_STATUS_SUCCESS){ report(status); }

    status = am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM2_SDA, g_AM_BSP_GPIO_IOM2_SDA);
    if(status != AM_HAL_STATUS_SUCCESS){ report(status); }
}


//*****************************************************************************
//
// Main
//
//*****************************************************************************
int
main(void)
{
    uint32_t status = AM_HAL_STATUS_SUCCESS;

    //
    // Perform the standard initialzation for clocks, cache settings, and
    // board-level low-power operation.
    //
    am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_SYSCLK_MAX, 0);
    am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
    am_hal_cachectrl_enable();
    am_bsp_low_power_init();

    //
    // Initialize the printf interface for UART output
    //
    am_bsp_uart_printf_enable();

    //
    // Print the banner.
    //
    am_util_stdio_terminal_clear();
    am_util_stdio_printf("I2C Testing\n\n");


    // do i2c stuff
    init_iom();

    char cmd[2];

    cmd[0] = 0x01;
    cmd[1] = 0x00;
    // i2c.write(addr8bit, cmd, 2);

    xfer.uPeerInfo.ui32I2CDevAddr = DEVICE_ADDR_W;
    xfer.ui32InstrLen = 0;
    xfer.ui32Instr = 0;
    xfer.ui32NumBytes = 2;
    xfer.eDirection = AM_HAL_IOM_TX;
    xfer.pui32TxBuffer = (uint32_t*)cmd;
    xfer.pui32RxBuffer = NULL;
    xfer.bContinue = false;
    xfer.ui8RepeatCount = 0;
    xfer.ui8Priority = 1;
    xfer.ui32PauseCondition = 0;
    xfer.ui32StatusSetClr = 0;


#ifndef AM_HAL_DISABLE_API_VALIDATION
    am_util_stdio_printf("API Validation is enabled\n");
#else
    am_util_stdio_printf("API Validation DISabled\n");
#endif


#ifdef ASYNCH
    status = am_hal_iom_nonblocking_transfer(iom_handle, &xfer, xfer_complete_callback, NULL);
    report(status);

    while(!xfer_complete){
        static uint32_t count = 0;
        am_util_stdio_printf("waiting for xfer to complete... %d\n", count++);
        if(count > 0x0000FFFF){
            am_util_stdio_printf("!TIME OUT!\n");
            break;
        }
    }
#else
    status = am_hal_iom_blocking_transfer(iom_handle, &xfer);
    report(status);
#endif // ASYNCH

    const char* stat_msg = NULL;
    switch(status){
        case AM_HAL_STATUS_SUCCESS : stat_msg = "success"; break;
        case AM_HAL_IOM_ERR_I2C_NAK : stat_msg = "NAK"; break;
        case AM_HAL_IOM_ERR_I2C_ARB : stat_msg = "ARB"; break;
        
        default:
            stat_msg = "UNKNOWN ERROR";
            break;
    }
    am_util_stdio_printf("I2C write result: %s\n", stat_msg);

    // cmd[0] = 0x00;
    // i2c.write(addr8bit, cmd, 1);
    // i2c.read(addr8bit, cmd, 2);


    //
    // Loop forever while sleeping.
    //
    while (1)
    {
        //
        // Go to Deep Sleep.
        //
        am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
    }
}
