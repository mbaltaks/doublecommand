/*
 * $Id$
 *
 * Name: MBHIDHack.cpp
 * Project: DoubleCommand
 * Author: Michael Baltaks <mbaltaks@mac.com>
 * Creation Date: 2002-4-26
 * Last Modified: 2003-02-06
 * Originally based on iJect by Christian Starkjohann <cs@obdev.at> 
 * Tabsize: 4
 * Copyright: GNU General Public License version 2.0
 */

//#define MB_DEBUG

#ifdef __cplusplus
    extern "C"
	{
#endif

#include <mach/mach_types.h>
#include <sys/systm.h>

extern int	MBHidInit(void);
extern int	MBHidExit(void);

#ifdef __cplusplus
    }
#endif

#include "MBHIDHack.h"
#include <IOKit/system.h>
#include <IOKit/assert.h>
#include <IOKit/hidsystem/IOHIDSystem.h>

static void		*oldVtable = NULL;
static void		*myVtable = NULL;

// int variable to set the configuration of DoubleCommand
int dcConfig = 0;


//----------------------------------------------------------------------------
class MBHIDHack : public IOHIDSystem
{
/* we must not declare anything which is not in our superclass
 * since we want to pose as our superclass.
 */
public:
  virtual void keyboardEvent(unsigned   eventType,
      /* flags */            unsigned   flags,
      /* keyCode */          unsigned   key,
      /* charCode */         unsigned   charCode,
      /* charSet */          unsigned   charSet,
      /* originalCharCode */ unsigned   origCharCode,
      /* originalCharSet */  unsigned   origCharSet,
      /* keyboardType */     unsigned   keyboardType,
      /* repeat */           bool       repeat,
      /* atTime */           AbsoluteTime ts);
  virtual void keyboardSpecialEvent(unsigned   eventType,
				/* flags */        unsigned   flags,
				/* keyCode  */     unsigned   key,
				/* specialty */    unsigned   flavor,
				/* guid */         UInt64     guid,
				/* repeat */       bool       repeat,
				/* atTime */       AbsoluteTime ts);
};


//----------------------------------------------------------------------------
// MBHidInit() - replace the real IOHIDSystem with our imposter.
//----------------------------------------------------------------------------
int
MBHidInit(void)
{
	IOHIDSystem	*p;
	MBHIDHack	*sub;

	if(oldVtable != NULL)
	{
		printf("Module DoubleCommand already loaded!\n");
		return 1;
	}
	if(myVtable == NULL)
	{
		sub = new MBHIDHack();
		myVtable = *(void **)sub;
		//sub->free();
	}
    p = IOHIDSystem::instance();
    oldVtable = *(void **)p;
    *(void **)p = myVtable;
    return 0;
}


//----------------------------------------------------------------------------
// MBHidExit() - replace our imposter with the real IOHIDSystem.
//----------------------------------------------------------------------------
int
MBHidExit(void)
{
	IOHIDSystem	*p;

    if(oldVtable != NULL)
	{
        p = IOHIDSystem::instance();
		if(*(void **)p != myVtable)
		{
			printf("Sorry, cannot unload DoubleCommand!\n");
			return 1;
		}
        *(void **)p = oldVtable;
        oldVtable = NULL;
    }
	return 0;
}


// key remapping stuff down here.
unsigned char setCommandFlag = 0;
unsigned char setControlFlag = 0;
unsigned char setOptionFlag = 0;
unsigned char setfnFlag = 0;
//unsigned char setCapslockFlag = 0;
unsigned char commandHeldDown = 0;
unsigned char optionHeldDown = 0;
unsigned char controlHeldDown = 0;
unsigned char fnHeldDown = 0;
unsigned char capslockHeldDown = 1;
unsigned char capslockOn = 0;
unsigned char inFnMode = 0;
unsigned char unsetCommandFlag = 0;
unsigned char unsetOptionFlag = 0;
unsigned char unsetControlFlag = 0;
unsigned char unsetfnFlag = 0;
unsigned char unsetCapslockFlag = 0;

unsigned char keepSpecialEvent = 1;
unsigned char keepKeyboardEvent = 1;

unsigned lastKeyboardType = 202;


