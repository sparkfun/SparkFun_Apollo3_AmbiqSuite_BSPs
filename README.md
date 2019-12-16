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
cd boards_sfe/$YOUR_BOARD/examples/$YOUR_EXAMPLE/gcc
make
make clean
make bootload       # eqivalent to bootload_svl
make bootload_svl   # builds and bootloads with SparkFun Variable Loader - you must have this bootloader flashed onto your board
make bootload_asb   # builds and bootloads with Ambiq Secure Bootloader - should work with most all boards. If not try changing the baud rate
```

How to Generate BSP Files
=========================
See the [bsp_pinconfig README](https://github.com/sparkfun/SparkFun_Apollo3_AmbiqSuite_BSPs/tree/master/common/bsp_pinconfig/README.md)
