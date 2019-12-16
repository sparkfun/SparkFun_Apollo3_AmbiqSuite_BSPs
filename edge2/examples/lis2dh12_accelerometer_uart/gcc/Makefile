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
COM_PORT		?=
# ASB_UPLOAD_BAUD is the baud rate setting of the Ambiq Secue Bootloader (ASB) as it is configured on the Apollo3. Defautls to 115200 if unset
ASB_UPLOAD_BAUD	?=
# SVL_UPLOAD_BAUD is the baud rate setting of the SparkFun Variable Loader (SVL). Defaults to 921600 if unset
SVL_UPLOAD_BAUD	?=
# PYTHON3 should evaluate to a call to the Python3 executable on your machine
PYTHON3			?=

# *Optionally* specify absolute paths to the SDK and the BSP
# Make sure to use / instead of \ when on Windows
SDKPATH			?=# Set as the path to the SDK root if not located at ../../../../..
COMMONPATH		?=# Set as the path to the BSP common folder if not located at ../../../../common
BOARDPATH		?=# Set as the path to the board if not located at ../../..
PROJECTPATH		?=# Set as the path to the project if not located at ..

### Project Settings
TARGET := example
COMPILERNAME := gcc
PROJECT := $(TARGET)_gcc


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

# Build Dir
CONFIG := $(PROJECTPATH)/gcc/bin

#******************************************************************************
#
# User Defines / Includes / Sources / Libraries
#
#******************************************************************************

# Global Defines
DEFINES=  -DPART_$(PART)
DEFINES+= -DAM_CUSTOM_BDADDR
DEFINES+= -DAM_PACKAGE_BGA
DEFINES+= -DWSF_TRACE_ENABLED
DEFINES+= -DAM_DEBUG_PRINTF
DEFINES+= -DAM_PART_APOLLO3
DEFINES+=

# Includes (Add paths to where example header files are located)
INCLUDES=
INCLUDES+= -I$(PROJECTPATH)/src
INCLUDES+= -I$(BOARDPATH)/bsp
INCLUDES+= -I$(SDKPATH)
INCLUDES+= -I$(SDKPATH)/utils
INCLUDES+= -I$(SDKPATH)/devices
INCLUDES+= -I$(SDKPATH)/mcu/apollo3
INCLUDES+= -I$(SDKPATH)/CMSIS/AmbiqMicro/Include
INCLUDES+= -I$(SDKPATH)/CMSIS/ARM/Include
INCLUDES+= -I$(COMMONPATH)/third_party/lis2dh12
INCLUDES+=

# Compilation Units (Add all the .c files you need to compile)
SRC=
SRC+= main.c
SRC+= startup_gcc.c
SRC+= lis2dh12_reg.c
SRC+= lis2dh12_platform_apollo3.c
SRC+= am_util_delay.c
SRC+= am_util_stdio.c
SRC+=

# VPATH (Add paths to where your source files are located)
VPATH=
VPATH+= $(PROJECTPATH)/src
VPATH+= $(SDKPATH)/utils
VPATH+= $(COMMONPATH)/examples/lis2dh12_accelerometer_uart
VPATH+= $(COMMONPATH)/third_party/lis2dh12
VPATH+= $(COMMONPATH)/tools_sfe/templates
VPATH+=

# LIBS (Precompiled libraries to include in the linker step)
LIBS=
LIBS+= $(BOARDPATH)/bsp/gcc/bin/libam_bsp.a
LIBS+= $(SDKPATH)/mcu/apollo3/hal/gcc/bin/libam_hal.a
LIBS+=



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

TOOLCHAIN ?= arm-none-eabi
PART = apollo3
CPU = cortex-m4
FPU = fpv4-sp-d16
# Default to FPU hardware calling convention.  However, some customers and/or
# applications may need the software calling convention.
#FABI = softfp
FABI = hard

STARTUP_FILE := ./startup_$(COMPILERNAME).c

#### Required Executables ####
CC = $(TOOLCHAIN)-gcc
GCC = $(TOOLCHAIN)-gcc
CPP = $(TOOLCHAIN)-cpp
CXX = $(TOOLCHAIN)-g++
LD = $(TOOLCHAIN)-ld
CP = $(TOOLCHAIN)-objcopy
OD = $(TOOLCHAIN)-objdump
RD = $(TOOLCHAIN)-readelf
AR = $(TOOLCHAIN)-ar
SIZE = $(TOOLCHAIN)-size
RM = $(shell which rm 2>/dev/null)

EXECUTABLES = CC LD CP OD AR RD SIZE GCC CXX
K := $(foreach exec,$(EXECUTABLES),\
        $(if $(shell which $($(exec)) 2>/dev/null),,\
        $(info $(exec) not found on PATH ($($(exec))).)$(exec)))
$(if $(strip $(value K)),$(info Required Program(s) $(strip $(value K)) not found))

ifneq ($(strip $(value K)),)
all clean:
	$(info Tools $(TOOLCHAIN)-$(COMPILERNAME) not installed.)
	$(RM) -rf bin
else



#******************************************************************************
#
# Machinery
#
#******************************************************************************

XSRC = $(filter %.cpp,$(SRC))
ZSRC = $(filter %.cc,$(SRC))
CSRC = $(filter %.c,$(SRC))
ASRC = $(filter %.s,$(SRC))

OBJS = $(XSRC:%.cpp=$(CONFIG)/%.o)
OBJS+= $(ZSRC:%.cc=$(CONFIG)/%.o)
OBJS+= $(CSRC:%.c=$(CONFIG)/%.o)
OBJS+= $(ASRC:%.s=$(CONFIG)/%.o)

