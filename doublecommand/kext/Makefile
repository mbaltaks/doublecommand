# Project global make settings go in this file
include ../Make.config


# kext only settings go right here
INFO_PLIST = Info.plist
XCODE_PROJECT = DoubleCommand.xcodeproj/project.pbxproj
CC = /usr/bin/xcodebuild
BUILD_FLAGS = -configuration $(BUILD_CONFIG)
EN_STRINGS = English.lproj/InfoPlist.strings
HEADERS = Boundary.h DoubleCommand.h MBDCC.h MBHIDHack.h
SOURCES = DoubleCommand.cpp Substitute.cpp


all: build

build: $(INFO_PLIST) $(XCODE_PROJECT) $(EN_STRINGS) $(HEADERS) $(SOURCES)
	$(CC) $(BUILD_FLAGS) -project ($XCODE_PROJECT)

clean:
	$(CC) $(BUILD_FLAGS) -project ($XCODE_PROJECT) clean
