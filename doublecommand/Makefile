# Project global make settings go in this file
include Make.config


# products
prefpane=build/DoubleCommand.prefPane
kext=build/DoubleCommand.kext

# paths
dist=build_pkg/dstroot
pkgres=build_pkg/pkgres
english=English.lproj/Localizable.strings
mystartup=../pkg_resources/StartupItems/uControl
packagemaker=/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker
TMP=build/tmp
DSTROOT=../../dstroot
STARTUPITEMS=$(DSTROOT)/Library/StartupItems
PREFPANES=$(DSTROOT)/Library/PreferencePanes
SVN_TRUNK=file:///Volumes/Data/Subversion/DoubleCommand/trunk
DC_FOLDER=$(STARTUPITEMS)/DoubleCommand

# versioned files
XCODE_PROJECT = kext/DoubleCommand.xcodeproj/project.pbxproj
INFO_PLIST = kext/Info.plist
EN_STRINGS = kext/English.lproj/InfoPlist.strings
VERSION = 1.6.1
OS=10.4

XCODE_PROJECT_COPY = project.pbxproj
INFO_PLIST_COPY = Info.plist
EN_STRINGS_COPY = InfoPlist.strings

XCODE_UPDATE = ./build/update_xcode_versions.pl
PLIST_UPDATE = ./build/update_plist_versions.pl
STRINGS_UPDATE = ./build/update_strings_versions.pl


build:
	( cd kext; make build )

dist:
	sudo rm -fr $(DSTROOT)
	mkdir -p $(STARTUPITEMS)
	mkdir -p $(PREFPANES)
	svn export $(SVN_TRUNK)/dist/DoubleCommand $(DC_FOLDER)
	svn export $(SVN_TRUNK)/docs $(DC_FOLDER)/docs
	cp -Rp kext/build/$(BUILD_CONFIG)/DoubleCommand.kext $(DC_FOLDER)
	cp -Rp prefpane/build/DoubleCommandPreferences.prefPane $(PREFPANES)
	sudo chown -R root:wheel $(DSTROOT)/Library
	sudo chmod -R 775 $(DSTROOT)/Library
	sudo chmod -R 555 $(PREFPANES)/DoubleCommandPreferences.prefPane
	sudo chmod 444 $(PREFPANES)/DoubleCommandPreferences.prefPane/Contents/Info.plist
	sudo chmod 444 $(PREFPANES)/DoubleCommandPreferences.prefPane/Contents/Resources/DoubleCommand.tiff
	sudo chmod 444 $(PREFPANES)/DoubleCommandPreferences.prefPane/Contents/Resources/NSAuthenticationLock.tiff
	sudo chmod 444 $(PREFPANES)/DoubleCommandPreferences.prefPane/Contents/Resources/NSAuthenticationUnlock.tiff
	sudo chmod -R 555 $(DC_FOLDER)
	sudo chmod -R 444 $(DC_FOLDER)/DoubleCommand.pref
	sudo chmod -R 444 $(DC_FOLDER)/Licence.txt
	sudo chmod -R 444 $(DC_FOLDER)/StartupParameters.plist
	sudo chmod -R 444 $(DC_FOLDER)/doublecommand.css
	sudo chmod -R 444 $(DC_FOLDER)/download.html
	sudo chmod -R 444 $(DC_FOLDER)/index.html
	sudo chmod -R 444 $(DC_FOLDER)/install.html
	sudo chmod -R 444 $(DC_FOLDER)/valid-xhtml10.gif
	#sudo chmod -R 444 $(DC_FOLDER)/valid-xhtml10.png
	cd $(DC_FOLDER)
	#echo "Correcting permissions for DoubleCommand..."
	sudo chown -R root:wheel ./DoubleCommand.kext
	sudo chmod -R 755 ./DoubleCommand.kext
	sudo chmod 644 ./DoubleCommand.kext/Contents/Info.plist
	sudo chmod 644 ./DoubleCommand.kext/Contents/MacOS/DoubleCommand
	sudo chmod 644 ./DoubleCommand.kext/Contents/Resources/Info.plist
	sudo chmod 644 ./DoubleCommand.kext/Contents/Resources/English.lproj/InfoPlist.strings


package:
	rm -rf build/DoubleCommand$(VERSION)\_$(OS).pkg
	$(packagemaker) -build \
	  -p $(shell pwd)/build/DoubleCommand$(VERSION)\_$(OS).pkg \
	  -f $(shell pwd)/$(DSTROOT) \
	  -r $(shell pwd)/package/resources; echo 
	@echo Created package build/DoubleCommand$(VERSION)\_$(OS).pkg


old_package:
	mkdir -p $(TMP)
	svn export $(SVN_TRUNK)/dist/DoubleCommand $(TMP)/DoubleCommand
	cp -Rp kext/build/$(BUILD_CONFIG)/DoubleCommand.kext $(TMP)/DoubleCommand
	svn export --force $(SVN_TRUNK)/docs $(TMP)/DoubleCommand
	cd $(TMP) && \
	gnutar -c -z -f DoubleCommand$(VERSION)_$(OS).tar.gz DoubleCommand && \
	cp DoubleCommand$(VERSION)_$(OS).tar.gz ../../../Release
	#scp DoubleCommand$(VERSION)\_$(OS).tar.gz mbaltaks@shell.sourceforge.net:/home/groups/d/do/doublecommand/
	rm -fr $(TMP)


uninstall:
	kextunload -b com.baltaks.driver.DoubleCommand
	rm -rf /Library/PreferencePanes/DoubleCommandPreferences.prefPane
	rm -rf /Library/StartupItems/DoubleCommand


clean:
	( cd kext; make clean )
	( cd package; make clean )

versions: $(XCODE_PROJECT) $(INFO_PLIST) $(EN_STRINGS) $(VERSION)
	$(XCODE_UPDATE) $(VERSION) < $(XCODE_PROJECT) > $(XCODE_PROJECT_COPY)
	$(PLIST_UPDATE) $(VERSION) < $(INFO_PLIST) > $(INFO_PLIST_COPY)
	$(STRINGS_UPDATE) $(VERSION) < $(EN_STRINGS) > $(EN_STRINGS_COPY)
	cp -f $(XCODE_PROJECT_COPY) $(XCODE_PROJECT)
	cp -f $(INFO_PLIST_COPY) $(INFO_PLIST)
	cp -f $(EN_STRINGS_COPY) $(EN_STRINGS)
	rm $(XCODE_PROJECT_COPY)
	rm $(INFO_PLIST_COPY)
	rm $(EN_STRINGS_COPY)
