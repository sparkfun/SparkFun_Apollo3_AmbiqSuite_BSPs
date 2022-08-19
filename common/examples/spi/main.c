#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"

// #define ASYNCH // comment out to use blocking methods

#define IOMN (0)
#define SPI_MODE (3)
#define SPI_FREQ (AM_HAL_IOM_4MHZ)
void* iom_handle = NULL;
am_hal_iom_config_t iom_cfg = {0};
am_hal_iom_transfer_t xfer = {0};

#define CS_PIN (16)

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

    iom_cfg.eInterfaceMode = AM_HAL_IOM_SPI_MODE;
    iom_cfg.ui32ClockFreq = SPI_FREQ;
    iom_cfg.eSpiMode = SPI_MODE;
    iom_cfg.pNBTxnBuf = NULL;
    iom_cfg.ui32NBTxnBufLength = 0;

    status = am_hal_iom_initialize(IOMN, &iom_handle);
    if(status != AM_HAL_STATUS_SUCCESS){ report(status); }

    status = am_hal_iom_power_ctrl(iom_handle, AM_HAL_SYSCTRL_WAKE, false);
    if(status != AM_HAL_STATUS_SUCCESS){ report(status); }

    status = am_hal_iom_configure(iom_handle, &iom_cfg);
    if(status != AM_HAL_STATUS_SUCCESS){ report(status); }

    status = am_hal_iom_enable(iom_handle);
    if(status != AM_HAL_STATUS_SUCCESS){ report(status); }

    // config pins
    status = am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MISO, g_AM_BSP_GPIO_IOM0_MISO);
    if(status != AM_HAL_STATUS_SUCCESS){ report(status); }

    status = am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_MOSI, g_AM_BSP_GPIO_IOM0_MOSI);
    if(status != AM_HAL_STATUS_SUCCESS){ report(status); }

    status = am_hal_gpio_pinconfig(AM_BSP_GPIO_IOM0_SCK, g_AM_BSP_GPIO_IOM0_SCK);
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
    am_util_stdio_printf("PDM FFT example.\n\n");


    // do spi stuff
    init_iom();

    char cmd[2];
    cmd[0] = 0xEC;
    cmd[1] = 0xC3;

    xfer.uPeerInfo.ui32SpiChipSelect = 0;
    xfer.ui32InstrLen = 0;
    xfer.ui32Instr = 0;
    xfer.ui32NumBytes = 1;
    xfer.eDirection = AM_HAL_IOM_TX;
    xfer.pui32TxBuffer = (uint32_t*)cmd;
    xfer.pui32RxBuffer = NULL;
    xfer.bContinue = false;
    xfer.ui8RepeatCount = 0;
    xfer.ui8Priority = 1;
    xfer.ui32PauseCondition = 0;
    xfer.ui32StatusSetClr = 0;

#ifdef ASYNCH
    status = am_hal_iom_nonblocking_transfer(iom_handle, &xfer, xfer_complete_callback, NULL);
    report(status);
#else
    status = am_hal_iom_blocking_transfer(iom_handle, &xfer);
    report(status);
#endif

    char rx_buf[10];

    xfer.eDirection = AM_HAL_IOM_FULLDUPLEX;
    xfer.pui32TxBuffer = (uint32_t*)cmd;
    xfer.pui32RxBuffer = (uint32_t*)rx_buf;
#ifdef ASYNCH
    status = am_hal_iom_nonblocking_transfer(iom_handle, &xfer, xfer_complete_callback, NULL);
    report(status);
#else
    status = am_hal_iom_spi_blocking_fullduplex(iom_handle, &xfer);
    report(status);
#endif


    while (1)
    {
#ifdef ASYNCH
        status = am_hal_iom_nonblocking_transfer(iom_handle, &xfer, xfer_complete_callback, NULL);
#else
        status = am_hal_iom_spi_blocking_fullduplex(iom_handle, &xfer);
#endif

        if(status != AM_HAL_STATUS_SUCCESS){
            report(status);
        }
    }
}
