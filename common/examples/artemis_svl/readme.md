SparkFun Variable Loader (SVL)
==========================

The Artemis module is loaded with two bootloaders: the ASB and SVL. The Ambiq Secure Boot Loader (ASB) resides from 0x00 to 0xC000. This bootloader is physically part of the IC and is configured using the info0 registers. At power up, if the ASB is not activated, it jumps to 0xC000. The SparkFun Variable Bootloader (SVL) resides at 0xC000 and will wait for an incoming serial character. If a character is received, the baud rate will be auto-detected, the SVL will load new code, then jump to the new user code starting at 0x10000. The SVL times out after 50ms.

This directory contains the source code for the *SVL*. A compiled binary is included with the Apollo3 core for Arduino as well as the Artemis Fimrware Upload GUI.

The bootloader for the Artemis can be upgraded via Arduino using [these instructions](https://learn.sparkfun.com/tutorials/designing-with-the-sparkfun-artemis/all#troubleshooting). It can also be upgraded using the [Artemis Firmware Upload GUI](https://github.com/sparkfun/Artemis-Firmware-Upload-GUI).
