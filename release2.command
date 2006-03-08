#!/bin/sh

# DoubleCommand release script 2
# 
# $Id$

version=`cat dcversion`
os=10.4

# before running this, open the package builder and build the package.

HDIUTIL=$(which hdiutil)


# make a new folder called DoubleCommand, and put the install package, the tar.gz,
# and the html and css and gif files, along with the license.
# make a disk image of this folder, and put it in the release folder.
tmpDC=/tmp/501/dc2
mkdir $tmpDC
mkdir $tmpDC/DoubleCommand
mkdir $tmpDC/DoubleCommand/Documentation
here=`pwd`

cp -Rp ~/Desktop/DoubleCommand.pkg $tmpDC/DoubleCommand/DoubleCommand$version\_$os.pkg
# now that the Apple package is proven, leave the tar.gz as a separate download
#cp $here/Release/DoubleCommand$version\_$os.tar.gz $tmpDC/DoubleCommand
cp $here/docs/*.css $tmpDC/DoubleCommand/Documentation
cp $here/docs/*.html $tmpDC/DoubleCommand/Documentation
cp $here/docs/Licence.txt $tmpDC/DoubleCommand/Documentation
cp $here/docs/*.gif $tmpDC/DoubleCommand/Documentation
cp $here/docs/disk\ image\ resources/Documentation/*.png $tmpDC/DoubleCommand/Documentation
cp $here/docs/disk\ image\ resources/.DS_Store $tmpDC/DoubleCommand

${HDIUTIL} create -fs HFS+ -srcfolder ${tmpDC}/DoubleCommand \
    -volname DoubleCommand$version\_$os \
    ${here}/../../Release/DoubleCommand$version\_$os.dmg 2>&1 >/dev/null

rm -fr $tmpDC
exit

# then upload the .dmg, tar.gz and website files to the site.
# using upload.command

#then update versiontracker.com, macupdate.com and send email to mailing list.

#ssh mbaltaks@shell.sourceforge.net

