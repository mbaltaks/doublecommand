/*
 *  MBHIDHack.h
 *  DoubleCommand
 *
 *  Created by Michael Baltaks on Tuesday April 30 2002.
 *
 * $Id$
 *
 *  Last modified 2003-02-06.
 *  Copyright (c) 2002 Michael Baltaks. All rights reserved.
 *  Thanks to Shane Celis <shane@terraspring.com> for some
 *  constants.
 *
 */


#define COMMAND_FLAG 0x100000
#define CONTROL_FLAG 0x40000
#define CAPSLOCK_FLAG 0x10000
#define OPTION_FLAG 0x80000
#define FN_FLAG 0x800000
#define SHIFT_FLAG 0x20000

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

#define KEY_DOWN 10
#define KEY_UP 11
#define KEY_MODIFY 12

#define INTERNAL_KEYBOARD 195

#define ENTER_TO_COMMAND 1
#define ENTER_TO_CONTROL 2
#define ENTER_TO_OPTION 4
#define ENTER_TO_FN 8
#define COMMAND_TO_OPTION 16
#define COMMAND_TO_CONTROL 32
#define OPTION_TO_COMMAND 64
#define OPTION_TO_FN 128
#define CONTROL_TO_COMMAND 256
#define SWAP_CONTROL_AND_OPTION 512
#define CONTROL_TO_FN 1024
#define CAPSLOCK_TO_CONTROL 2048
#define FN_TO_CONTROL 4096
#define REVERSE_NUMPAD_DOT_AND_SHIFT_NUMPAD_DOT 8192
#define SHIFT_DELETE_TO_FORWARD_DELETE 16384
#define DISABLE_COMMAND_AND_OPTION 32768
#define PC_STYLE_HOME_AND_END 65536
#define BACKSLASH_TO_FORWARD_DELETE 131072
