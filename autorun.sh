#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "ERROR: This script must be run with root permisions"
  exit
fi
   
APP='qcodedesk'
DIR=`pwd`
APP_PATH=$DIR/$APP

echo Path to the executable: $APP_PATH
echo Generating script..

SCRIPT=`echo -e '#!/bin/bash\n' $APP_PATH`
echo Generated script: $SCRIPT
echo Copying script to the /etc/init.d/$APP directory..
echo $SCRIPT > /etc/init.d/$APP
echo Copied.
echo Giving exec permisions to the script..
chmod +x  /etc/init.d/$APP
echo DONE.

