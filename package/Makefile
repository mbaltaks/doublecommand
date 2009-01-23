.PHONY: build versions pkg_root

# Project global make settings go in this file
include ../Make.config

VERSION_FILE = ../dcversion


# commands
chown=/usr/sbin/chown
chmod=/bin/chmod
packagemaker=/Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker


# products
prefpane=../prefpane/build/$(BUILD_CONFIG)/DoubleCommandPreferences.prefPane
kext=../kext/build/$(BUILD_CONFIG)/DoubleCommand.kext

# places
pkg_root=../build/pkg_root
startup=../dist/DoubleCommand
dcfolder=/Library/StartupItems/DoubleCommand
tmp=../build


# Files containing the version number
INFO_PLIST = Info.plist

INFO_PLIST_COPY = $(tmp)/Info.plist

PLIST_UPDATE = ../update_plist_versions.pl


build: pkg_root resources versions
	rm -fr $(tmp)/resources
	mkdir $(tmp)/resources
	cp resources/pre* resources/post* $(tmp)/resources
	rm -rf "$(tmp)/DoubleCommand-`cat $(VERSION_FILE)`.pkg"
	-$(packagemaker) -build \
	  -p "$(shell pwd)/$(tmp)/DoubleCommand-`cat $(VERSION_FILE)`.pkg" \
	  -f "$(shell pwd)/$(pkg_root)" \
	  -ds \
	  -r "$(shell pwd)/$(tmp)/resources" \
	  -i "$(shell pwd)/Info.plist" \
	  -d "$(shell pwd)/Description.plist"
	@echo Created package $(tmp)/DoubleCommand-`cat $(VERSION_FILE)`.pkg


versions: $(INFO_PLIST) $(VERSION_FILE)
	$(PLIST_UPDATE) $(VERSION_FILE) < $(INFO_PLIST) > $(INFO_PLIST_COPY)
	cp -f $(INFO_PLIST_COPY) $(INFO_PLIST)
	rm $(INFO_PLIST_COPY)


pkg_root: $(prefpane) $(kext) $(startup)
	sudo rm -fr "$(pkg_root)"
	mkdir -p "$(pkg_root)/Library/StartupItems"
	cp -R "$(startup)" "$(pkg_root)/Library/StartupItems"
	rm -fr "$(pkg_root)$(dcfolder)/.svn"
	cp -R "$(kext)" "$(pkg_root)$(dcfolder)"
	sudo $(chmod) -R go-w "$(pkg_root)$(dcfolder)/DoubleCommand.kext"
	sudo $(chown) -R root:wheel "$(pkg_root)$(dcfolder)/DoubleCommand.kext"
	mkdir -p "$(pkg_root)/Library/PreferencePanes"
	cp -R "$(prefpane)" "$(pkg_root)/Library/PreferencePanes"
