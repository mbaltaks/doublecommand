#!/bin/sh

# DoubleCommand install script
# 
# $Id$

# probably could check for conflicting apps like uControl, iJect etc
# and if a kextstat | grep uControl gives output then unload them.

#echo "Installing DoubleCommand..."
#sudo mkdir /Library/StartupItems/
#sudo cp -R ../DoubleCommand /Library/StartupItems

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
