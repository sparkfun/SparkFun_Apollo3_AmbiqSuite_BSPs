#!/usr/bin/env bash
# requires:
# - see requirements of source scripts

# example usage
# locations:
#   from bsp repo root:
#     common/bsp_pinconfig/scripts/regen_bsps.sh
#   from remote location: (requires $AMSDK environment variable -- {boards_sfe} can be whatever you named the root of the bsp repo )
#     $AMSDK/{boards_sfe}/common/bsp_pinconfig/scripts/regen_bsps.sh -r $AMSDK/{boards_sfe} 
# arguments:
#   [-r $BSP_ROOT]      path to bsp root      optional -- defaults to the current directory 
#                                                         (should be specified when calling script remotely)
#   [-b $BOARDS_FILE]   path to boards file   optional -- defaults to all supported bsp boards 
#                                                         (for now boards bust still have source files located in the bsp repo)

# setup
set -e
set -o errexit
echo "" 1>&2

# get enclosing directory
DIR=$(dirname -- "$(readlink -f -- "$BASH_SOURCE")")

# defaults
BSP_ROOT=.
BOARDS_FILE=$DIR/configuration/boards.sh

# handle arguments
while getopts ":r:b:" opt; do
  case $opt in
    r) BSP_ROOT="$OPTARG"
    ;;
    b) BOARDS_FILE="$OPTARG"
    ;;
    \?) echo "Invalid option -$OPTARG" 1>&2
    ;;
  esac
done

# regen pins and libs
$BSP_ROOT/common/tools_sfe/scripts/regen_bsp_pins.sh -r $BSP_ROOT -b $BOARDS_FILE # regenerates source (.h and .c) files from the bsp_pins.src files
$BSP_ROOT/common/tools_sfe/scripts/regen_bsp_libs.sh -r $BSP_ROOT -b $BOARDS_FILE # regenerates library archive files from bsp source (.h and .c) files