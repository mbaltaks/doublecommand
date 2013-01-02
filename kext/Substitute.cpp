#include "DoubleCommand.h"
#include "Common.h"

// int variable to set the configuration of DoubleCommand
// I think I need one of these per keyboard ID, really.
int dcConfig = 0;
int dcConfigAll = 0;
int dcConfig1 = 0;
int dcConfig2 = 0;
int dcConfig3 = 0;
int dcConfig4 = 0;
int dcConfigArray[MAX_KEYBOARDS];
int dcKeyboard1 = 0;
int dcKeyboard2 = 0;
int dcKeyboard3 = 0;
int dcKeyboard4 = 0;
int dcKeyboardArray[MAX_KEYBOARDS];

//unsigned char setCommandFlag = 0;
//unsigned char setControlFlag = 0;
//unsigned char setOptionFlag = 0;
//unsigned char setfnFlag = 0;
//unsigned char setCapslockFlag = 0;
unsigned char commandHeldDown = 0;
unsigned char optionHeldDown = 0;
unsigned char controlHeldDown = 0;
unsigned char fnHeldDown = 0;
unsigned char capslockHeldDown = 1;
unsigned char capslockOn = 0;
unsigned char inFnMode = 0;
bool tildeHeldDown = false;
//unsigned char unsetCommandFlag = 0;
//unsigned char unsetOptionFlag = 0;
//unsigned char unsetControlFlag = 0;
//unsigned char unsetfnFlag = 0;
//unsigned char unsetCapslockFlag = 0;

unsigned char keepSpecialEvent = 1;
unsigned char keepKeyboardEvent = 1;

unsigned lastKeyboardType = 202;
unsigned addFlags = 0;
unsigned removeFlags = 0;

