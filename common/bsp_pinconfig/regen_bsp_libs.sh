#!/usr/bin/env bash
# run me from the repo root
# requires:
# - this repo existing in a directory under ${AM_SDK_ROOT}
# - 'make' available at the command line
# - 'arm-none-eabi-xxx' available at the command line (preferred version is q4-2018-major)

source boards.sh

for value in $BOARDS
do
    echo "Regenerating bsp library for: $value"
    make -C $value/bsp/gcc
done