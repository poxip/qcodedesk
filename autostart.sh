#!/bin/bash

APP='qcodedesk'
APP_ARGS='--minimalized'
DIR=`pwd`
APP_PATH=$DIR/$APP' '$APP_ARGS

AUTOSTART_DIR=$HOME'/.config/autostart'

echo Path to the executable: $APP_PATH
echo Generating .desktop file..

SCRIPT="[Desktop Entry]\nName=$APP\nType=Application\nExec=$APP_PATH\n"
echo -e "Generated file:\n$SCRIPT"

mkdir -p $AUTOSTART_DIR
echo Copying file to $AUTOSTART_DIR/$APP'.desktop'
echo -e $SCRIPT > $AUTOSTART_DIR/$APP'.desktop'
echo Copied.
echo DONE.