//----------------------------------------------------------------------------
void MBHIDHack::keyboardEvent(unsigned   eventType,
      /* flags */            unsigned   flags,
      /* keyCode */          unsigned   key,
      /* charCode */         unsigned   charCode,
      /* charSet */          unsigned   charSet,
      /* originalCharCode */ unsigned   origCharCode,
      /* originalCharSet */  unsigned   origCharSet,
      /* keyboardType */     unsigned   keyboardType,
      /* repeat */           bool       repeat,
      /* atTime */           AbsoluteTime ts)
{
	unsigned flavor = 0;
	UInt64 guid = 0;
#ifdef MB_DEBUG
	printf("caught  hid event type %d flags 0x%x key %d charCode %d charSet %d origCharCode %d origCharSet %d kbdType %d\n", eventType, flags, key, charCode, charSet, origCharCode, origCharSet, keyboardType);
#endif

if (dcConfig != 0)
{
	lastKeyboardType = keyboardType;

	if( (dcConfig & CAPSLOCK_DISABLED) && (flags & CAPSLOCK_FLAG) )
	{
		flags ^= CAPSLOCK_FLAG;
	}

	switch (key)
	{
		case ENTER_KEY: // begin enter key
			if (dcConfig & ENTER_TO_COMMAND)
			{
				if (eventType == KEY_DOWN)
				{
					setCommandFlag = 1;
					key = COMMAND_KEY; // we don't want any enter key stuff to get through
					eventType = KEY_MODIFY;
					//charCode = 111;
					//flags |= COMMAND_FLAG;
				}
				else if (eventType == KEY_UP)
				{
					setCommandFlag = 0;
					key = COMMAND_KEY; // we don't want any enter key stuff to get through
					eventType = KEY_MODIFY;
				}
			}
			else if (dcConfig & ENTER_TO_CONTROL)
			{
				if (eventType == KEY_DOWN)
				{
					setControlFlag = 1;
					key = CONTROL_KEY; // we don't want any enter key stuff to get through
					eventType = KEY_MODIFY;
				}
				else if (eventType == KEY_UP)
				{
					setControlFlag = 0;
					key = CONTROL_KEY; // we don't want any enter key stuff to get through
					eventType = KEY_MODIFY;
				}
			}
			else if (dcConfig & ENTER_TO_OPTION)
			{
				if (eventType == KEY_DOWN)
				{
					setOptionFlag = 1;
					key = OPTION_KEY; // we don't want any enter key stuff to get through
					eventType = KEY_MODIFY;
				}
				else if (eventType == KEY_UP)
				{
					setOptionFlag = 0;
					key = OPTION_KEY; // we don't want any enter key stuff to get through
					eventType = KEY_MODIFY;
				}
			}
			else if (dcConfig & ENTER_TO_FN)
			{
				if (eventType == KEY_DOWN)
				{
					setfnFlag = 1;
					key = FN_KEY; // we don't want any enter key stuff to get through
					eventType = KEY_MODIFY;
					flags |= FN_FLAG;
					inFnMode = 1;
				}
				else if (eventType == KEY_UP)
				{
					setfnFlag = 0;
					key = FN_KEY; // we don't want any enter key stuff to get through
					eventType = KEY_MODIFY;
					inFnMode = 0;
				}
			}
		break; // end enter key

		case COMMAND_KEY: // begin command key
			if (dcConfig & COMMAND_TO_OPTION)
			{
				if (commandHeldDown) // this event is a key up
				{
					commandHeldDown = 0;
					setOptionFlag = 0;
					unsetCommandFlag = 0;
				}
				else // this event is a key down
				{
					commandHeldDown = 1;
					setOptionFlag = 1;
					unsetCommandFlag = 1;
				}
				key = OPTION_KEY;
			}
			else if (dcConfig & COMMAND_TO_CONTROL)
			{
				if (commandHeldDown) // this event is a key up
				{
					commandHeldDown = 0;
					setControlFlag = 0;
					unsetCommandFlag = 0;
				}
				else // this event is a key down
				{
					commandHeldDown = 1;
					setControlFlag = 1;
					unsetCommandFlag = 1;
				}
				key = CONTROL_KEY;
			}
			else if (dcConfig & DISABLE_COMMAND_AND_OPTION)
			{
				if (commandHeldDown) // this event is a key up
				{
					commandHeldDown = 0;
					unsetCommandFlag = 0;
				}
				else // this event is a key down
				{
					commandHeldDown = 1;
					unsetCommandFlag = 1;
				}
			}
		break; // end command key

		case OPTION_KEY: // begin option key
			if (dcConfig & OPTION_TO_COMMAND)
			{
				if (optionHeldDown) // this event is a key up
				{
					optionHeldDown = 0;
					setCommandFlag = 0;
					unsetOptionFlag = 0;
				}
				else // this event is a key down
				{
					optionHeldDown = 1;
					setCommandFlag = 1;
					unsetOptionFlag = 1;
				}
				key = COMMAND_KEY;
			}
			else if (dcConfig & OPTION_TO_FN)
			{
				if (optionHeldDown) // this event is a key up
				{
					optionHeldDown = 0;
					setfnFlag = 0;
					unsetOptionFlag = 0;
				}
				else // this event is a key down
				{
					optionHeldDown = 1;
					setfnFlag = 1;
					unsetOptionFlag = 1;
				}
				key = FN_KEY;
			}
			else if (dcConfig & DISABLE_COMMAND_AND_OPTION)
			{
				if (optionHeldDown) // this event is a key up
				{
					optionHeldDown = 0;
					unsetOptionFlag = 0;
				}
				else // this event is a key down
				{
					optionHeldDown = 1;
					unsetOptionFlag = 1;
				}
			}
			else if (dcConfig & SWAP_CONTROL_AND_OPTION) // control <-> option
			{
				if (optionHeldDown) // this event is a key up
				{
					optionHeldDown = 0;
					setControlFlag = 0;
					if (!controlHeldDown)
					{
						unsetOptionFlag = 0;
					}
				}
				else // this event is a key down
				{
					optionHeldDown = 1;
					setControlFlag = 1;
					if (!controlHeldDown)
					{
						unsetOptionFlag = 1;
					}
				}
				key = CONTROL_KEY;
			}
		break; // end option key

		case CONTROL_KEY: // begin control key
			if (dcConfig & CONTROL_TO_COMMAND)
			{
				if (controlHeldDown) // this event is a key up
				{
					controlHeldDown = 0;
					setCommandFlag = 0;
					unsetControlFlag = 0;
				}
				else // this event is a key down
				{
					controlHeldDown = 1;
					setCommandFlag = 1;
					unsetControlFlag = 1;
				}
				key = COMMAND_KEY;
			}
			else if (dcConfig & SWAP_CONTROL_AND_OPTION) // control <-> option
			{
				if (controlHeldDown) // this event is a key up
				{
					controlHeldDown = 0;
					setOptionFlag = 0;
					if (!optionHeldDown)
					{
						unsetControlFlag = 0;
					}
				}
				else // this event is a key down
				{
					controlHeldDown = 1;
					setOptionFlag = 1;
					if (!optionHeldDown)
					{
						unsetControlFlag = 1;
					}
				}
				key = OPTION_KEY;
			}
			else if (dcConfig & CONTROL_TO_FN)
			{
				if (controlHeldDown) // this event is a key up
				{
					controlHeldDown = 0;
					setfnFlag = 0;
					unsetControlFlag = 0;
				}
				else // this event is a key down
				{
					controlHeldDown = 1;
					setfnFlag = 1;
					unsetControlFlag = 1;
				}
				key = FN_KEY;
			}
		break; // end control key

		case FN_KEY: // begin fn key
			if (flags & FN_FLAG)
			{
				fnHeldDown = 1;
			}
			else
			{
				fnHeldDown = 0;
				unsetfnFlag = 0;
			}
			if (dcConfig & FN_TO_CONTROL)
			{
				if (fnHeldDown) // this event is a key up
				{
					//fnHeldDown = 0;
					setControlFlag = 0;
					unsetfnFlag = 0;
				}
				else // this event is a key down
				{
					//fnHeldDown = 1;
					setControlFlag = 1;
					unsetfnFlag = 1;
				}
				key = CONTROL_KEY;
			}
		break; // end fn key

		case DELETE_KEY: // begin delete key
			// Make Shift + Delete send a Forward Delete key
			// This doesn't conflict with anything I know of and is handy
			if (dcConfig & SHIFT_DELETE_TO_FORWARD_DELETE)
			{
				//if (eventType == KEY_DOWN || eventType == KEY_UP)
				//{
					if (flags == SHIFT_FLAG) // with _only_ shift held as well
					{
						key = FORWARD_DELETE;
						//flags ^= 0x20000;
						flags = FN_FLAG;
						charCode = 45;
						charSet = 254;
						origCharCode = 45;
						origCharSet = 254;
					}
				//}
			}
		break; // end delete key

		// begin supplied by Giel Scharff <mgsch@mac.com>
		case NUMPAD_DOT: // begin numpad dot
			if (dcConfig & REVERSE_NUMPAD_DOT_AND_SHIFT_NUMPAD_DOT)
			{
				if (eventType == KEY_DOWN && (flags == 0x200000)) // key down without shift held as well
				{
					//key = 65;
					flags = 0x220000;
				}
				else if (eventType == KEY_DOWN && (flags == 0x220000)) // key down with shift held as well
				{
					//key = 65;
					flags = 0x200000;
				}
			}
		break; // end numpad dot
		// end supplied by Giel Scharff <mgsch@mac.com>

		case CAPSLOCK_KEY: // begin capslock key
			if(dcConfig & CAPSLOCK_TO_CONTROL)
			{
				unsetCapslockFlag = 1;
				key = CONTROL_KEY;
				//capslockHeldDown = 1;
				setControlFlag = 1;
				if(flags & CAPSLOCK_FLAG)
				{
					//flags ^= CAPSLOCK_FLAG;
					//flags |= CONTROL_FLAG;
					//capslockOn = 1;
				}
				else if(keyboardType == POWERBOOKG3_KEYBOARD || keyboardType == IBOOK_KEYBOARD)
				{
					//capslockHeldDown = 0;
					setControlFlag = 0;
				}
				else
				{
					//capslockOn = 0;
					//capslockHeldDown = 0;
				}
			}
		break; // end capslock key

		case HOME_KEY: // begin home key
			if(dcConfig & PC_STYLE_HOME_AND_END)
			{
				key = LEFT_ARROW_KEY;
				if (eventType == KEY_DOWN)
				{
					setCommandFlag = 1;
				}
				else if (eventType == KEY_UP)
				{
					setCommandFlag = 0;
				}
			}
		break; // end home key

		case END_KEY: // begin end key
			if(dcConfig & PC_STYLE_HOME_AND_END)
			{
				key = RIGHT_ARROW_KEY;
				if (eventType == KEY_DOWN)
				{
					setCommandFlag = 1;
				}
				else if (eventType == KEY_UP)
				{
					setCommandFlag = 0;
				}
			}
		break; // end end key

		case BACKSLASH_KEY: // begin backslash key
			if(dcConfig & BACKSLASH_TO_FORWARD_DELETE)
			{
				key = FORWARD_DELETE;
			}
		break; // end backslash key

		case F1: // begin F1 key
			if ( (dcConfig & SWAP_FUNCTION_KEYS) && (lastKeyboardType != APPLE_PRO_2003_KEYBOARD) )
			{
				key = BRIGHTNESS_DOWN;
				flavor = 3;
				keepKeyboardEvent = 0;
				unsetfnFlag = 1;
			}
		break; // end F1 key
		case F2: // begin F2 key
			if ( (dcConfig & SWAP_FUNCTION_KEYS) && (lastKeyboardType != APPLE_PRO_2003_KEYBOARD) )
			{
				key = BRIGHTNESS_UP;
				flavor = 2;
				keepKeyboardEvent = 0;
				unsetfnFlag = 1;
			}
		break; // end F2 key
		case F3: // begin F3 key
			if ( (dcConfig & SWAP_FUNCTION_KEYS) && (lastKeyboardType != APPLE_PRO_2003_KEYBOARD) )
			{
				key = VOLUME_MUTE;
				flavor = 7;
				keepKeyboardEvent = 0;
				unsetfnFlag = 1;
			}
		break; // end F3 key
		case F4: // begin F4 key
			if ( (dcConfig & SWAP_FUNCTION_KEYS) && (lastKeyboardType != APPLE_PRO_2003_KEYBOARD) )
			{
				key = VOLUME_DOWN;
				flavor = 1;
				keepKeyboardEvent = 0;
				unsetfnFlag = 1;
			}
		break; // end F4 key
		case F5: // begin F5 key
			if ( (dcConfig & SWAP_FUNCTION_KEYS) && (lastKeyboardType != APPLE_PRO_2003_KEYBOARD) )
			{
				key = VOLUME_UP;
				flavor = 0;
				keepKeyboardEvent = 0;
				unsetfnFlag = 1;
			}
		break; // end F5 key
		case F6: // begin F6 key
			if ( (dcConfig & SWAP_FUNCTION_KEYS) && (lastKeyboardType != APPLE_PRO_2003_KEYBOARD) )
			{
				key = SPECIAL_KEY;
				flavor = 10;
				keepKeyboardEvent = 0;
				unsetfnFlag = 1;
			}
		break; // end F6 key
		/* who wants these other keys? anyone?
		actually, we can probably do this if the fn flag is set - 
		that would mean a 17" PBG4 keyboard */
		case F7: // begin F7 key
			if (
				(dcConfig & SWAP_FUNCTION_KEYS) && 
				(lastKeyboardType != APPLE_PRO_2003_KEYBOARD) &&
				(flags & FN_FLAG)
				)
			{
				key = F7a;
				flavor = 15;
				keepKeyboardEvent = 0;
				unsetfnFlag = 1;
			}
		break; // end F7 key
		case F8: // begin F8 key
			if (
				(dcConfig & SWAP_FUNCTION_KEYS) && 
				(lastKeyboardType != APPLE_PRO_2003_KEYBOARD) &&
				(flags & FN_FLAG)
				)
			{
				key = F8a;
				flavor = 23;
				keepKeyboardEvent = 0;
				unsetfnFlag = 1;
			}
		break; // end F8 key
		case F9: // begin F9 key
			if (
				(dcConfig & SWAP_FUNCTION_KEYS) && 
				(lastKeyboardType != APPLE_PRO_2003_KEYBOARD) &&
				(flags & FN_FLAG)
				)
			{
				key = F9a;
				flavor = 22;
				keepKeyboardEvent = 0;
				unsetfnFlag = 1;
			}
		break; // end F9 key
		case F10: // begin F10 key
			if (
				(dcConfig & SWAP_FUNCTION_KEYS) && 
				(lastKeyboardType != APPLE_PRO_2003_KEYBOARD) &&
				(flags & FN_FLAG)
				)
			{
				key = F10a;
				flavor = 21;
				keepKeyboardEvent = 0;
				unsetfnFlag = 1;
			}
		break; // end F10 key

	} // end switch (key)

	// begin supplied by Giel Scharff <mgsch@mac.com>
	if (inFnMode)
	{   //up -> PgUp
		if ((key == 126) && (flags == 0x200000) && (charCode == 173) && (charSet == 1) &&
	  (origCharCode == 173) && (origCharSet == 1))
		{
			key = 116; flags = 0x800000; charCode =  48; charSet = 254; origCharCode =  48;  origCharSet = 254;
		}
		//down -> PgDown
		else if ((key == 125) && (flags == 0x200000) && (charCode == 175) && (charSet == 1) &&
		   (origCharCode == 175) && (origCharSet == 1))
		{
			key = 121; flags = 0x800000; charCode =  49; charSet = 254; origCharCode =  49;  origCharSet = 254;
		}
		//left -> Home
		else if ((key == 123) && (flags == 0x200000) && (charCode == 172) && (charSet == 1) &&
		   (origCharCode == 172) && (origCharSet == 1))
		{
			key = 115; flags = 0x800000; charCode =  46; charSet = 254; origCharCode =  46;  origCharSet = 254;
		}
		//right -> End
		else if ((key == 124) && (flags == 0x200000) && (charCode == 174) && (charSet == 1) &&
		   (origCharCode == 174) && (origCharSet == 1))
		{
			key = 119; flags = 0x800000; charCode =  47; charSet = 254; origCharCode =  46;  origCharSet = 254;
		}
	}
	// end supplied by Giel Scharff <mgsch@mac.com>

	/*
		what we need here is to have two vars one to OR
		with flags and one to XOR - each part can add or
		remove its bit to that var.
	*/
	if (unsetCommandFlag && (flags & COMMAND_FLAG) )
	{
		flags ^= COMMAND_FLAG;
	}
	if (unsetOptionFlag && (flags & OPTION_FLAG) )
	{
		flags ^= OPTION_FLAG;
	}
	if (unsetControlFlag && (flags & CONTROL_FLAG) )
	{
		flags ^= CONTROL_FLAG;
	}
	if (unsetfnFlag && (flags & FN_FLAG) )
	{
		flags ^= FN_FLAG;
	}
	if (unsetCapslockFlag && (flags & CAPSLOCK_FLAG) )
	{
		flags ^= CAPSLOCK_FLAG;
	}
	if (setCommandFlag)
	{
		flags |= COMMAND_FLAG;
	}
	if (setControlFlag)
	{
		flags |= CONTROL_FLAG;
	}
	if (setOptionFlag)
	{
		flags |= OPTION_FLAG;
	}
	if (setfnFlag)
	{
		flags |= FN_FLAG;
	}
//	if (setCapslockFlag)
//	{
//		flags |= CAPSLOCK_FLAG;
//	}
} // end if dcConfig != 0

if(keepKeyboardEvent)
{
#ifdef MB_DEBUG
	printf("sending hid event type %d flags 0x%x key %d charCode %d charSet %d origCharCode %d origCharSet %d kbdType %d\n", eventType, flags, key, charCode, charSet, origCharCode, origCharSet, keyboardType);
#endif
    IOHIDSystem::keyboardEvent(eventType, flags, key, charCode, charSet, origCharCode, origCharSet, keyboardType, repeat, ts);
}
else
{
#ifdef MB_DEBUG
	printf("CHANGE sending special event type %d flags 0x%x key %d flavor %d\n", eventType, flags, key, flavor);
#endif
	keepKeyboardEvent = 1;
	IOHIDSystem::keyboardSpecialEvent(eventType, flags, key, flavor, guid, repeat, ts);
}

} // end MBHIDHack::keyboardEvent()


