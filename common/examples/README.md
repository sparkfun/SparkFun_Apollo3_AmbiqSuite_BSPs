Common Examples README
======================

The beauty of the Ambiq BSP system is that it can be used to create common example code. For example if we define the proper UART instance and TX/RX pads as part of the BSP then a single source file can be used to demonstrate UART functionality across all capable boards. To utilize this capability to the fullest the common examples source code shoul dbe isolated from specific boards. This reduces redundancy in maintenance.

This directory contains common source files that may be referred to by any of the boards included in this repo. For board-specific source code look beneath the respective board's directory.