#!/bin/sh

# DoubleCommand upload script
# 
# $Id$

version=`cat dcversion`
os=10.4
scp build/DoubleCommand$version\_$os.dmg mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/htdocs/files
scp docs/*.html mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/htdocs
scp docs/*.css mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/htdocs