//----------------------------------------------------------------------------
void MBHIDHack::keyboardSpecialEvent(   unsigned   eventType,
                       /* flags */        unsigned   flags,
                       /* keyCode  */     unsigned   key,
                       /* specialty */    unsigned   flavor,
                       /* guid */         UInt64     guid,
                       /* repeat */       bool       repeat,
                       /* atTime */       AbsoluteTime ts)
{
	unsigned charCode = 0;
	unsigned charSet = 0;
	//unsigned keyboardType = lastKeyboardType;
#ifdef MB_DEBUG
	printf("caught  special event type %d flags 0x%x key %d flavor %d\n", eventType, flags, key, flavor);
#endif

if (dcConfig != 0)
{
	switch (key)
	{
		case BRIGHTNESS_DOWN: // begin F1 key
			if (dcConfig & SWAP_FUNCTION_KEYS)
			{
				keepSpecialEvent = 0;
				key = F1;
				//setfnFlag = 1;
				charCode = 32;
			}
		break; // end F1 key
		case BRIGHTNESS_UP: // begin F2 key
			if (dcConfig & SWAP_FUNCTION_KEYS)
			{
				keepSpecialEvent = 0;
				key = F2;
				charCode = 33;
			}
		break; // end F2 key
		case VOLUME_MUTE: // begin F3 key
			if ( (dcConfig & SWAP_FUNCTION_KEYS) && (lastKeyboardType != APPLE_PRO_2003_KEYBOARD) )
			{
				keepSpecialEvent = 0;
				key = F3;
				charCode = 34;
			}
		break; // end F3 key
		case VOLUME_DOWN: // begin F4 key
			if ( (dcConfig & SWAP_FUNCTION_KEYS) && (lastKeyboardType != APPLE_PRO_2003_KEYBOARD) )
			{
				keepSpecialEvent = 0;
				key = F4;
				charCode = 35;
			}
		break; // end F4 key
		case VOLUME_UP: // begin F5 key
			if ( (dcConfig & SWAP_FUNCTION_KEYS) && (lastKeyboardType != APPLE_PRO_2003_KEYBOARD) )
			{
				keepSpecialEvent = 0;
				key = F5;
				charCode = 36;
			}
		break; // end F5 key
		case SPECIAL_KEY: // begin F6 key
			if ( (dcConfig & SWAP_FUNCTION_KEYS) && (flavor == 10) )
			{
				keepSpecialEvent = 0;
				key = F6;
				charCode = 37;
			}
			else if( (dcConfig & CAPSLOCK_TO_CONTROL) && (flags & CAPSLOCK_FLAG) )
			{
				keepSpecialEvent = 0;
				key = CONTROL_KEY;
				charCode = 0;
				eventType = KEY_MODIFY;
				unsetCapslockFlag = 1;
				//flags ^= CAPSLOCK_FLAG;
				if (!capslockHeldDown)
				{
					capslockHeldDown = 1;
					setControlFlag = 1;
					//flags |= CONTROL_FLAG;
				}
				else
				{
					capslockHeldDown = 0;
					setControlFlag = 0;
				}
			}
		break; // end F6 key
		case F7a: // begin F7 key
			if (dcConfig & SWAP_FUNCTION_KEYS)
			{
				keepSpecialEvent = 0;
				key = F7;
				charCode = 38;
			}
		break; // end F7 key
		case F8a: // begin F8 key
			if (dcConfig & SWAP_FUNCTION_KEYS)
			{
				keepSpecialEvent = 0;
				key = F8;
				charCode = 39;
			}
		break; // end F8 key
		case F9a: // begin F9 key
			if (dcConfig & SWAP_FUNCTION_KEYS)
			{
				keepSpecialEvent = 0;
				key = F9;
				charCode = 40;
			}
		break; // end F9 key
		case F10a: // begin F10 key
			if (dcConfig & SWAP_FUNCTION_KEYS)
			{
				keepSpecialEvent = 0;
				key = F10;
				charCode = 41;
			}
		break; // end F10 key
		case CAPSLOCK_KEY: // begin capslock key
			if (dcConfig & CAPSLOCK_TO_CONTROL)
			{
				if(eventType == KEY_DOWN)
				{
					return;
				}
				keepSpecialEvent = 0;
				key = CONTROL_KEY;
				unsetCapslockFlag = 1;
				setControlFlag = 0;
				eventType = KEY_MODIFY;
				//capslockHeldDown = 0;
			}
		break; // end capslock key

	} // end switch (key)
	/*
		what we need here is to have two vars one to OR
		with flags and one to XOR - each part can add or
		remove its bit to that var.
	*/
	if (unsetCommandFlag && (flags & COMMAND_FLAG) )
	{
		flags ^= COMMAND_FLAG;
	}
	if (unsetOptionFlag && (flags & OPTION_FLAG) )
	{
		flags ^= OPTION_FLAG;
	}
	if (unsetControlFlag && (flags & CONTROL_FLAG) )
	{
		flags ^= CONTROL_FLAG;
	}
	if (unsetfnFlag && (flags & FN_FLAG) )
	{
		flags ^= FN_FLAG;
	}
	if (unsetCapslockFlag && (flags & CAPSLOCK_FLAG) )
	{
		flags ^= CAPSLOCK_FLAG;
	}
	if (setCommandFlag)
	{
		flags |= COMMAND_FLAG;
	}
	if (setControlFlag)
	{
		flags |= CONTROL_FLAG;
	}
	if (setOptionFlag)
	{
		flags |= OPTION_FLAG;
	}
	if (setfnFlag)
	{
		flags |= FN_FLAG;
	}
//	if (setCapslockFlag)
//	{
//		flags |= CAPSLOCK_FLAG;
//	}
} // end if dcConfig != 0

if(keepSpecialEvent)
{
#ifdef MB_DEBUG
	printf("sending special event type %d flags 0x%x key %d flavor %d\n", eventType, flags, key, flavor);
#endif
	IOHIDSystem::keyboardSpecialEvent(eventType, flags, key, flavor, guid, repeat, ts);
}
else
{
#ifdef MB_DEBUG
	printf("CHANGE sending hid event type %d flags 0x%x key %d charCode %d charSet %d kbdType %d caps %d\n", eventType, flags, key, charCode, charSet, lastKeyboardType, capslockHeldDown);
#endif
	keepSpecialEvent = 1;
	charSet = 254;
    IOHIDSystem::keyboardEvent(eventType, flags, key, charCode, charSet, charCode, charSet, lastKeyboardType, repeat, ts);
}

} // end MBHIDHack::keyboardSpecialEvent()