DEPS = $(XSRC:%.cpp=$(CONFIG)/%.d)
DEPS+= $(ZSRC:%.cc=$(CONFIG)/%.d)
DEPS+= $(CSRC:%.c=$(CONFIG)/%.d)
DEPS+= $(ASRC:%.s=$(CONFIG)/%.d)

CSTD = -std=c99 

CFLAGS = -mthumb -mcpu=$(CPU) -mfpu=$(FPU) -mfloat-abi=$(FABI)
CFLAGS+= -ffunction-sections -fdata-sections
CFLAGS+= -MMD -MP -Wall -g
CFLAGS+= -O0
CFLAGS+= $(DEFINES)
CFLAGS+= $(INCLUDES)
CFLAGS+= 

XSTD   = -std=gnu++11

XFLAGS = $(CFLAGS)
XFLAGS+= -fno-exceptions

LFLAGS = -mthumb -mcpu=$(CPU) -mfpu=$(FPU) -mfloat-abi=$(FABI)
LFLAGS+= -nostartfiles -static
LFLAGS+= -Wl,--gc-sections,--entry,Reset_Handler,-Map,$(CONFIG)/$(TARGET).map
LFLAGS+= -Wl,--start-group -lm -lc -lgcc $(LIBS) -Wl,--end-group
LFLAGS+= 

# Additional user specified CFLAGS
CFLAGS+=$(EXTRA_CFLAGS)

CPFLAGS = -Obinary

ODFLAGS = -S

#******************************************************************************
#
# Targets / Rules
#
#******************************************************************************
all: asb
asb: directories $(CONFIG)/$(TARGET)_asb.bin
svl: directories $(CONFIG)/$(TARGET)_svl.bin

directories: $(CONFIG)

$(CONFIG):
	@mkdir -p $@

$(CONFIG)/%.o: %.cpp $(CONFIG)/%.d
	@echo " Compiling $(COMPILERNAME) $<" ;\
	$(CXX) -c $(XSTD) $(XFLAGS) $< -o $@

$(CONFIG)/%.o: %.cc $(CONFIG)/%.d
	@echo " Compiling $(COMPILERNAME) $<" ;\
	$(CXX) -c $(XSTD) $(XFLAGS) $< -o $@

$(CONFIG)/%.o: %.c $(CONFIG)/%.d
	@echo " Compiling $(COMPILERNAME) $<" ;\
	$(CC) -c $(CFLAGS) $< -o $@

$(CONFIG)/%.o: %.s $(CONFIG)/%.d
	@echo " Assembling $(COMPILERNAME) $<" ;\
	$(CC) -c $(CFLAGS) $< -o $@

$(CONFIG)/$(TARGET)_asb.axf: LINKER_FILE = $(COMMONPATH)/tools_sfe/templates/asb_linker.ld
$(CONFIG)/$(TARGET)_asb.axf: $(OBJS) $(LIBS)
	@echo " Linking $(COMPILERNAME) $@ with script $(LINKER_FILE)";\
	$(CC) -Wl,-T,$(LINKER_FILE) -o $@ $(OBJS) $(LFLAGS)

$(CONFIG)/$(TARGET)_svl.axf: LINKER_FILE = $(COMMONPATH)/tools_sfe/templates/asb_svl_linker.ld
$(CONFIG)/$(TARGET)_svl.axf: $(OBJS) $(LIBS)
	@echo " Linking $(COMPILERNAME) $@ with script $(LINKER_FILE)";\
	$(CC) -Wl,-T,$(LINKER_FILE) -o $@ $(OBJS) $(LFLAGS)

$(CONFIG)/$(TARGET)_%.bin: $(CONFIG)/$(TARGET)_%.axf
	@echo " Copying $(COMPILERNAME) $@..." ;\
	$(CP) $(CPFLAGS) $< $@ ;\
	$(OD) $(ODFLAGS) $< > $(CONFIG)/$(TARGET).lst

bootload_asb: directories $(CONFIG)/$(TARGET)_asb.bin
	$(AMBIQ_BIN2BOARD) --bin $(CONFIG)/$(TARGET)_asb.bin --load-address-blob 0x20000 --magic-num 0xCB -o $(CONFIG)/$(TARGET) --version 0x0 --load-address-wired 0xC000 -i 6 --options 0x1 -b $(ASB_UPLOAD_BAUD) -port $(COM_PORT) -r 2 -v 

bootload_svl: directories $(CONFIG)/$(TARGET)_svl.bin
	$(ARTEMIS_SVL) $(COM_PORT) -f $(CONFIG)/$(TARGET)_svl.bin -b $(SVL_UPLOAD_BAUD) -v

bootload: bootload_svl

clean:
	@echo "Cleaning..." ;\
	$(RM) -R -f $(CONFIG)

$(CONFIG)/%.d: ;

$(SDKPATH)/mcu/apollo3/hal/gcc/bin/libam_hal.a:
	$(MAKE) -C $(SDKPATH)/mcu/apollo3/hal/gcc

$(SDKPATH)/third_party/uecc/gcc/bin/lib_uecc.a:
	$(MAKE) -C $(SDKPATH)/third_party/uecc

$(BOARDPATH)/bsp/gcc/bin/libam_bsp.a:
	$(MAKE) -C $(BOARDPATH)/bsp/gcc

# Automatically include any generated dependencies
-include $(DEPS)
endif
.PHONY: all clean directories bootload bootload_asb bootload_svl
