#!/bin/sh

# DoubleCommand install script
# 
# $Id$

# probably could check for conflicting apps like uControl, iJect etc
# and if a kextstat | grep uControl gives output then unload them.

# we want to run in the same folder as this script, not
# the users home folder
cd `dirname $0`

# if the StartupItems folder doesn't exist, create it
if [ -e /Library/StartupItems ]; then
	echo ""
else
	sudo mkdir /Library/StartupItems
	sudo chmod g+w /Library/StartupItems
fi

# if there is an old DoubleCommand already installed, unload
# and remove it
if [ -e /Library/StartupItems/DoubleCommand ]; then
	echo "Unloading the old DoubleCommand..."
	sudo kextunload /Library/StartupItems/DoubleCommand/DoubleCommand.kext
	
	echo "Backing up the config..."
	#oldconfig=`cat /Library/StartupItems/DoubleCommand/DoubleCommand.pref`
	sudo cp -f /Library/StartupItems/DoubleCommand/DoubleCommand.pref ./DoubleCommand.pref
	
	echo "Removing the old DoubleCommand..."
	sudo rm -fr /Library/StartupItems/DoubleCommand
fi

echo "Installing DoubleCommand..."
sudo cp -R -p ../DoubleCommand /Library/StartupItems

cd /Library/StartupItems/DoubleCommand/

echo "Correcting permissions for DoubleCommand..."
sudo chown -R root:wheel ./DoubleCommand.kext
sudo chmod -R 755 ./DoubleCommand.kext
sudo chmod 644 ./DoubleCommand.kext/Contents/Info.plist
sudo chmod 644 ./DoubleCommand.kext/Contents/MacOS/DoubleCommand
sudo chmod 644 ./DoubleCommand.kext/Contents/Resources/English.lproj/InfoPlist.strings
sudo chmod 644 ./DoubleCommand.kext/Contents/pbdevelopment.plist
sudo ./DoubleCommand stop
sudo ./DoubleCommand start
