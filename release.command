# update version in dcversion file and Make.config
# make sure the Download links are right, and check the news and version history sections.
# make release
# now commit to svn and tag it.
svn commit -m "what has changed"
svn cp https://svn.sourceforge.net/svnroot/doublecommand/trunk https://svn.sourceforge.net/svnroot/doublecommand/tags/Release-[version]

# upload.command

#then update versiontracker.com, macupdate.com and send email to mailing list.

#ssh mbaltaks@shell.sourceforge.net









#!/bin/sh

# DoubleCommand release script
# 
# $Id$

# Later I should try to set the version in all the other places from here.
#make sure the Download links are right, and check the news and version history sections.
#update version in info.plist, target settings, package settings and web pages.

version=`cat dcversion`
os=10.4
# and perhaps commit to cvs as well?
# I'm assuming that has all been done for now.
#commit files to cvs:
#cd ~/Development/DoubleCommand/cvs
#dccvs commit -m "what has changed"
#dccvs tag -F Release-1_4_3

#now run this script.


# after running this, open the package builder and build the package.

tmpDC=/tmp/501/dc
mkdir -p $tmpDC
here=`pwd`

cp -Rp $here/dist/DoubleCommand $tmpDC
rm -fr $tmpDC/DoubleCommand/CVS
rm -fr $tmpDC/DoubleCommand/.DS_Store
cp -Rp $here/kext/build/Debug/DoubleCommand.kext $tmpDC/DoubleCommand
cp $here/docs/doublecommand.css $tmpDC/DoubleCommand
cp $here/docs/*.html $tmpDC/DoubleCommand
#cp $here/docs/index.html $tmpDC/DoubleCommand
#cp $here/docs/install.html $tmpDC/DoubleCommand
cp $here/docs/Licence.txt $tmpDC/DoubleCommand
cp $here/docs/*.gif $tmpDC/DoubleCommand
cd $tmpDC
gnutar -c -z -f DoubleCommand$version\_$os.tar.gz DoubleCommand/
cp DoubleCommand$version\_$os.tar.gz $here/../../Release
#scp DoubleCommand$version\_$os.tar.gz mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/

# Permissions:
# 1 - x bit
# 2 - w bit
# 4 - r bit

distroot=$here/../../dstroot

echo removing $distroot/Library/StartupItems/DoubleCommand
sudo rm -fr $distroot/Library/StartupItems/DoubleCommand
sudo cp -Rpf $tmpDC/DoubleCommand $distroot/Library/StartupItems

sudo chown -R root:wheel $distroot/Library
sudo chmod -R 775 $distroot/Library
sudo chmod -R 555 $distroot/Library/PreferencePanes/DoubleCommandPreferences.prefPane
sudo chmod 444 $distroot/Library/PreferencePanes/DoubleCommandPreferences.prefPane/Contents/Info.plist
sudo chmod 444 $distroot/Library/PreferencePanes/DoubleCommandPreferences.prefPane/Contents/Resources/DoubleCommand.tiff
sudo chmod 444 $distroot/Library/PreferencePanes/DoubleCommandPreferences.prefPane/Contents/Resources/NSAuthenticationLock.tiff
sudo chmod 444 $distroot/Library/PreferencePanes/DoubleCommandPreferences.prefPane/Contents/Resources/NSAuthenticationUnlock.tiff
sudo chmod -R 555 $distroot/Library/StartupItems/DoubleCommand
sudo chmod -R 444 $distroot/Library/StartupItems/DoubleCommand/DoubleCommand.pref
sudo chmod -R 444 $distroot/Library/StartupItems/DoubleCommand/Licence.txt
sudo chmod -R 444 $distroot/Library/StartupItems/DoubleCommand/StartupParameters.plist
sudo chmod -R 444 $distroot/Library/StartupItems/DoubleCommand/doublecommand.css
sudo chmod -R 444 $distroot/Library/StartupItems/DoubleCommand/download.html
sudo chmod -R 444 $distroot/Library/StartupItems/DoubleCommand/index.html
sudo chmod -R 444 $distroot/Library/StartupItems/DoubleCommand/install.html
sudo chmod -R 444 $distroot/Library/StartupItems/DoubleCommand/valid-xhtml10.gif
#sudo chmod -R 444 $distroot/Library/StartupItems/DoubleCommand/valid-xhtml10.png
cd $distroot/Library/StartupItems/DoubleCommand
#echo "Correcting permissions for DoubleCommand..."
sudo chown -R root:wheel ./DoubleCommand.kext
sudo chmod -R 755 ./DoubleCommand.kext
sudo chmod 644 ./DoubleCommand.kext/Contents/Info.plist
sudo chmod 644 ./DoubleCommand.kext/Contents/MacOS/DoubleCommand
sudo chmod 644 ./DoubleCommand.kext/Contents/Resources/Info.plist
sudo chmod 644 ./DoubleCommand.kext/Contents/Resources/English.lproj/InfoPlist.strings
rm -fr $tmpDC
exit
