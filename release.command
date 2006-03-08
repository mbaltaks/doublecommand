#!/bin/sh

# DoubleCommand release script
# 
# $Id$

# - Update version in dcversion file and Make.config.
# - Make sure the Download links are right, and check the news and version history sections.
# - Run this script.

make release
version=`cat dcversion`
svn commit -m "commit for Release-${version}"
svn cp https://svn.sourceforge.net/svnroot/doublecommand/trunk https://svn.sourceforge.net/svnroot/doublecommand/tags/Release-${version}
./upload.command

# Now go update versiontracker.com, macupdate.com and send email to mailing list.
