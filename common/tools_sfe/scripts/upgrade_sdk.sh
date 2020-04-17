#!/usr/bin/env bash
# requires:

# a script to aid in upgrading the AmbiqSuite SDK

# # get enclosing directory
# DIR=$(dirname -- "$(readlink -f -- "$BASH_SOURCE")") # can handle symlinks but fails on mac
DIR=$(dirname -- "$BASH_SOURCE") # assumes no symlinks.... (for mac compatibility)

# import sh-realpath
source $DIR/utility/sh-realpath/realpath.sh

# defaults
SDK_ROOT=$DIR/../../../..
UPGRADE_ROOT=

# handle arguments
while getopts ":r:u:" opt; do
  case $opt in
    r) SDK_ROOT="$OPTARG"
    ;;
    u) UPGRADE_ROOT="$OPTARG"
    ;;
    \?) echo "Invalid option -$OPTARG" 1>&2
    ;;
  esac
done

# ensure user provided -u option
if [ -z "$UPGRADE_ROOT" ]
then
  echo "User must provide -u option (path to upgrade sdk directory)"
  exit 1
fi


# make a temporary directory to hold the old sdk
TMP_DIR=$(mktemp -d -t ambiqsuite_sdk_upgrade_tmp)
echo "created temporary directory $TMP_DIR"

# get the sdk folder name (and verify)
SDK_ROOT=$(realpath $SDK_ROOT)
SDK_NAME=$(basename $SDK_ROOT)
echo "Got SDK name: $SDK_NAME"
VFILE=$SDK_ROOT/VERSION.txt
if [ -f "$VFILE" ]; 
then
    echo "\$SDK_ROOT verification passed" 1>&2
else
    echo "\$SDK_ROOT verification failed" 1>&2
    exit 1
fi

# get the upgrade folder (and verify)
UPGRADE_ROOT=$(realpath $UPGRADE_ROOT)
echo "Got upgrade SDK path: $UPGRADE_ROOT"
VFILE=$UPGRADE_ROOT/VERSION.txt
if [ -f "$VFILE" ]; 
then
    echo "\$UPGRADE_ROOT verification passed" 1>&2
else
    echo "\$UPGRADE_ROOT verification failed" 1>&2
    exit 1
fi

# copy sdk to temporary dir (being sure to keep hidden files)
cp -r $SDK_ROOT/. $TMP_DIR

# remove old directory
rm -rf $SDK_ROOT

# copy the upgrade to the SDK_ROOT
cp -r $UPGRADE_ROOT $SDK_ROOT

# copy important directories from the old SDK to the upgrade
cp -r $TMP_DIR/.git* $SDK_ROOT
cp -r $TMP_DIR/README.md $SDK_ROOT
cp -r $TMP_DIR/boards_sfe $SDK_ROOT

# clean up temporary folder
rm -rf $TMP_DIR
