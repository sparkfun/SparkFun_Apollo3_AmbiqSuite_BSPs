#!/usr/bin/env bash
# run me from the repo root

source boards.sh

for value in $BOARDS
do
    echo "Regenerating bsp library for: $value"
    make -C $value/bsp/gcc
done