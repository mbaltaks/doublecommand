#!/bin/sh

# DoubleCommand upload script
# 
# $Id$

version=`cat dcversion`
scp build/DoubleCommand-$version.dmg mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/htdocs/files
scp docs/*.html mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/htdocs
scp docs/*.css mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/htdocs
scp docs/*.gif mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/htdocs
scp docs/*.txt mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/htdocs
