#!/bin/sh

# DoubleCommand login configure script

sysctl=/usr/sbin/sysctl

if [ ! -z "$1" ]
then
	if [ -e "/Users/$1/Library/Preferences/DoubleCommand.pref" ]; then
		config_value=`cat /Users/$1/Library/Preferences/DoubleCommand.pref`
		$sysctl -w dc.config=$config_value
	fi
fi
