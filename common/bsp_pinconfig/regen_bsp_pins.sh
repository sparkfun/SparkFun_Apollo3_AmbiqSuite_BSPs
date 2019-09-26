#!/usr/bin/env bash
# run me from the repo root
# requires:
# - python3 available at the command line

source boards.sh

for value in $BOARDS
do
    echo "Regenerating bsp_pins files for: $value"
    common/bsp_pinconfig/pinconfig.py $value/bsp/bsp_pins.src h > $value/bsp/am_bsp_pins.h
    common/bsp_pinconfig/pinconfig.py $value/bsp/bsp_pins.src c > $value/bsp/am_bsp_pins.c
done