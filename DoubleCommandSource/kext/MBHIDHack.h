/*
 * $Id$
 *
 * MBHIDHack.h
 * DoubleCommand
 *
 * Created by Michael Baltaks on Tuesday April 30 2002.
 *
 * Last modified 2003-02-06.
 * Copyright (c) 2002 Michael Baltaks. All rights reserved.
 * Thanks to Shane Celis <shane@terraspring.com> for some
 * constants.
 */


// define MB_DEBUG to enable output of keycodes
//#define MB_DEBUG

// these should be defined for a 10.2 build, but
// not for 10.3 or later
//#define IOHIDSystem_AVAILABLE
//#define JAGUAR_FLAGS


#define COMMAND_FLAG 0x100000
#define CONTROL_FLAG 0x40000
#define CAPSLOCK_FLAG 0x10000
#define OPTION_FLAG 0x80000
#define FN_FLAG 0x800000
#ifdef JAGUAR_FLAGS
	#define SHIFT_FLAG 0x20000
#else
	#define SHIFT_FLAG 0x20002
#endif

#define SPACE_KEY 49
#define CONTROL_KEY 59
#define CAPSLOCK_KEY 57
// special caps key for TiBook (and probably other models)
#define SPECIAL_CAPSLOCK_KEY 127
#define ENTER_KEY 52
#define COMMAND_KEY 55
#define OPTION_KEY 58
#define FN_KEY 63
#define DELETE_KEY 51
#define FORWARD_DELETE 117
#define NUMPAD_DOT 65
#define HOME_KEY 115
#define END_KEY 119
#define LEFT_ARROW_KEY 123
#define RIGHT_ARROW_KEY 124
#define BACKSLASH_KEY 42
#define F1		122
#define F2		120
#define F3		99
#define F4		118
#define F5		96
#define F6		97
#define F7		98
#define F8		100
#define F9		101
#define F10		109
#define F11		103
#define F12		111
#define BRIGHTNESS_DOWN		10
#define BRIGHTNESS_UP		9
#define VOLUME_MUTE		8
#define VOLUME_DOWN		7
#define VOLUME_UP		6
#define SPECIAL_KEY		127
#define BRIGHTNESS_DOWN_FLAVOUR		3
#define BRIGHTNESS_UP_FLAVOUR		2
#define VOLUME_MUTE_FLAVOUR		7
#define VOLUME_DOWN_FLAVOUR		1
#define VOLUME_UP_FLAVOUR		0
#define NUMLOCK_FLAVOUR		10
#define F7a_FLAVOUR		15
#define F8a_FLAVOUR		23
#define F9a_FLAVOUR		22
#define F10a_FLAVOUR		21
#define EJECT_FLAVOUR		14
#define F7a		12
#define F8a		13
#define F9a		14
#define F10a	15
#define F11a	103
#define EJECT	11

#define KEY_DOWN 10
#define KEY_UP 11
#define KEY_MODIFY 12

#define POWERBOOKG3_KEYBOARD 195
#define TIBOOK_KEYBOARD 196
#define IBOOK_KEYBOARD 202
#define APPLE_PRO_2003_KEYBOARD 34

#define ENTER_TO_COMMAND						1
#define ENTER_TO_CONTROL						2
#define ENTER_TO_OPTION							4
#define ENTER_TO_FN								8
#define COMMAND_TO_OPTION						16
#define COMMAND_TO_CONTROL						32
#define OPTION_TO_COMMAND						64
#define OPTION_TO_FN							128

#define CONTROL_TO_COMMAND						256
#define SWAP_CONTROL_AND_OPTION					512
#define CONTROL_TO_FN							1024
#define CAPSLOCK_TO_CONTROL						2048
#define FN_TO_CONTROL							4096
#define REVERSE_NUMPAD_DOT_AND_SHIFT_NUMPAD_DOT	8192
#define SHIFT_DELETE_TO_FORWARD_DELETE			16384
#define DISABLE_COMMAND_AND_OPTION				32768

#define PC_STYLE_HOME_AND_END					65536
#define BACKSLASH_TO_FORWARD_DELETE				131072
#define SWAP_FUNCTION_KEYS						262144
#define CAPSLOCK_DISABLED						524288
//#define 1048576
//#define 2097152
//#define 4194304
//#define 8388608

//#define 16777216
//#define 33554432
//#define 67108864
//#define 134217728
//#define 268435456
//#define 536870912
//#define 1073741824
//#define 2147483648
#define NEED_NEW_INT_VAR 4294967296
