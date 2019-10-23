#******************************************************************************
#
# Makefile - Rules for building the libraries, examples and docs.
#
# Copyright (c) 2019, Ambiq Micro
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
# 
# 3. Neither the name of the copyright holder nor the names of its
# contributors may be used to endorse or promote products derived from this
# software without specific prior written permission.
# 
# Third party software included in this distribution is subject to the
# additional license terms as defined in the /docs/licenses directory.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# This is part of revision 2.1.0 of the AmbiqSuite Development Package.
#
#******************************************************************************

#******************************************************************************
#
# This is an example makefile for SparkFun Apollo3 boards as used in the 
#   AmbiqSuite SDK.
#
# Recommended usage
#   make
#   make bootload_svl (uses the SparkFun Variable Loader to upload code)
#   make bootload_asb (uses the Ambiq Secure Bootlaoder to upload code)
#   make clean
#
# Filepaths
#   You can relocate this makefile easily by providing the path to the root of
#   the AmbiqSuite SDK. If that path is not specified then this file will 
#   assume that it is located in 
#   <AmbiqSDKRoot>/boards/<your_board>/examples/<your_example>/gcc
#   and use relative paths
#
# User Configuration
#   You must also specify which COM_PORT to use if you want to use the 
#   'bootlaoder' targets. 
#     Windows example: 	COM_PORT=COM4
#     *nix example: 	COM_PORT=/dev/usbserialxxxx
#
# Python vs. Executable
#   For simplicity the upload tools are called as Python scripts by default.
#   Make sure PYTHON is set to the appropriate command to run Python3 from the
#   command line.
#
#******************************************************************************


#******************************************************************************
#
# User Options
#
#******************************************************************************

# COM_PORT is the serial port to use for uploading. For example COM#### on Windows or /dev/cu.usbserial-#### on *nix
COM_PORT        ?=
# ASB_UPLOAD_BAUD is the baud rate setting of the Ambiq Secue Bootloader (ASB) as it is configured on the Apollo3. Defautls to 115200 if unset
ASB_UPLOAD_BAUD ?=
# SVL_UPLOAD_BAUD is the baud rate setting of the SparkFun Variable Loader (SVL). Defaults to 921600 if unset
SVL_UPLOAD_BAUD ?=
# PYTHON3 should evaluate to a call to the Python3 executable on your machine
PYTHON3         ?=

# *Optionally* specify absolute paths to the SDK and the BSP
# Make sure to use / instead of \ when on Windows
SDKPATH			?=# Set as the path to the SDK root if not located at ../../../../..
COMMONPATH		?=# Set as the path to the BSP common folder if not located at ../../../../common
BOARDPATH		?=# Set as the path to the board if not located at ../../..
PROJECTPATH		?=# Set as the path to the project if not located at ..


#******************************************************************************
#
# Warning Messages
#
#******************************************************************************
ifeq ($(COM_PORT),)
    COM_PORT=COM4
    $(warning warning: you have not defined COM_PORT. Assuming it is COM4)
endif
ifeq ($(PYTHON3),)
    PYTHON3=python3
    $(warning warning: you have not defined PYTHON3. assuming it is accessible by 'python3')
endif
ifeq ($(ASB_UPLOAD_BAUD),)
    ASB_UPLOAD_BAUD=115200
    $(warning defaulting to 115200 baud for ASB)
endif
ifeq ($(SVL_UPLOAD_BAUD),)
    SVL_UPLOAD_BAUD=921600
    $(warning defaulting to 921600 baud for SVL)
endif

ifeq ($(SDKPATH),)
    SDKPATH			=../../../../..
    $(warning warning: you have not defined SDKPATH so will continue assuming that the SDK root is at $(SDKPATH))
else
# When the SDKPATH is given export it
export SDKPATH
endif

ifeq ($(COMMONPATH),)
    COMMONPATH			=../../../../common
    $(warning warning: you have not defined COMMONPATH so will continue assuming that the COMMON root is at $(COMMONPATH))
else
# When the COMMONPATH is given export it
export COMMONPATH
endif

ifeq ($(BOARDPATH),)
    BOARDPATH			=../../..
    $(warning warning: you have not defined BOARDPATH so will continue assuming that the BOARD root is at $(BOARDPATH))
else
# When the BOARDPATH is given export it
export BOARDPATH
endif

ifeq ($(PROJECTPATH),)
    PROJECTPATH			=..
    $(warning warning: you have not defined PROJECTPATH so will continue assuming that the PROJECT root is at $(PROJECTPATH))
else
# When the PROJECTPATH is given export it
export PROJECTPATH
endif

#******************************************************************************
#
# Warning Messages
#
#******************************************************************************
### Bootloader Tools
AMBIQ_BIN2BOARD=$(PYTHON3) $(COMMONPATH)/tools_sfe/ambiq/ambiq_bin2board.py
ARTEMIS_SVL=$(PYTHON3) $(COMMONPATH)/tools_sfe/artemis/artemis_svl.py


SHELL:=/bin/bash
#### Setup ####


#******************************************************************************
#
# Targets / Rules
#
#******************************************************************************
all: bootload_asb

bootload_asb: bin/person_detection.bin
	$(AMBIQ_BIN2BOARD) --bin bin/person_detection.bin --load-address-blob 0x20000 --magic-num 0xCB -o bin/person_detection --version 0x0 --load-address-wired 0xC000 -i 6 --options 0x1 -b $(ASB_UPLOAD_BAUD) -port $(COM_PORT) -r 2 -v 

bootload: bootload_asb

clean:
	@echo "Cleaning..." ;\

$(SDKPATH)/mcu/apollo3/hal/gcc/bin/libam_hal.a:
	$(MAKE) -C $(SDKPATH)/mcu/apollo3/hal/gcc

$(SDKPATH)/third_party/uecc/gcc/bin/lib_uecc.a:
	$(MAKE) -C $(SDKPATH)/third_party/uecc

# Automatically include any generated dependencies
-include $(DEPS)
.PHONY: all clean directories bootload bootload_asb bootload_svl
