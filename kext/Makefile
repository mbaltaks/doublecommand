.PHONY: build versions clean unload load

# Project global make settings go in this file
include ../Make.config

VERSION_FILE = ../dcversion


# kext only settings go right here
XCODE_PROJECT = DoubleCommand.xcodeproj
CC = /usr/bin/xcodebuild
BUILD_FLAGS = -configuration $(BUILD_CONFIG)
HEADERS = Boundary.h DoubleCommand.h MBDCC.h MBHIDHack.h
SOURCES = DoubleCommand.cpp Substitute.cpp
# Files containing the version number
XCODE_PROJECT_FILE = $(XCODE_PROJECT)/project.pbxproj
INFO_PLIST = Info.plist
EN_STRINGS = English.lproj/InfoPlist.strings

XCODE_PROJECT_COPY = ../build/project.pbxproj
INFO_PLIST_COPY = ../build/Info.plist
EN_STRINGS_COPY = ../build/InfoPlist.strings

XCODE_UPDATE = ../update_xcode_versions.pl
PLIST_UPDATE = ../update_plist_versions.pl
STRINGS_UPDATE = ../update_strings_versions.pl

kextload=/sbin/kextload
kextunload=/sbin/kextunload
kextstat=/usr/sbin/kextstat
sudo=/usr/bin/sudo
chown=/usr/sbin/chown
kext=build/${BUILD_CONFIG}/DoubleCommand.kext
kext_id="com.baltaks.driver.DoubleCommand"

build:
	$(CC) $(BUILD_FLAGS) -project $(XCODE_PROJECT)

build_old: $(INFO_PLIST) $(XCODE_PROJECT) $(EN_STRINGS) $(HEADERS) $(SOURCES)
	$(CC) $(BUILD_FLAGS) -project $(XCODE_PROJECT)

clean:
	$(CC) $(BUILD_FLAGS) -project $(XCODE_PROJECT) clean

versions: $(XCODE_PROJECT_FILE) $(INFO_PLIST) $(EN_STRINGS) $(VERSION_FILE)
	$(XCODE_UPDATE) $(VERSION_FILE) < $(XCODE_PROJECT_FILE) > $(XCODE_PROJECT_COPY)
	$(PLIST_UPDATE) $(VERSION_FILE) < $(INFO_PLIST) > $(INFO_PLIST_COPY)
	$(STRINGS_UPDATE) $(VERSION_FILE) < $(EN_STRINGS) > $(EN_STRINGS_COPY)
	cp -f $(XCODE_PROJECT_COPY) $(XCODE_PROJECT_FILE)
	cp -f $(INFO_PLIST_COPY) $(INFO_PLIST)
	cp -f $(EN_STRINGS_COPY) $(EN_STRINGS)
	rm $(XCODE_PROJECT_COPY)
	rm $(INFO_PLIST_COPY)
	rm $(EN_STRINGS_COPY)

unload:
	-${sudo} ${kextunload} -b ${kext_id}

load: build unload
	${sudo} ${chown} -R root:wheel ${kext}
	${sudo} ${kextload} ${kext}
