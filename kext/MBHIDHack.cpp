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
#include "MBDCC.h"
#include <IOKit/system.h>
#include <IOKit/assert.h>


#ifdef IOHIDSystem_AVAILABLE

#include <IOKit/hidsystem/IOHIDSystem.h>
static void		*oldVtable = NULL;
static void		*myVtable = NULL;

#else

// warning: egregious abuse of preprocessor follows!
#define private public
#define protected public
#include <IOKit/hidsystem/IOHIDSystem.h>
#undef private
#undef protected

//#include <IOKit/IOCommandGate.h>

typedef struct _IOHIDCmdGateActionArgs {
    void* arg0; void* arg1; void* arg2; void* arg3; void* arg4;
    void* arg5; void* arg6; void* arg7; void* arg8; void *arg9;
} IOHIDCmdGateActionArgs;

#endif




#ifdef IOHIDSystem_AVAILABLE

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

#endif


// key remapping stuff down here.


#ifdef IOHIDSystem_AVAILABLE

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

#else

IOReturn keyboardEventAction(IOHIDSystem* self, void* arg0, void* arg1, void* arg2, void* arg3) {
    IOHIDCmdGateActionArgs* args = (IOHIDCmdGateActionArgs*)arg0;
    unsigned   eventType        = *(unsigned *)         ((IOHIDCmdGateActionArgs *)args)->arg0;
    unsigned   flags            = *(unsigned *)         ((IOHIDCmdGateActionArgs *)args)->arg1;
    unsigned   key              = *(unsigned *)         ((IOHIDCmdGateActionArgs *)args)->arg2;
    unsigned   charCode         = *(unsigned *)         ((IOHIDCmdGateActionArgs *)args)->arg3;
    unsigned   charSet          = *(unsigned *)         ((IOHIDCmdGateActionArgs *)args)->arg4;
    unsigned   origCharCode     = *(unsigned *)         ((IOHIDCmdGateActionArgs *)args)->arg5;
    unsigned   origCharSet      = *(unsigned *)         ((IOHIDCmdGateActionArgs *)args)->arg6;
    unsigned   keyboardType     = *(unsigned *)         ((IOHIDCmdGateActionArgs *)args)->arg7;
    bool       repeat           = *(bool *)             ((IOHIDCmdGateActionArgs *)args)->arg8;
    AbsoluteTime ts             = *(AbsoluteTime *)     ((IOHIDCmdGateActionArgs *)args)->arg9;


#endif

	unsigned flavor = 0;
	UInt64 guid = 0;
#ifdef MB_DEBUG
	printf("caught  hid event type %d flags 0x%x key %d charCode %d charSet %d origCharCode %d origCharSet %d kbdType %d\n", eventType, flags, key, charCode, charSet, origCharCode, origCharSet, keyboardType);
#endif
dcConfig = 1;
if (dcConfig != 0)
{
	lastKeyboardType = keyboardType;

	if( (dcConfig & CAPSLOCK_DISABLED))
	{
		removeFlags |= CAPSLOCK_FLAG;
	}

	switch (key)
	{
		case ENTER_KEY: // begin enter key
		case POWERBOOKG4_2005_ENTER_KEY:
			if (dcConfig & ENTER_TO_COMMAND)
			{
                KeyToModifier(&eventType, &addFlags, &key,
                    COMMAND_FLAG, COMMAND_KEY, KEY_MODIFY);
			}
			else if (dcConfig & ENTER_TO_CONTROL)
			{
                KeyToModifier(&eventType, &addFlags, &key,
                    CONTROL_FLAG, CONTROL_KEY, KEY_MODIFY);
			}
			else if (dcConfig & ENTER_TO_OPTION)
			{
                KeyToModifier(&eventType, &addFlags, &key,
                    OPTION_FLAG, OPTION_KEY, KEY_MODIFY);
			}
			else if (dcConfig & ENTER_TO_FN)
			{
				if (eventType == KEY_DOWN)
				{
					addFlags |= FN_FLAG;
					key = FN_KEY; // we don't want any enter key stuff to get through
					eventType = KEY_MODIFY;
					inFnMode = 1;
				}
				else if (eventType == KEY_UP)
				{
					REMOVE(addFlags, FN_FLAG);
					key = FN_KEY; // we don't want any enter key stuff to get through
					eventType = KEY_MODIFY;
					inFnMode = 0;
				}
			}
			else if (dcConfig & ENTER_TO_FORWARD_DELETE)
			{
				key = FORWARD_DELETE;
				// FN_FLAG is needed for Office
				flags = FN_FLAG;
				charCode = 45;
				charSet = 254;
				origCharCode = 45;
				origCharSet = 254;
			}
		break; // end enter key

		case COMMAND_KEY: // begin command key
		case COMMAND_KEY_R:
			if (dcConfig & COMMAND_TO_OPTION)
			{
				if (commandHeldDown) // this event is a key up
				{
					commandHeldDown = 0;
					REMOVE(addFlags, OPTION_FLAG);
					if( key == COMMAND_KEY )
					{
						REMOVE(removeFlags, COMMAND_FLAG);
					}
					if( key == COMMAND_KEY_R )
					{
						REMOVE(removeFlags, COMMAND_FLAG_R);
					}
				}
				else // this event is a key down
				{
					commandHeldDown = 1;
					addFlags |= OPTION_FLAG;
					if (key == COMMAND_KEY)
					{
						removeFlags |= COMMAND_FLAG;
					}
					if (key == COMMAND_KEY_R)
					{
						removeFlags |= COMMAND_FLAG_R;
					}
				}
				key = OPTION_KEY;
			}
			else if (dcConfig & COMMAND_TO_CONTROL)
			{
				if (commandHeldDown) // this event is a key up
				{
					commandHeldDown = 0;
					REMOVE(addFlags, CONTROL_FLAG);
					if( key == COMMAND_KEY )
					{
						REMOVE(removeFlags, COMMAND_FLAG);
					}
					if( key == COMMAND_KEY_R )
					{
						REMOVE(removeFlags, COMMAND_FLAG_R);
					}
				}
				else // this event is a key down
				{
					commandHeldDown = 1;
					addFlags |= CONTROL_FLAG;
					if( key == COMMAND_KEY )
					{
						removeFlags |= COMMAND_FLAG;
					}
					if( key == COMMAND_KEY_R )
					{
						removeFlags |= COMMAND_FLAG_R;
					}
				}
				key = CONTROL_KEY;
			}
			else if (dcConfig & DISABLE_COMMAND_AND_OPTION)
			{
				if (commandHeldDown) // this event is a key up
				{
					commandHeldDown = 0;
					if( key == COMMAND_KEY )
					{
						REMOVE(removeFlags, COMMAND_FLAG);
					}
					if( key == COMMAND_KEY_R )
					{
						REMOVE(removeFlags, COMMAND_FLAG_R);
					}
				}
				else // this event is a key down
				{
					commandHeldDown = 1;
					if( key == COMMAND_KEY )
					{
						removeFlags |= COMMAND_FLAG;
					}
					if( key == COMMAND_KEY_R )
					{
						removeFlags |= COMMAND_FLAG_R;
					}
				}
			}
		break; // end command key

		case OPTION_KEY: // begin option key
		case OPTION_KEY_R:
			if (dcConfig & OPTION_TO_COMMAND)
			{
				if (optionHeldDown) // this event is a key up
				{
					optionHeldDown = 0;
					REMOVE(addFlags, COMMAND_FLAG);
					if( key == OPTION_KEY )
					{
						REMOVE(removeFlags, OPTION_FLAG);
					}
					if( key == OPTION_KEY_R )
					{
						REMOVE(removeFlags, OPTION_FLAG_R);
					}
				}
				else // this event is a key down
				{
					optionHeldDown = 1;
					addFlags |= COMMAND_FLAG;
					if( key == OPTION_KEY )
					{
						removeFlags |= OPTION_FLAG;
					}
					if( key == OPTION_KEY_R )
					{
						removeFlags |= OPTION_FLAG_R;
					}
				}
				key = COMMAND_KEY;
			}
			else if (dcConfig & OPTION_TO_FN)
			{
				if (optionHeldDown) // this event is a key up
				{
					optionHeldDown = 0;
					REMOVE(addFlags, FN_FLAG);
					if( key == OPTION_KEY )
					{
						REMOVE(removeFlags, OPTION_FLAG);
					}
					if( key == OPTION_KEY_R )
					{
						REMOVE(removeFlags, OPTION_FLAG_R);
					}
				}
				else // this event is a key down
				{
					optionHeldDown = 1;
					addFlags |= FN_FLAG;
					if( key == OPTION_KEY )
					{
						removeFlags |= OPTION_FLAG;
					}
					if( key == OPTION_KEY_R )
					{
						removeFlags |= OPTION_FLAG_R;
					}
				}
				key = FN_KEY;
			}
			else if (dcConfig & DISABLE_COMMAND_AND_OPTION)
			{
				if (optionHeldDown) // this event is a key up
				{
					optionHeldDown = 0;
					if( key == OPTION_KEY )
					{
						REMOVE(removeFlags, OPTION_FLAG);
					}
					if( key == OPTION_KEY_R )
					{
						REMOVE(removeFlags, OPTION_FLAG_R);
					}
				}
				else // this event is a key down
				{
					optionHeldDown = 1;
					if( key == OPTION_KEY )
					{
						removeFlags |= OPTION_FLAG;
					}
					if( key == OPTION_KEY_R )
					{
						removeFlags |= OPTION_FLAG_R;
					}
				}
			}
			else if (dcConfig & SWAP_CONTROL_AND_OPTION) // control <-> option
			{
				if (optionHeldDown) // this event is a key up
				{
					optionHeldDown = 0;
					REMOVE(addFlags, CONTROL_FLAG);
					if (!controlHeldDown)
					{
						if( key == OPTION_KEY )
						{
							REMOVE(removeFlags, OPTION_FLAG);
						}
						if( key == OPTION_KEY_R )
						{
							REMOVE(removeFlags, OPTION_FLAG_R);
						}
					}
				}
				else // this event is a key down
				{
					optionHeldDown = 1;
					addFlags |= CONTROL_FLAG;
					if (!controlHeldDown)
					{
						if( key == OPTION_KEY )
						{
							removeFlags |= OPTION_FLAG;
						}
						if( key == OPTION_KEY_R )
						{
							removeFlags |= OPTION_FLAG_R;
						}
					}
				}
				key = CONTROL_KEY;
			}
		break; // end option key

		case CONTROL_KEY: // begin control key
		case CONTROL_KEY_R:
			if (dcConfig & CONTROL_TO_COMMAND)
			{
				if (controlHeldDown) // this event is a key up
				{
					controlHeldDown = 0;
					REMOVE(addFlags, COMMAND_FLAG);
					if( key == CONTROL_KEY )
					{
						REMOVE(removeFlags, CONTROL_FLAG);
					}
					if( key == CONTROL_KEY_R )
					{
						REMOVE(removeFlags, CONTROL_FLAG_R);
					}
				}
				else // this event is a key down
				{
					controlHeldDown = 1;
					addFlags |= COMMAND_FLAG;
					if( key == CONTROL_KEY )
					{
						removeFlags |= CONTROL_FLAG;
					}
					if( key == CONTROL_KEY_R )
					{
						removeFlags |= CONTROL_FLAG_R;
					}
				}
				key = COMMAND_KEY;
			}
			else if (dcConfig & SWAP_CONTROL_AND_OPTION) // control <-> option
			{
				if (controlHeldDown) // this event is a key up
				{
					controlHeldDown = 0;
					REMOVE(addFlags, OPTION_FLAG);
					if (!optionHeldDown)
					{
						if( key == CONTROL_KEY )
						{
							REMOVE(removeFlags, CONTROL_FLAG);
						}
						if( key == CONTROL_KEY_R )
						{
							REMOVE(removeFlags, CONTROL_FLAG_R);
						}
					}
				}
				else // this event is a key down
				{
					controlHeldDown = 1;
					addFlags |= OPTION_FLAG;
					if (!optionHeldDown)
					{
						if( key == CONTROL_KEY )
						{
							removeFlags |= CONTROL_FLAG;
						}
						if( key == CONTROL_KEY_R )
						{
							removeFlags |= CONTROL_FLAG_R;
						}
					}
				}
				key = OPTION_KEY;
			}
			else if (dcConfig & CONTROL_TO_FN)
			{
				if (controlHeldDown) // this event is a key up
				{
					controlHeldDown = 0;
					REMOVE(addFlags, FN_FLAG);
					if( key == CONTROL_KEY )
					{
						REMOVE(removeFlags, CONTROL_FLAG);
					}
					if( key == CONTROL_KEY_R )
					{
						REMOVE(removeFlags, CONTROL_FLAG_R);
					}
				}
				else // this event is a key down
				{
					controlHeldDown = 1;
					addFlags |= FN_FLAG;
					if( key == CONTROL_KEY )
					{
						removeFlags |= CONTROL_FLAG;
					}
					if( key == CONTROL_KEY_R )
					{
						removeFlags |= CONTROL_FLAG_R;
					}
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
				REMOVE(removeFlags, FN_FLAG);
			}
			if (dcConfig & FN_TO_CONTROL)
			{
				if (fnHeldDown) // this event is a key down
				{
					addFlags |= CONTROL_FLAG;
					removeFlags |= FN_FLAG;
				}
				else // this event is a key up
				{
					REMOVE(addFlags, CONTROL_FLAG);
					REMOVE(removeFlags, FN_FLAG);
				}
				key = CONTROL_KEY;
			}
		break; // end fn key

		case DELETE_KEY: // begin delete key
			// Make Shift + Delete send a Forward Delete key
			if (dcConfig & SHIFT_DELETE_TO_FORWARD_DELETE)
			{
				if (flags == SHIFT_FLAG) // with _only_ shift held as well
				{
					key = FORWARD_DELETE;
					// FN_FLAG is needed for Office
					flags = FN_FLAG;
					charCode = 45;
					charSet = 254;
					origCharCode = 45;
					origCharSet = 254;
				}
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
				removeFlags |= CAPSLOCK_FLAG;
				key = CONTROL_KEY;
				//capslockHeldDown = 1;
				addFlags |= CONTROL_FLAG;
				if(flags & CAPSLOCK_FLAG)
				{
					//flags ^= CAPSLOCK_FLAG;
					//flags |= CONTROL_FLAG;
					//capslockOn = 1;
				}
				else if(keyboardType == POWERBOOKG3_KEYBOARD || keyboardType == IBOOK_KEYBOARD)
				{
					// key up for 'books
					//capslockHeldDown = 0;
					REMOVE(addFlags, CONTROL_FLAG);
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
				if (flags & CONTROL_FLAG)
				{
					removeFlags |= CONTROL_FLAG;
				}
				else if (flags & CONTROL_FLAG_R)
				{
					removeFlags |= CONTROL_FLAG_R;
				}
				else
				{
					key = LEFT_ARROW_KEY;
					flags |= COMMAND_FLAG;
				}
				//if (eventType == KEY_DOWN)
				//{
					//addFlags |= COMMAND_FLAG;
				//}
				//else if (eventType == KEY_UP)
				//{
				//	REMOVE(addFlags, COMMAND_FLAG);
				//}
			}
		break; // end home key

		case END_KEY: // begin end key
			if(dcConfig & PC_STYLE_HOME_AND_END)
			{
				if (flags & CONTROL_FLAG)
				{
					removeFlags |= CONTROL_FLAG;
				}
				else if (flags & CONTROL_FLAG_R)
				{
					removeFlags |= CONTROL_FLAG_R;
				}
				else
				{
					key = RIGHT_ARROW_KEY;
					flags |= COMMAND_FLAG;
				}
				//if (eventType == KEY_DOWN)
				//{
				//	addFlags |= COMMAND_FLAG;
				//}
				//else if (eventType == KEY_UP)
				//{
				//	REMOVE(addFlags, COMMAND_FLAG);
				//}
			}
		break; // end end key

		case BACKSLASH_KEY: // begin backslash key
			if(dcConfig & BACKSLASH_TO_FORWARD_DELETE)
			{
				if (flags == 0x0)
				{
					key = FORWARD_DELETE;
				}
			}
		break; // end backslash key

		case F1: // begin F1 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						key = BRIGHTNESS_DOWN;
						flavor = BRIGHTNESS_DOWN_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F1 key
		case F2: // begin F2 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						key = BRIGHTNESS_UP;
						flavor = BRIGHTNESS_UP_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F2 key
		case F3: // begin F3 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
						key = VOLUME_MUTE;
						flavor = VOLUME_MUTE_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						key = VOLUME_DOWN;
						flavor = VOLUME_DOWN_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F3 key
		case F4: // begin F4 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
						key = VOLUME_DOWN;
						flavor = VOLUME_DOWN_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						key = VOLUME_UP;
						flavor = VOLUME_UP_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F4 key
		case F5: // begin F5 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
						key = VOLUME_UP;
						flavor = VOLUME_UP_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						key = SPECIAL_KEY;
						flavor = NUMLOCK_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F5 key
		case F6: // begin F6 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
						key = SPECIAL_KEY;
						flavor = NUMLOCK_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						key = VOLUME_MUTE;
						flavor = VOLUME_MUTE_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F6 key
		/* who wants these other keys? anyone?
		actually, we can probably do this if the fn flag is set - 
		that would mean a 17" PBG4 keyboard */
		case F7: // begin F7 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						key = F7a;
						flavor = F7a_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F7 key
		case F8: // begin F8 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						key = F8a;
						flavor = F8a_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F8 key
		case F9: // begin F9 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						key = F9a;
						flavor = F9a_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F9 key
		case F10: // begin F10 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						key = F10a;
						flavor = F10a_FLAVOUR;
						keepKeyboardEvent = 0;
						removeFlags |= FN_FLAG;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F10 key
		case F11: // begin F11 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						flags |= FN_FLAG;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F11 key
		case F12: // begin F12 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						if ( flags & FN_FLAG )
						{
							key = EJECT;
							flavor = EJECT_FLAVOUR;
							keepKeyboardEvent = 0;
							removeFlags |= FN_FLAG;
						}
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F12 key

	} // end switch (key)

	// begin supplied by Giel Scharff <mgsch@mac.com>
	if (inFnMode)
	{   //up -> PgUp
		if ((key == 126) /*&& (flags == 0x200000)*/ && (charCode == 173) && (charSet == 1) &&
	  (origCharCode == 173) && (origCharSet == 1))
		{
			key = 116; flags = 0x800000; charCode =  48; charSet = 254; origCharCode =  48;  origCharSet = 254;
		}
		//down -> PgDown
		else if ((key == 125) /*&& (flags == 0x200000)*/ && (charCode == 175) && (charSet == 1) &&
		   (origCharCode == 175) && (origCharSet == 1))
		{
			key = 121; flags = 0x800000; charCode =  49; charSet = 254; origCharCode =  49;  origCharSet = 254;
		}
		//left -> Home
		else if ((key == 123) /*&& (flags == 0x200000)*/ && (charCode == 172) && (charSet == 1) &&
		   (origCharCode == 172) && (origCharSet == 1))
		{
			key = 115; flags = 0x800000; charCode =  46; charSet = 254; origCharCode =  46;  origCharSet = 254;
		}
		//right -> End
		else if ((key == 124) /*&& (flags == 0x200000)*/ && (charCode == 174) && (charSet == 1) &&
		   (origCharCode == 174) && (origCharSet == 1))
		{
			key = 119; flags = 0x800000; charCode =  47; charSet = 254; origCharCode =  46;  origCharSet = 254;
		}
	}
	// end supplied by Giel Scharff <mgsch@mac.com>

	removeFlags &= flags;
	flags ^= removeFlags;
	flags |= addFlags;
	//printf("flags 0x%x, aflags 0x%x, rflags 0x%x\n", flags, addFlags, removeFlags);
} // end if dcConfig != 0

if(keepKeyboardEvent)
{
#ifdef MB_DEBUG
	printf("sending hid event type %d flags 0x%x key %d charCode %d charSet %d origCharCode %d origCharSet %d kbdType %d\n", eventType, flags, key, charCode, charSet, origCharCode, origCharSet, keyboardType);
#endif

#ifdef IOHIDSystem_AVAILABLE

    IOHIDSystem::keyboardEvent(eventType, flags, key, charCode, charSet, origCharCode, origCharSet, keyboardType, repeat, ts);

#else

    IOHIDSystem::instance()->keyboardEventGated(eventType, flags, key, charCode, charSet, origCharCode, origCharSet, keyboardType, repeat, ts, NULL);


#endif

}
else
{
#ifdef MB_DEBUG
	printf("CHANGE sending special event type %d flags 0x%x key %d flavor %d\n", eventType, flags, key, flavor);
#endif
	keepKeyboardEvent = 1;

#ifdef IOHIDSystem_AVAILABLE

	IOHIDSystem::keyboardSpecialEvent(eventType, flags, key, flavor, guid, repeat, ts);

#else

    IOHIDSystem::instance()->keyboardSpecialEventGated(eventType, flags, key, flavor, guid, repeat, ts, NULL);


#endif

}
#ifdef IOHIDSystem_AVAILABLE
#else
	return kIOReturnSuccess;
#endif

} // end MBHIDHack::keyboardEvent()


#ifdef IOHIDSystem_AVAILABLE

//----------------------------------------------------------------------------
void MBHIDHack::keyboardSpecialEvent(   unsigned   eventType,
                       /* flags */        unsigned   flags,
                       /* keyCode  */     unsigned   key,
                       /* specialty */    unsigned   flavor,
                       /* guid */         UInt64     guid,
                       /* repeat */       bool       repeat,
                       /* atTime */       AbsoluteTime ts)
{

#else

IOReturn keyboardSpecialEventAction(IOHIDSystem* self, void* arg0, void* arg1, void* arg2, void* arg3) {
      IOHIDCmdGateActionArgs* args = (IOHIDCmdGateActionArgs*)arg0;
      unsigned   eventType= *(unsigned *) ((IOHIDCmdGateActionArgs *)args)->arg0;
      unsigned   flags    = *(unsigned *) ((IOHIDCmdGateActionArgs *)args)->arg1;
      unsigned   key      = *(unsigned *) ((IOHIDCmdGateActionArgs *)args)->arg2;
      unsigned   flavor   = *(unsigned *) ((IOHIDCmdGateActionArgs *)args)->arg3;
      UInt64     guid     = *(UInt64 *)   ((IOHIDCmdGateActionArgs *)args)->arg4;
      bool       repeat   = *(bool *)     ((IOHIDCmdGateActionArgs *)args)->arg5;
      AbsoluteTime ts     = *(AbsoluteTime *)((IOHIDCmdGateActionArgs *)args)->arg6;


#endif

	unsigned charCode = 0;
	unsigned charSet = 0;
	//unsigned keyboardType = lastKeyboardType;
	//unsigned origCharCode = charCode;
	//unsigned origCharSet = charSet;
#ifdef MB_DEBUG
	printf("caught  special event type %d flags 0x%x key %d flavor %d\n", eventType, flags, key, flavor);
#endif

if (dcConfig != 0)
{
	switch (key)
	{
		case BRIGHTNESS_DOWN: // begin F1 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F1;
						charCode = 32;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F1 key
		case BRIGHTNESS_UP: // begin F2 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F2;
						charCode = 33;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F2 key
		case VOLUME_MUTE: // begin VOLUME_MUTE key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F3;
						charCode = 34;
					break;
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F6;
						charCode = 37;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end VOLUME_MUTE key
		case VOLUME_DOWN: // begin F4 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F4;
						charCode = 35;
					break;
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F3;
						charCode = 34;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F4 key
		case VOLUME_UP: // begin F5 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F5;
						charCode = 36;
					break;
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F4;
						charCode = 35;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F5 key
		case SPECIAL_KEY: // begin F6 key
			if ( (dcConfig & SWAP_FUNCTION_KEYS) && (flavor == NUMLOCK_FLAVOUR) )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F6;
						charCode = 37;
					break;
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F5;
						charCode = 36;
					break;
				} // end switch (lastKeyboardType)
			}
			else if( (dcConfig & CAPSLOCK_TO_CONTROL) && (flags & CAPSLOCK_FLAG) )
			{
				keepSpecialEvent = 0;
				key = CONTROL_KEY;
				charCode = 0;
				eventType = KEY_MODIFY;
				removeFlags |= CAPSLOCK_FLAG;
				// we toggle between states for this, since the key up and key down
				// for the different states look the same at this level
				if (!capslockHeldDown)
				{
					capslockHeldDown = 1;
					addFlags |= CONTROL_FLAG;
				}
				else
				{
					capslockHeldDown = 0;
					REMOVE(addFlags, CONTROL_FLAG);
				}
			}
		break; // end F6 key
		case F7a: // begin F7 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F7;
						charCode = 38;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F7 key
		case F8a: // begin F8 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F8;
						charCode = 39;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F8 key
		case F9a: // begin F9 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F9;
						charCode = 40;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F9 key
		case F10a: // begin F10 key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F10;
						charCode = 41;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end F10 key
		case EJECT: // begin EJECT key
			if ( dcConfig & SWAP_FUNCTION_KEYS )
			{
				switch (lastKeyboardType)
				{
					case IBOOK_KEYBOARD:
					case POWERBOOKG3_KEYBOARD:
					case TIBOOK_KEYBOARD:
						keepSpecialEvent = 0;
						key = F12;
						charCode = 43;
					break;
				} // end switch (lastKeyboardType)
			}
		break; // end EJECT key
		case CAPSLOCK_KEY: // begin capslock key
			// USB keyboards use a different key code, and
			// also output an extra event which we can ignore
			if (dcConfig & CAPSLOCK_TO_CONTROL)
			{
				if(eventType == KEY_DOWN)
				{
#ifdef IOHIDSystem_AVAILABLE
					return;
#else
					return kIOReturnSuccess;
#endif
				}
				keepSpecialEvent = 0;
				key = CONTROL_KEY;
				removeFlags |= CAPSLOCK_FLAG;
				REMOVE(addFlags, CONTROL_FLAG);
				eventType = KEY_MODIFY;
				//capslockHeldDown = 0;
			}
		break; // end capslock key

	} // end switch (key)

	removeFlags &= flags;
	flags ^= removeFlags;
	flags |= addFlags;
} // end if dcConfig != 0

if(keepSpecialEvent)
{
#ifdef MB_DEBUG
	printf("sending special event type %d flags 0x%x key %d flavor %d\n", eventType, flags, key, flavor);
#endif

#ifdef IOHIDSystem_AVAILABLE

	IOHIDSystem::keyboardSpecialEvent(eventType, flags, key, flavor, guid, repeat, ts);

#else

    IOHIDSystem::instance()->keyboardSpecialEventGated(eventType, flags, key, flavor, guid, repeat, ts, NULL);

#endif

}
else
{
#ifdef MB_DEBUG
	printf("CHANGE sending hid event type %d flags 0x%x key %d charCode %d charSet %d kbdType %d caps %d\n", eventType, flags, key, charCode, charSet, lastKeyboardType, capslockHeldDown);
#endif
	keepSpecialEvent = 1;
	charSet = 254;

#ifdef IOHIDSystem_AVAILABLE

    IOHIDSystem::keyboardEvent(eventType, flags, key, charCode, charSet, charCode, charSet, lastKeyboardType, repeat, ts);

#else

    IOHIDSystem::instance()->keyboardEventGated(eventType, flags, key, charCode, charSet, charCode, charSet, lastKeyboardType, repeat, ts, NULL);

#endif

}
#ifdef IOHIDSystem_AVAILABLE
#else
	return kIOReturnSuccess;
#endif

} // end MBHIDHack::keyboardSpecialEvent()


#ifdef IOHIDSystem_AVAILABLE
#else


class MyCmdGate : public IOCommandGate {
public:
    IOCommandGate *parent;
    MyCmdGate(IOCommandGate *new_parent) { parent = new_parent; }

  virtual bool checkForWork() { return parent->checkForWork(); }
  virtual bool init(OSObject *owner, Action action = 0) { return 1; }
  virtual IOReturn runCommand(void *arg0 = 0, void *arg1 = 0, void *arg2 = 0, void *arg3 = 0) {
      return parent->runCommand(arg0, arg1, arg2, arg3);
  }
  virtual IOReturn attemptCommand(void *arg0 = 0, void *arg1 = 0, void *arg2 = 0, void *arg3 = 0) {
      return parent->attemptCommand(arg0, arg1, arg2, arg3);
  }
  virtual IOReturn attemptAction(Action action, void *arg0 = 0, void *arg1 = 0, void *arg2 = 0, void *arg3 = 0) {
      return parent->attemptAction(action, arg0, arg1, arg2, arg3);
  }
  virtual IOReturn commandSleep(void *event, UInt32 interruptible = THREAD_ABORTSAFE) {
      return parent->commandSleep(event, interruptible);
  }
  virtual void commandWakeup(void *event, bool oneThread = false) {
      return parent->commandWakeup(event, oneThread);
  }
  virtual IOReturn runAction(Action action, void *arg0 = 0, void *arg1 = 0, void *arg2 = 0, void *arg3 = 0) {
      if (action == (Action)&IOHIDSystem::doKeyboardSpecialEvent) {
          action = (Action)keyboardSpecialEventAction;
      } else if (action == (Action)&IOHIDSystem::doKeyboardEvent) {
          action = (Action)keyboardEventAction;
      }
      return parent->runAction(action, arg0, arg1, arg2, arg3);
  }
};

static MyCmdGate *myCmdGate = NULL;

int     MBHidInit(void)
{
    IOHIDSystem     *p = IOHIDSystem::instance();

    if (p == NULL) {
        // IOHIDSystem not loaded
        printf("error: cannot load DoubleCommand!\n");
        return 1;
    }

    if (myCmdGate == NULL) myCmdGate = new MyCmdGate(p->cmdGate);
    if (p->cmdGate == myCmdGate) {
        printf("error: module DoubleCommand already loaded!\n");
        return 1;
    }

    p->cmdGate = myCmdGate;
    return 0;
}

int     MBHidExit(void)
{
    IOHIDSystem     *p = IOHIDSystem::instance();

    if (p->cmdGate != myCmdGate) {
        printf("error: cannot unload DoubleCommand!\n");
        return 1;
    }
    
    p->cmdGate = myCmdGate->parent;
    return 0;
}

#endif