int remap(unsigned * eventType,
	unsigned * flags,
	unsigned * key,
	unsigned * charCode,
	unsigned * charSet,
	unsigned * origCharCode,
	unsigned * origCharSet,
	unsigned * keyboardType,
	bool * repeat,
	AbsoluteTime * ts)
{
int return_value = kContinue;
//addFlags = 0;
//removeFlags = 0;

#ifdef MB_DEBUG
	printf("caught  hid event type %d flags 0x%x key %d ", *eventType, *flags, *key);
	printf("charCode %d charSet %d ", *charCode, *charSet);
	printf("origCharCode %d origCharSet %d kbdType %d ",
		*origCharCode, *origCharSet, *keyboardType);
	printf("repeat %d ts %d\n", *repeat, *ts);
#endif


dcConfigArray[0] = dcConfig1;
dcConfigArray[1] = dcConfig2;
dcConfigArray[2] = dcConfig3;
dcConfigArray[3] = dcConfig4;
dcKeyboardArray[0] = dcKeyboard1;
dcKeyboardArray[1] = dcKeyboard2;
dcKeyboardArray[2] = dcKeyboard3;
dcKeyboardArray[3] = dcKeyboard4;
/*printf("config0 = %d\n", dcConfigArray[0]);
printf("config1 = %d\n", dcConfigArray[1]);
printf("config2 = %d\n", dcConfigArray[2]);
printf("config3 = %d\n", dcConfigArray[3]);
printf("keyboard0 = %d\n", dcKeyboardArray[0]);
printf("keyboard1 = %d\n", dcKeyboardArray[1]);
printf("keyboard2 = %d\n", dcKeyboardArray[2]);
printf("keyboard3 = %d\n", dcKeyboardArray[3]);*/
int i = 0;
dcConfig = dcConfigAll;
for (i = 0; i < MAX_KEYBOARDS; i++)
{
	if ( (dcKeyboardArray[i] == 0)
		|| (*keyboardType == dcKeyboardArray[i]) )
	{
		dcConfig |= dcConfigArray[i];
	}
}
//printf("final dcConfig = %d\n", dcConfig);



if (dcConfig != 0)
{
	if (dcConfig & MB_DEBUG_OUTPUT)
	{
		printf("caught  hid event type %d flags 0x%x key %d ", *eventType, *flags, *key);
		printf("charCode %d charSet %d ", *charCode, *charSet);
		printf("origCharCode %d origCharSet %d kbdType %d ",
			*origCharCode, *origCharSet, *keyboardType);
		printf("repeat %d ts %llud\n", *repeat, *ts);
	}

	lastKeyboardType = *keyboardType;

	if( (dcConfig & CAPSLOCK_DISABLED))
	{
		removeFlags |= CAPSLOCK_FLAG;
	}

	switch (*key)
	{
		case ENTER_KEY: // begin enter key
		case POWERBOOKG4_2005_ENTER_KEY:
			if (dcConfig & ENTER_TO_COMMAND)
			{
				KeyToModifier(eventType, &addFlags, key,
					COMMAND_FLAG, COMMAND_KEY, KEY_MODIFY);
				removeFlags |= ENTER_KEY_FLAG;
				*charCode = 0;
				*origCharCode = 0;
			}
			else if (dcConfig & ENTER_TO_CONTROL)
			{
				KeyToModifier(eventType, &addFlags, key,
					CONTROL_FLAG, CONTROL_KEY, KEY_MODIFY);
				removeFlags |= ENTER_KEY_FLAG;
				*charCode = 0;
				*origCharCode = 0;
			}
			else if (dcConfig & ENTER_TO_OPTION)
			{
				KeyToModifier(eventType, &addFlags, key,
					OPTION_FLAG_R, OPTION_KEY_R, KEY_MODIFY);
				removeFlags |= ENTER_KEY_FLAG;
				*charCode = 0;
				*origCharCode = 0;
			}
			else if (dcConfig & ENTER_TO_FN)
			{
				if (*eventType == KEY_DOWN)
				{
					addFlags |= FN_FLAG;
					*key = FN_KEY; // we don't want any enter key stuff to get through
					*eventType = KEY_MODIFY;
					inFnMode = 1;
				}
				else if (*eventType == KEY_UP)
				{
					REMOVE(addFlags, FN_FLAG);
					*key = FN_KEY; // we don't want any enter key stuff to get through
					*eventType = KEY_MODIFY;
					inFnMode = 0;
				}
			}
			else if (dcConfig & ENTER_TO_FORWARD_DELETE)
			{
				*key = FORWARD_DELETE;
				// FN_FLAG is needed for Office
				*flags = FN_FLAG;
				*charCode = 45;
				*charSet = 254;
				*origCharCode = 45;
				*origCharSet = 254;
			}
		break; // end enter key

		case COMMAND_KEY: // begin command key
		case COMMAND_KEY_R:

/* Uncomment this if doing this mod via behavior modification, rather than key
 * code translation.
			if (dcConfig & SWAP_TILDE_AND_COMMAND) {
				*key = TILDE_KEY;
			}
			else
*/

			if (dcConfig & COMMAND_TO_OPTION)
			{
				if (commandHeldDown) // this event is a key up
				{
					commandHeldDown = 0;
					REMOVE(addFlags, OPTION_FLAG);
					if( *key == COMMAND_KEY )
					{
						REMOVE(removeFlags, COMMAND_FLAG);
					}
					if( *key == COMMAND_KEY_R )
					{
						REMOVE(removeFlags, COMMAND_FLAG_R);
					}
				}
				else // this event is a key down
				{
					commandHeldDown = 1;
					addFlags |= OPTION_FLAG;
					if (*key == COMMAND_KEY)
					{
						removeFlags |= COMMAND_FLAG;
					}
					if (*key == COMMAND_KEY_R)
					{
						removeFlags |= COMMAND_FLAG_R;
					}
				}
				*key = OPTION_KEY;
			}
			else if (dcConfig & COMMAND_TO_CONTROL)
			{
				if (commandHeldDown) // this event is a key up
				{
					commandHeldDown = 0;
					REMOVE(addFlags, CONTROL_FLAG);
					if( *key == COMMAND_KEY )
					{
						REMOVE(removeFlags, COMMAND_FLAG);
					}
					if( *key == COMMAND_KEY_R )
					{
						REMOVE(removeFlags, COMMAND_FLAG_R);
					}
				}
				else // this event is a key down
				{
					commandHeldDown = 1;
					addFlags |= CONTROL_FLAG;
					if( *key == COMMAND_KEY )
					{
						removeFlags |= COMMAND_FLAG;
					}
					if( *key == COMMAND_KEY_R )
					{
						removeFlags |= COMMAND_FLAG_R;
					}
				}
				*key = CONTROL_KEY;
			}
			else if (dcConfig & DISABLE_COMMAND_AND_OPTION)
			{
				if (commandHeldDown) // this event is a key up
				{
					commandHeldDown = 0;
					if( *key == COMMAND_KEY )
					{
						REMOVE(removeFlags, COMMAND_FLAG);
					}
					if( *key == COMMAND_KEY_R )
					{
						REMOVE(removeFlags, COMMAND_FLAG_R);
					}
				}
				else // this event is a key down
				{
					commandHeldDown = 1;
					if( *key == COMMAND_KEY )
					{
						removeFlags |= COMMAND_FLAG;
					}
					if( *key == COMMAND_KEY_R )
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
					if( *key == OPTION_KEY )
					{
						REMOVE(removeFlags, OPTION_FLAG);
					}
					if( *key == OPTION_KEY_R )
					{
						REMOVE(removeFlags, OPTION_FLAG_R);
					}
				}
				else // this event is a key down
				{
					optionHeldDown = 1;
					addFlags |= COMMAND_FLAG;
					if( *key == OPTION_KEY )
					{
						removeFlags |= OPTION_FLAG;
					}
					if( *key == OPTION_KEY_R )
					{
						removeFlags |= OPTION_FLAG_R;
					}
				}
				*key = COMMAND_KEY;
			}
			else if (dcConfig & OPTION_TO_FN)
			{
				if (optionHeldDown) // this event is a key up
				{
					*eventType = KEY_MODIFY;
					inFnMode = 0;
					optionHeldDown = 0;
					REMOVE(addFlags, FN_FLAG);
					if( *key == OPTION_KEY )
					{
						REMOVE(removeFlags, OPTION_FLAG);
					}
					if( *key == OPTION_KEY_R )
					{
						REMOVE(removeFlags, OPTION_FLAG_R);
					}
				}
				else // this event is a key down
				{
					*eventType = KEY_MODIFY;
					inFnMode = 1;
					optionHeldDown = 1;
					addFlags |= FN_FLAG;
					if( *key == OPTION_KEY )
					{
						removeFlags |= OPTION_FLAG;
					}
					if( *key == OPTION_KEY_R )
					{
						removeFlags |= OPTION_FLAG_R;
					}
				}
				*key = FN_KEY;
			}
			else if (dcConfig & DISABLE_COMMAND_AND_OPTION)
			{
				if (optionHeldDown) // this event is a key up
				{
					optionHeldDown = 0;
					if( *key == OPTION_KEY )
					{
						REMOVE(removeFlags, OPTION_FLAG);
					}
					if( *key == OPTION_KEY_R )
					{
						REMOVE(removeFlags, OPTION_FLAG_R);
					}
				}
				else // this event is a key down
				{
					optionHeldDown = 1;
					if( *key == OPTION_KEY )
					{
						removeFlags |= OPTION_FLAG;
					}
					if( *key == OPTION_KEY_R )
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
						if( *key == OPTION_KEY )
						{
							REMOVE(removeFlags, OPTION_FLAG);
						}
						if( *key == OPTION_KEY_R )
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
						if( *key == OPTION_KEY )
						{
							removeFlags |= OPTION_FLAG;
						}
						if( *key == OPTION_KEY_R )
						{
							removeFlags |= OPTION_FLAG_R;
						}
					}
				}
				*key = CONTROL_KEY;
			}

			//Added by Sastira - sastira@gmail.com
			else if (dcConfig & OPTION_KEY_R_TO_CONTROL && *key == OPTION_KEY_R) // control <-> option
			{
				if (optionHeldDown) // this event is a key up
				{
					optionHeldDown = 0;
					REMOVE(addFlags, CONTROL_FLAG);
					if (!controlHeldDown)
					{
						REMOVE(removeFlags, OPTION_FLAG_R);	
					}
				}
				else // this event is a key down
				{
					optionHeldDown = 1;
					addFlags |= CONTROL_FLAG;
					removeFlags |= OPTION_FLAG_R;
				}
				*key = CONTROL_KEY;
			}
			else if (dcConfig & OPTION_KEY_R_TO_FORWARD_DELETE && *key == OPTION_KEY_R)
			{
				if (optionHeldDown) // this event is a key up
				{
					optionHeldDown = 0;
					if (!controlHeldDown)
					{				
						REMOVE(removeFlags, OPTION_FLAG_R);
						*eventType = kKeyUp;
					}
				}
				else // this event is a key down
				{
					optionHeldDown = 1;
					if (!controlHeldDown)
					{
						removeFlags |= OPTION_FLAG_R;						
						*eventType = kKeyDown;
					}
				}
				*key = FORWARD_DELETE;
				
				// FN_FLAG is needed for Office
				*flags = FN_FLAG;
				*charCode = 45;
				*charSet = 254;
				*origCharCode = 45;
				*origCharSet = 254;			
			}
			else if (dcConfig & OPTION_KEY_R_TO_ENTER && *key == OPTION_KEY_R)
			{
				if (optionHeldDown) // this event is a key up
				{
					optionHeldDown = 0;
					if (!controlHeldDown)
					{				
						REMOVE(removeFlags, OPTION_FLAG_R);
						REMOVE(addFlags, ENTER_KEY_FLAG);
						*eventType = kKeyUp;
					}
				}
				else // this event is a key down
				{
					optionHeldDown = 1;
					if (!controlHeldDown)
					{
						removeFlags |= OPTION_FLAG_R;
						addFlags |= ENTER_KEY_FLAG;				
						*eventType = kKeyDown;
					}
				}
				*key = ENTER_KEY;				
			}			
			//End Addition by Sastira

		break; // end option key

		case CONTROL_KEY: // begin control key
		case CONTROL_KEY_R:
            REMOVE(removeFlags, CONTROL_FLAG);
            REMOVE(removeFlags, CONTROL_FLAG_R);
			if (dcConfig & CONTROL_TO_COMMAND)
			{
				if (controlHeldDown) // this event is a key up
				{
					controlHeldDown = 0;
					REMOVE(addFlags, COMMAND_FLAG);
					if( *key == CONTROL_KEY )
					{
						REMOVE(removeFlags, CONTROL_FLAG);
					}
					if( *key == CONTROL_KEY_R )
					{
						REMOVE(removeFlags, CONTROL_FLAG_R);
					}
				}
				else // this event is a key down
				{
					controlHeldDown = 1;
					addFlags |= COMMAND_FLAG;
					if( *key == CONTROL_KEY )
					{
						removeFlags |= CONTROL_FLAG;
					}
					if( *key == CONTROL_KEY_R )
					{
						removeFlags |= CONTROL_FLAG_R;
					}
				}
				*key = COMMAND_KEY;
			}
			else if (dcConfig & SWAP_CONTROL_AND_OPTION) // control <-> option
			{
				if (controlHeldDown) // this event is a key up
				{
					controlHeldDown = 0;
					REMOVE(addFlags, OPTION_FLAG);
					if (!optionHeldDown)
					{
						if( *key == CONTROL_KEY )
						{
							REMOVE(removeFlags, CONTROL_FLAG);
						}
						if( *key == CONTROL_KEY_R )
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
						if( *key == CONTROL_KEY )
						{
							removeFlags |= CONTROL_FLAG;
						}
						if( *key == CONTROL_KEY_R )
						{
							removeFlags |= CONTROL_FLAG_R;
						}
					}
				}
				*key = OPTION_KEY;
			}
			else if (dcConfig & CONTROL_TO_FN)
			{
				if (controlHeldDown) // this event is a key up
				{
					controlHeldDown = 0;
					REMOVE(addFlags, FN_FLAG);
					if( *key == CONTROL_KEY )
					{
						REMOVE(removeFlags, CONTROL_FLAG);
					}
					if( *key == CONTROL_KEY_R )
					{
						REMOVE(removeFlags, CONTROL_FLAG_R);
					}
				}
				else // this event is a key down
				{
					controlHeldDown = 1;
					addFlags |= FN_FLAG;
					if( *key == CONTROL_KEY )
					{
						removeFlags |= CONTROL_FLAG;
					}
					if( *key == CONTROL_KEY_R )
					{
						removeFlags |= CONTROL_FLAG_R;
					}
				}
				*key = FN_KEY;
			}

			//zilla
			if (dcConfig & CONTROL_TO_DELETE)
			{
				if (controlHeldDown) // this event is a key up
				{
					controlHeldDown = 0;
					REMOVE(removeFlags, CONTROL_FLAG);
					*eventType = KEY_UP;
				}
				else // this event is a key down
				{
					controlHeldDown = 1;
					removeFlags |= CONTROL_FLAG;
					*eventType = KEY_DOWN;
				}
				*key = DELETE_KEY;
			}
		break; // end control key

		case FN_KEY: // begin fn key
			if (*flags & FN_FLAG)
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
				*key = CONTROL_KEY;
			}
		break; // end fn key

		//Addition by Sastira - sastira@gmail.com
		case FORWARD_DELETE:
		{
			if (dcConfig & SWAP_DELETE_AND_FORWARD_DELETE)
			{
				*flags = 0;
				*key = DELETE_KEY;
			}
		}
		break;
		//End addition by Sastira

		case DELETE_KEY: // begin delete key
			// Make Shift + Delete send a Forward Delete key
			if (dcConfig & SHIFT_DELETE_TO_FORWARD_DELETE)
			{
				if ( (*flags == SHIFT_FLAG) // with _only_ shift held as well
					|| (*flags == SHIFT_FLAG_R) // left OR right shift key
					|| (*flags == (SHIFT_FLAG + CAPSLOCK_FLAG)) // and caps lock is sticky,
					|| (*flags == (SHIFT_FLAG_R + CAPSLOCK_FLAG)) ) // so allow it also
				{
					*key = FORWARD_DELETE;
					// FN_FLAG is needed for Office
					*flags = FN_FLAG;
					*charCode = 45;
					*charSet = 254;
					*origCharCode = 45;
					*origCharSet = 254;
				}
			}

			//Addition by Sastira - sastira@gmail.com
			else if(dcConfig & SWAP_DELETE_AND_FORWARD_DELETE)
			{
				*key = FORWARD_DELETE;
				// FN_FLAG is needed for Office
				*flags = FN_FLAG;
				*charCode = 45;
				*charSet = 254;
				*origCharCode = 45;
				*origCharSet = 254;					
			}
			//End addition by Sastira

			//zilla
			else if (dcConfig & DELETE_TO_CONTROL)
			{
				if (*eventType == KEY_DOWN)
				{
					addFlags |= CONTROL_FLAG;
					*key = CONTROL_KEY;
					*eventType = KEY_MODIFY;
				}
				else if (*eventType == KEY_UP)
				{
					REMOVE(addFlags, CONTROL_FLAG);
					*key = CONTROL_KEY;
					*eventType = KEY_MODIFY;
				}
			}
		break; // end delete key

		// begin supplied by Giel Scharff <mgsch@mac.com>
		case NUMPAD_DOT: // begin numpad dot
			if (dcConfig & REVERSE_NUMPAD_DOT_AND_SHIFT_NUMPAD_DOT)
			{
				if (*eventType == KEY_DOWN && (*flags == 0x200000)) // key down without shift held as well
				{
					//key = 65;
					*flags = 0x220000;
				}
				else if (*eventType == KEY_DOWN && (*flags == 0x220000)) // key down with shift held as well
				{
					//key = 65;
					*flags = 0x200000;
				}
			}
		break; // end numpad dot
		// end supplied by Giel Scharff <mgsch@mac.com>

		case CAPSLOCK_KEY: // begin capslock key
	    case SPECIAL_CAPSLOCK_KEY:
			if(dcConfig & CAPSLOCK_TO_CONTROL)
			{
				removeFlags |= CAPSLOCK_FLAG;
				*key = CONTROL_KEY;
				//capslockHeldDown = 1;
				addFlags |= CONTROL_FLAG;
				if(*flags & CAPSLOCK_FLAG)
				{
					//flags ^= CAPSLOCK_FLAG;
					//flags |= CONTROL_FLAG;
					//capslockOn = 1;
				}
				else if(*keyboardType == POWERBOOKG3_KEYBOARD || *keyboardType == IBOOK_KEYBOARD)
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
	        if (dcConfig & CAPSLOCK_TO_DELETE)
	        {
	            return_value = kSupress;
	        }

			//Added by Sastira - sastira@gmail.com
			else if (dcConfig & CAPSLOCK_TO_FORWARD_DELETE)
			{
				return_value = kSupress;
			}
			//End Addition by Sastira

		break; // end capslock key

		case HOME_KEY: // begin home key
			if(dcConfig & PC_STYLE_HOME_AND_END)
			{
				if (*flags & CONTROL_FLAG)
				{
					removeFlags |= CONTROL_FLAG;
				}
				else if (*flags & CONTROL_FLAG_R)
				{
					removeFlags |= CONTROL_FLAG_R;
				}
				else
				{
					KeyToModifier(eventType, &addFlags, key,
						COMMAND_FLAG, LEFT_ARROW_KEY, *eventType);
					//*key = LEFT_ARROW_KEY;
					//*flags |= COMMAND_FLAG;
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
				if (*flags & CONTROL_FLAG)
				{
					removeFlags |= CONTROL_FLAG;
				}
				else if (*flags & CONTROL_FLAG_R)
				{
					removeFlags |= CONTROL_FLAG_R;
				}
				else
				{
					KeyToModifier(eventType, &addFlags, key,
						COMMAND_FLAG, RIGHT_ARROW_KEY, *eventType);
					//*key = RIGHT_ARROW_KEY;
					//*flags |= COMMAND_FLAG;
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
				if (*flags == 0x0
					|| *flags == OPTION_FLAG
					|| *flags == OPTION_FLAG_R)
				{
					*key = FORWARD_DELETE;
				}
			}
		break; // end backslash key

/* Uncomment this if doing this mod via behavior modification, rather than key
 * code translation.
		case TILDE_KEY: // begin tilde key
			if (dcConfig & SWAP_TILDE_AND_COMMAND)
			{
				if (tildeHeldDown) {
					tildeHeldDown = false;
					REMOVE(addFlags, COMMAND_FLAG);
				}
				else {
					tildeHeldDown = true;
					addFlags |= COMMAND_FLAG;
				}
				*key = COMMAND_KEY;
			}
			break; // end tilde key
*/

	} // end switch (key)

	// begin supplied by Giel Scharff <mgsch@mac.com>
	if (inFnMode)
	{
		if (*key == UP_ARROW_KEY)
		{
			*key = PAGE_UP_KEY;
		}
		else if (*key == DOWN_ARROW_KEY)
		{
			*key = PAGE_DOWN_KEY;
		}
		else if (*key == LEFT_ARROW_KEY)
		{
			*key = HOME_KEY;
		}
		else if (*key == RIGHT_ARROW_KEY)
		{
			*key = END_KEY;
		}
	}
	// end supplied by Giel Scharff <mgsch@mac.com>

	REMOVE(*flags, removeFlags);
	*flags |= addFlags;
	//printf("flags 0x%x, aflags 0x%x, rflags 0x%x\n", flags, addFlags, removeFlags);
#ifdef MB_DEBUG
	printf("sending hid event type %d flags 0x%x key %d ", *eventType, *flags, *key);
	printf("charCode %d charSet %d ", *charCode, *charSet);
	printf("origCharCode %d origCharSet %d kbdType %d ",
		*origCharCode, *origCharSet, *keyboardType);
	printf("repeat %d ts %d\n", *repeat, *ts);
#endif

	if (dcConfig & MB_DEBUG_OUTPUT)
	{
		printf("sending hid event type %d flags 0x%x key %d ", *eventType, *flags, *key);
		printf("charCode %d charSet %d ", *charCode, *charSet);
		printf("origCharCode %d origCharSet %d kbdType %d ",
			*origCharCode, *origCharSet, *keyboardType);
		printf("repeat %d ts %llud\n", *repeat, *ts);
	}
} // end if dcConfig != 0
//printf("returning %d\n", return_value);
return return_value;
}


//----------------------------------------------------------------------------
int special_remap(unsigned * eventType,
	unsigned * flags,
	unsigned * key,
	unsigned * flavor,
	UInt64 * guid,
	bool * repeat,
	AbsoluteTime * ts)
{
int return_value = kContinue;
//addFlags = 0;
//removeFlags = 0;

dcConfigArray[0] = dcConfig1;
dcConfigArray[1] = dcConfig2;
dcConfigArray[2] = dcConfig3;
dcConfigArray[3] = dcConfig4;
dcConfig = dcConfigAll;
int i = 0;
for (i = 0; i < 4; i++)
{
	dcConfig |= dcConfigArray[i];
}
//printf("final dcConfig = %d\n", dcConfig);

if (dcConfig != 0)
{
	if (dcConfig & MB_DEBUG_OUTPUT)
	{
		printf("caught  special event type %d flags 0x%x key %d ",
			*eventType, *flags, *key);
		printf("flavor %d guid %llud ", *flavor, *guid);
		printf("repeat %d ts %llud\n", *repeat, *ts);
	}

	if( (dcConfig & CAPSLOCK_DISABLED))
	{
		removeFlags |= CAPSLOCK_FLAG;
	}

	switch (*key)
	{
	    case CAPSLOCK_KEY:
	    case SPECIAL_CAPSLOCK_KEY:
	        if (dcConfig & CAPSLOCK_TO_DELETE)
	        {
	            return_value = kSwitch;
	            *key = DELETE_KEY;
	            removeFlags |= CAPSLOCK_FLAG;
	        }

            //Added by Sastira - sastira@gmail.com
            else if (dcConfig & CAPSLOCK_TO_FORWARD_DELETE)
            {							
                removeFlags |= CAPSLOCK_FLAG;
                *key = FORWARD_DELETE;
                return_value = kSwitch;
                
                // FN_FLAG is needed for Office
                *flags = FN_FLAG;
            }
            //End Addition by Sastira					

	    break;
	}

	REMOVE(*flags, removeFlags);
	*flags |= addFlags;

	if (dcConfig & MB_DEBUG_OUTPUT)
	{
		printf("sending special event type %d flags 0x%x key %d ",
			*eventType, *flags, *key);
		printf("flavor %d guid %llud ", *flavor, *guid);
		printf("repeat %d ts %llud\n", *repeat, *ts);
	}
}
//printf("returning %d\n", return_value);
return return_value;
}


//static keySeq inkeys[MAXREMAPS];
//static keySeq outkeys[MAXREMAPS];
int current_remapID = 0;
int current_sequenceNumber = 0;
int last_remapID = 0;
int last_sequenceNumber = 0;


//----------------------------------------------------------------------------
int KeyToModifier(unsigned * eventType,
			  unsigned * addFlags,
			  unsigned * key,
			  unsigned desired_flags,
			  unsigned desired_key,
			  unsigned desired_eventType)
{
	if (*eventType == KEY_DOWN)
	{
		*addFlags |= desired_flags;
		*key = desired_key;
		*eventType = desired_eventType;
	}
	else if (*eventType == KEY_UP)
	{
		REMOVE(*addFlags, desired_flags);
		*key = desired_key;
		*eventType = desired_eventType;
	}
	return 0;
}


/* The rest of these functions are unfinished and never yet used. */
//----------------------------------------------------------------------------
int ModifierToKey(unsigned * eventType,
			  unsigned * addFlags,
			  unsigned * removeFlags,
			  unsigned * key,
			  unsigned desired_flags,
			  unsigned desired_key,
			  unsigned desired_eventType)
{
	if (*addFlags & desired_flags)
	{
	}
	else
	{
	}
	return 0;
}


//----------------------------------------------------------------------------
int SetKeySeq(keySeq * seq,
			unsigned index,
			unsigned eventType,
			unsigned flags,
			unsigned key,
			unsigned charCode,
			unsigned charSet,
			unsigned origCharCode,
			unsigned origCharSet,
			unsigned keyboardType,
			bool repeat,
			int remapID,
			int sequenceNumber,
			bool allKeyboards)
{
	seq[index].eventType = eventType;
	seq[index].flags = flags;
	seq[index].key = key;
	seq[index].charCode = charCode;
	seq[index].charSet = charSet;
	seq[index].origCharCode = origCharCode;
	seq[index].origCharSet = origCharSet;
	seq[index].keyboardType = keyboardType;
	seq[index].repeat = repeat;
	seq[index].remapID = remapID;
	seq[index].sequenceNumber = sequenceNumber;
	seq[index].allKeyboards = allKeyboards;
	return 0;
}


//----------------------------------------------------------------------------
unsigned MatchID(keySeq * seq,
			unsigned eventType,
			unsigned flags,
			unsigned key,
			unsigned charCode,
			unsigned charSet,
			unsigned origCharCode,
			unsigned origCharSet,
			unsigned keyboardType,
			int * remapID,
			int * sequenceNumber)
{
	for (int index = 0; index < MAXREMAPS; index++)
	{
		if (seq[index].eventType == eventType
			&& seq[index].flags == flags
			&& seq[index].key == key
			&& seq[index].charCode == charCode
			&& seq[index].charSet == charSet
			&& seq[index].origCharCode == origCharCode
			&& seq[index].origCharSet == origCharSet
			&& (seq[index].keyboardType == keyboardType
				|| seq[index].allKeyboards == true)
			)
		{
			*remapID = seq[index].remapID;
			*sequenceNumber = seq[index].sequenceNumber;
			return 0;
		}
	}
	return 0;
}


//----------------------------------------------------------------------------
unsigned SwitchKeys(keySeq * seq,
				 unsigned * eventType,
				 unsigned * flags,
				 unsigned * key,
				 unsigned * charCode,
				 unsigned * charSet,
				 unsigned * origCharCode,
				 unsigned * origCharSet,
				 unsigned * keyboardType,
				 int remapID,
				 int sequenceNumber,
				 unsigned * addFlags,
				 unsigned * removeFlags)
{
	for (int index = 0; index < MAXREMAPS; index++)
	{
		if (seq[index].remapID == remapID
			&& seq[index].sequenceNumber == sequenceNumber)
		{
			// eventType is paired for most keys (up/down)
			switch (*eventType)
			{
				case (kKeyDown):
					*addFlags |= seq[index].flags;
					*removeFlags |= *flags;
				break;
				case (kKeyUp):
					*removeFlags |= seq[index].flags;
					*removeFlags |= *flags;
				break;
				case (kKeyModify):
					*addFlags |= seq[index].flags;
					*removeFlags |= *flags;
				break;
			}
			*eventType = seq[index].eventType;
			// flags is paired for modifier keys (on/off)
			//*flags = seq[index].flags;
			// key is what we use to match the others constant
			*key = seq[index].key;
			// these 4 are not really required
			/*
			*charCode = seq[index].charCode;
			*charSet = seq[index].charSet;
			*origCharCode = seq[index].origCharCode;
			*origCharSet = seq[index].origCharSet;
			*/
			return 0;
		}
	}
	return 0;
}

