#!/usr/bin/env bash
# this script relies on the environment variable $AMSDK being set to the root of your AmbiqSuite SDK
# use it when you have a pre-built binary file to upload onto a board

# default variables
BINFILE=.
ASB_UPLOAD_BAUD=115200
PORT=COM4

# immutables
COMMONPATH=$AMSDK/boards_sfe/common
PYTHON3=python3
AMBIQ_BIN2BOARD=$COMMONPATH/tools_sfe/asb/asb.py
BINPATH=temp_

# handle arguments
while getopts ":p:f:b:u:" opt; do
  case $opt in
    p) PORT="$OPTARG"
    ;;
    f) BINFILE="$OPTARG"
    ;;
    b) ASB_UPLOAD_BAUD="$OPTARG"
    ;;
    u) UPLOADER="$OPTARG"
    ;;
    \?) echo "Invalid option -$OPTARG" >&2
    ;;
  esac
done

printf "Uploading file with Ambiq Secure Bootloader:\n%s\n\n" "$BINFILE"

$PYTHON3 $AMBIQ_BIN2BOARD --bin $BINFILE --load-address-blob 0x20000 --magic-num 0xCB -o $BINPATH --version 0x0 --load-address-wired 0xC000 -i 6 --options 0x1 -b $ASB_UPLOAD_BAUD -port $PORT -r 2 -v 
