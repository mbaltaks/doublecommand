#!/bin/sh

# DoubleCommand uninstall script

echo ""
echo "You will need to provide your password to uninstall DoubleCommand"
echo ""

# Remove the main folder
dcfolder=/Library/StartupItems/DoubleCommand

if [ -e ${dcfolder} ]; then
	echo removing ${dcfolder}
	sudo rm -fr ${dcfolder}
fi

# Removed the crash file indicator
dccrashed=/var/tmp/DoubleCommand.crashed

if [ -e ${dccrashed} ]; then
	echo removing ${dccrashed}
	sudo rm -f ${dccrashed}
fi

# Remove the preference pane
pref=/Library/PreferencePanes/DoubleCommandPreferences.prefPane

if [ -e ${pref} ]; then
	echo removing ${pref}
	sudo rm -rf ${pref}
fi

# Remove the package receipts
echo removing /Library/Receipts/DoubleCommand*.pkg
sudo rm -rf /Library/Receipts/DoubleCommand*.pkg

# Now unload any existing kexts
kext_ids="com.baltaks.kext.DoubleCommand"
kext_ids="$kext_ids com.baltaks.driver.DoubleCommand"
kextunload=/sbin/kextunload
kextstat=/usr/sbin/kextstat

for k in ${kext_ids}; do
	kexts=$(${kextstat} -l -b ${k} | wc -l)
	if [ ${kexts} -gt 0 ]; then
		sudo ${kextunload} -b ${k}
	fi
done

echo ""
echo "DoubleCommand is now removed, you can close this window."
echo ""
