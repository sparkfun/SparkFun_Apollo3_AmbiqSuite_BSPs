#!/usr/bin/env bash

BOARDS="edge edge2 artemis artemis_thing_plus redboard_artemis redboard_artemis_atp redboard_artemis_nano"

for value in $BOARDS
do
    echo "Regenerating bsp_pins files for: $value"
    common/bsp_pinconfig/pinconfig.py $value/bsp/bsp_pins.src h > $value/bsp/am_bsp_pins.h
    common/bsp_pinconfig/pinconfig.py $value/bsp/bsp_pins.src c > $value/bsp/am_bsp_pins.c
done