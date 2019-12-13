#!/usr/bin/env bash
# requires:
# make

# example usage
# locations:
#   from bsp repo root:
#     common/bsp_pinconfig/scripts/regen_bsp_libs.sh
#   from remote location: (requires $AMSDK environment variable -- {boards_sfe} can be whatever you named the root of the bsp repo )
#     $AMSDK/{boards_sfe}/common/bsp_pinconfig/scripts/regen_bsp_libs.sh -r $AMSDK/{boards_sfe} 
# arguments:
#   [-r $BSP_ROOT]      path to bsp root      optional -- defaults to the current directory 
#                                                         (should be specified when calling script remotely)
#   [-b $BOARDS_FILE]   path to boards file   optional -- defaults to all supported bsp boards 
#                                                         (for now boards must still have source files located in the bsp repo)

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

# verify bsp root
echo "Using \$BSP_ROOT=$BSP_ROOT" 1>&2
VFILE=$BSP_ROOT/README.md
if [ -f "$VFILE" ]; 
then
    echo "\$BSP_ROOT verification passed" 1>&2
else
    echo "\$BSP_ROOT verification failed" 1>&2
    exit 1
fi

# load in boards to handle
echo "Using \$BOARDS_FILE=$BOARDS_FILE" 1>&2
source $BOARDS_FILE

echo "" 1>&2
for value in $BOARDS
do
    echo "Cleaning all examples for: $value" 1>&2

  # https://unix.stackexchange.com/questions/86722/how-do-i-loop-through-only-directories-in-bash
  # https://stackoverflow.com/questions/4515866/iterate-through-subdirectories-in-bash
  # https://stackoverflow.com/questions/9018723/what-is-the-simplest-way-to-remove-a-trailing-slash-from-each-parameter
  # shopt -s nullglob
  for f in $BSP_ROOT/$value/examples/*/; do
    if [[ "$f" = "*" ]]; then continue; fi # protect from empty directories
    if [[ -d "$f" && ! -L "$f" ]]; then
      # # $f is a directory and is not a symlink
      # echo "Removing: $f/gcc/bin"
      # rm -rf $f/gcc/bin
      echo "cleaning ${f%/}"
      make -f ${f%/}/gcc/Makefile clean PROJECTPATH=${f%/}
    fi
  done
done