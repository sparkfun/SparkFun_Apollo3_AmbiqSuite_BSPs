Welcome!
========
This is the location to get the latest Board Support Packages (BSPs) for SparkFun Apollo3 (Artemis) based boards to be used in the AmbiqSuite SDK.

Prerequisites
=============
* AmbiqSuiteSDK - contains source code to support the Apollo3 microcontroller (easy mode: [clone/fork our mirror here](https://github.com/sparkfun/AmbiqSuiteSDK))
* arm-none-eabi-gcc toolchain accessible at your cmmand line (recommend version [8-2018-q4-major](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads#panel2a) or lower)
* make
* python3 (only needed for bootloading)

Installation
============
Usage of these BSPs is quite flexible, however the easiest way to use them is to clone into your AmbiqSuite SDK directly. For example:

``` bash
cd $AM_SDK_ROOT # Enter the root of your AmbiqSuite SDK
git clone https://github.com/sparkfun/SparkFun_Apollo3_AmbiqSuite_BSPs boards_sfe # Clone this repo into a directory called 'boards_sfe'

# Assuming you have your ARMCC (arm-none-eabi-xxx) toolchain installed you can then build examples
YOUR_BOARD=redboard_artemis_atp       # choose the directory name of the board you want to use
cd boards_sfe/common/examples/$YOUR_EXAMPLE/gcc
make BOARD=$YOUR_BOARD
make BOARD=$YOUR_BOARD clean
make BOARD=$YOUR_BOARD bootload       # eqivalent to bootload_svl
make BOARD=$YOUR_BOARD bootload_svl   # builds and bootloads with SparkFun Variable Loader - you must have this bootloader flashed onto your board
make BOARD=$YOUR_BOARD bootload_asb   # builds and bootloads with Ambiq Secure Bootloader - should work with most all boards. If not try changing the baud rate
```

Advanced Usage
================

## Make Line Options
A basic build can be acheived with a simple command like ```make BOARD=redbaord_artemis bootload``` however this usage assumes a lot of things. You can override the default values for these options:

Name | Default Value | Description
--- | --- | ---
```BOARD``` | | Optional short syntax to select baord bsps that exist in the root directory of this repo
```BOARDPATH``` | | Directly specify the path to the board directory that contains desired bsp files. Will be ignored if ```BOARD``` is provided
```COM_PORT``` | COM4 | USB serial port that will be used in bootload steps. Can be Windows or \*nix style (COMX or /dev/xxx)
```PYTHON3``` | python3 | Command used to invoke Python3.x on the command line
```ASB_UPLOAD_BAUD``` | 115200 | Baud rate used for the Ambiq Secure Bootloader
```SVL_UPLOAD_BAUD``` | 921600 | Baud rate used for the SparkFun Variable Loader (configurable to common values 921600, 460800, 230400, 115200, and 57600)
```SDKPATH``` | ../../../../.. | Path used to locate the root of the AmbiqSuiteSDK (change this if installing BSPs in a non-standard location)
```COMMONPATH``` | ../../../../common | Path used to locate the common directory of this repo (change if compiling a project from a non-standard location (aka not from ~/common/examples/{example}/gcc))
```PROJECTPATH``` | .. | Path to project directory - the template makefile will automatically add $(PROJECTPATH)/src to the directories to search for included headers and source files


## Using Custom BSPs
When compiling pretty much any example the SDK relies on the BSP (Board Support Package) definitions to specialize to that board and its capabilities. In the basic examples above we use ```BOARD=$NAME_OF_BOARD_DIR``` to specify a SparkFun board. When the ```BOARD``` variable is set the Makefile will automatically configure the path used to find the BSP directory as ```../../../../$NAME_OF_BOARD_DIR```. This path, relative to the build directory ```~/common/examples/{example}/gcc``` finds the board directory in the root of the BSP repo. 

What if you want to use a custom BSP for your own board? Firstly, you can generate BSP files by following the [bsp_pinconfig README](https://github.com/sparkfun/SparkFun_Apollo3_AmbiqSuite_BSPs/blob/master/common/bsp_pinconfig/README.md). You should put your BSP files in a directory called ```bsp``` and enclose that in a directory named after your board. Once you have done that there are two options.

1. Instead of using ```BOARD=``` you can directly specify the path to the board directory on the make lines like this: ```make BOARDPATH=../some/relative/or/absolute/path/to/your/board/directory bootload```
1. You can hijack the system so that your baord works with the shortcut ```BOARD=```. Do this by placing your board directory next to the SparkFun boards in your copy of the repo.

How to Generate BSP Files
=========================
See the [bsp_pinconfig README](https://github.com/sparkfun/SparkFun_Apollo3_AmbiqSuite_BSPs/tree/master/common/bsp_pinconfig/README.md)
