#!/bin/sh

# DoubleCommand upload script
# 
# $Id$

version=`cat dcversion`
scp build/DoubleCommand-$version.dmg mbaltaks@web.sourceforge.net:/home/groups/d/do/doublecommand/htdocs/files
scp docs/*.html mbaltaks@web.sourceforge.net:/home/groups/d/do/doublecommand/htdocs
scp docs/*.css mbaltaks@web.sourceforge.net:/home/groups/d/do/doublecommand/htdocs
scp docs/*.gif mbaltaks@web.sourceforge.net:/home/groups/d/do/doublecommand/htdocs
scp docs/*.txt mbaltaks@web.sourceforge.net:/home/groups/d/do/doublecommand/htdocs
