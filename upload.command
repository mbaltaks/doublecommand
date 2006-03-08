#!/bin/sh

# DoubleCommand upload script
# 
# $Id$

version=`cat dcversion`
os=10.4
cd ~/Development/DoubleCommand/svn/trunk/build
#scp DoubleCommand$version\_$os.tar.gz mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/htdocs/files
scp DoubleCommand$version\_$os.dmg mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/htdocs/files
cd ~/Development/DoubleCommand/svn/trunk/docs
scp *.html mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/htdocs
scp *.css mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/htdocs
