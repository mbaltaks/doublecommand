.PHONY: build versions clean package disk_image

# Project global make settings go in this file
include Make.config


# commands
chown=/usr/sbin/chown
chmod=/bin/chmod
hdiutil=/usr/bin/hdiutil


# places
tmp=build
image=$(tmp)/image


release: versions build package disk_image
	@echo built release package

versions:
	( cd kext; make versions )
	( cd prefpane; make versions )
	( cd package; make versions )

build:
	( cd kext; make build )
	( cd prefpane; make build )

clean:
	( cd kext; make clean )
	( cd prefpane; make clean )

package:
	( cd package; make build )

disk_image:
	rm -f $(tmp)/DoubleCommand$(VERSION)_$(OS).dmg
	rm -fr $(image)
	mkdir -p $(image)/Documentation
	cp docs/*.gif docs/*.html docs/*.css docs/*.txt $(image)/Documentation
	cp -R $(tmp)/DoubleCommand$(VERSION)_$(OS).pkg $(image)
	#cp image_resources/*.png $(image)
	#cp image_resources/_DS_Store $(image)/.DS_Store
	$(hdiutil) create -srcfolder $(image) \
	-volname DoubleCommand \
	$(tmp)/DoubleCommand$(VERSION)_$(OS).dmg

off:
	# Create an initial disk image (32 megs)
	hdiutil create -size 32m -fs HFS+ -volname "My Volume" myimg.dmg
	# Mount the disk image
	hdiutil attach myimg.dmg
	# Obtain device information
	DEVS=$(hdiutil attach myimg.dmg | cut -f 1)
	DEV=$(echo $DEV | cut -f 1 -d ' ')
	# Unmount the disk image
	hdiutil detach $DEV
	# Convert the disk image to read-only
	hdiutil convert myimg.dmg -format UDZO -o myoutputimg.dmg



# old section

# paths
DSTROOT=../../dstroot
STARTUPITEMS=$(DSTROOT)/Library/StartupItems
PREFPANES=$(DSTROOT)/Library/PreferencePanes
SVN_TRUNK=file:///Volumes/Data/Subversion/DoubleCommand/trunk
DC_FOLDER=$(STARTUPITEMS)/DoubleCommand


uninstall:
	kextunload -b com.baltaks.driver.DoubleCommand
	rm -rf /Library/PreferencePanes/DoubleCommandPreferences.prefPane
	rm -rf /Library/StartupItems/DoubleCommand
