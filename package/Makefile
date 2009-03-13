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
tmp=../build
pkg_root=$(tmp)/pkg_root
lib=$(pkg_root)/Library
startupitems=$(lib)/StartupItems
prefpanes=$(lib)/PreferencePanes
dcfolder=$(startupitems)/DoubleCommand
startup=../dist/DoubleCommand


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
	mkdir -p "$(startupitems)"
	cp -R "$(startup)" "$(startupitems)"
	rm -fr "$(dcfolder)/.svn"
	cp -R "$(kext)" "$(dcfolder)"
	sudo $(chmod) -R go-w "$(dcfolder)/DoubleCommand.kext"
	sudo $(chown) -R root:wheel "$(dcfolder)/DoubleCommand.kext"
	mkdir -p "$(prefpanes)"
	cp -R "$(prefpane)" "$(prefpanes)"
	sudo $(chmod) g+w "$(lib)"
	sudo $(chmod) +t "$(lib)"
	sudo $(chown) -R root:admin "$(lib)"
	sudo $(chown) -R root:wheel "$(startupitems)"
	sudo $(chown) -R root:wheel "$(prefpanes)"
