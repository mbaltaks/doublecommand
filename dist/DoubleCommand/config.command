#!/bin/sh

# DoubleCommand configure script
# 
# $Id$

# we want to run in the same folder as this script, not
# the users home folder
#cd /Library/StartupItems/DoubleCommand/
here=`dirname "$0"`
cd "$here"

./DoubleCommand config
echo "DoubleCommand configured, it is now safe to close this window."
