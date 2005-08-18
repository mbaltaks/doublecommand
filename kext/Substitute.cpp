#include "DoubleCommand.h"
#include "MBHIDHack.h"
#include "MBDCC.h"

int remap(unsigned * eventType,
	unsigned * flags,
	unsigned * key,
	unsigned * charCode,
	unsigned * charSet,
	unsigned * origCharCode,
	unsigned * origCharSet,
	unsigned * keyboardType)
{
#ifdef MB_DEBUG
	printf("caught  hid event type %d flags 0x%x key %d ", *eventType, *flags, *key);
	printf("charCode %d charSet %d ", *charCode, *charSet);
	printf("origCharCode %d origCharSet %d kbdType %d\n",
		*origCharCode, *origCharSet, *keyboardType);
#endif

if (dcConfig != 0)
{
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
			}
			else if (dcConfig & ENTER_TO_CONTROL)
			{
                KeyToModifier(eventType, &addFlags, key,
                    CONTROL_FLAG, CONTROL_KEY, KEY_MODIFY);
			}
			else if (dcConfig & ENTER_TO_OPTION)
			{
                KeyToModifier(eventType, &addFlags, key,
                    OPTION_FLAG, OPTION_KEY, KEY_MODIFY);
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
		break; // end option key

		case CONTROL_KEY: // begin control key
		case CONTROL_KEY_R:
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

		case DELETE_KEY: // begin delete key
			// Make Shift + Delete send a Forward Delete key
			if (dcConfig & SHIFT_DELETE_TO_FORWARD_DELETE)
			{
				if (*flags == SHIFT_FLAG) // with _only_ shift held as well
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
					*key = LEFT_ARROW_KEY;
					*flags |= COMMAND_FLAG;
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
					*key = RIGHT_ARROW_KEY;
					*flags |= COMMAND_FLAG;
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
				if (*flags == 0x0)
				{
					*key = FORWARD_DELETE;
				}
			}
		break; // end backslash key

	} // end switch (key)

	// begin supplied by Giel Scharff <mgsch@mac.com>
	if (inFnMode)
	{   //up -> PgUp
		if ((*key == 126) /*&& (flags == 0x200000)*/ && (*charCode == 173) && (*charSet == 1) &&
	  (*origCharCode == 173) && (*origCharSet == 1))
		{
			*key = 116; *flags = 0x800000; *charCode =  48; *charSet = 254; *origCharCode =  48;  *origCharSet = 254;
		}
		//down -> PgDown
		else if ((*key == 125) /*&& (flags == 0x200000)*/ && (*charCode == 175) && (*charSet == 1) &&
		   (*origCharCode == 175) && (*origCharSet == 1))
		{
			*key = 121; *flags = 0x800000; *charCode =  49; *charSet = 254; *origCharCode =  49;  *origCharSet = 254;
		}
		//left -> Home
		else if ((*key == 123) /*&& (flags == 0x200000)*/ && (*charCode == 172) && (*charSet == 1) &&
		   (*origCharCode == 172) && (*origCharSet == 1))
		{
			*key = 115; *flags = 0x800000; *charCode =  46; *charSet = 254; *origCharCode =  46;  *origCharSet = 254;
		}
		//right -> End
		else if ((*key == 124) /*&& (flags == 0x200000)*/ && (*charCode == 174) && (*charSet == 1) &&
		   (*origCharCode == 174) && (*origCharSet == 1))
		{
			*key = 119; *flags = 0x800000; *charCode =  47; *charSet = 254; *origCharCode =  46;  *origCharSet = 254;
		}
	}
	// end supplied by Giel Scharff <mgsch@mac.com>

	removeFlags &= *flags;
	*flags ^= removeFlags;
	*flags |= addFlags;
	//printf("flags 0x%x, aflags 0x%x, rflags 0x%x\n", flags, addFlags, removeFlags);
#ifdef MB_DEBUG
	printf("sending hid event type %d flags 0x%x key %d ", *eventType, *flags, *key);
	printf("charCode %d charSet %d ", *charCode, *charSet);
	printf("origCharCode %d origCharSet %d kbdType %d\n",
		*origCharCode, *origCharSet, *keyboardType);
#endif
} // end if dcConfig != 0

return 0;
}
