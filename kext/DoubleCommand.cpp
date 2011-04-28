/*
 * Thanks to the fKeys project (http://www.kodachi.com/fKeys/)
 * for the IOKit technique used in this file.
 */

#include <IOKit/IOLib.h>
#include "DoubleCommand.h"
#include "Boundary.h"
//#include "KeyBehaviorManager.h"

extern "C"
{
//This is for debugging purposes ONLY
#include <pexpert/pexpert.h>
}

// Sole instance of the behavior manager.
//KeyBehaviorManager keyBehaviorManager;

// Define my superclass
#define super IOService
// REQUIRED! This macro defines the class's constructors, destructors,
// and several other methods I/O Kit requires. Do NOT use super as the
// second parameter. You must use the literal name of the superclass.
OSDefineMetaClassAndStructors(com_baltaks_driver_DoubleCommand, IOService)


dc_keyboard Keyboards[MAX_KEYBOARDS];

bool com_baltaks_driver_DoubleCommand::init(OSDictionary *dict)
{
	bool res = super::init(dict);
	//IOLog("Initializing DoubleCommand\n");
	sysctl_register_oid(&sysctl__dc);
	sysctl_register_oid(&sysctl__dc_config);
	sysctl_register_oid(&sysctl__dc_config1);
	sysctl_register_oid(&sysctl__dc_config2);
	sysctl_register_oid(&sysctl__dc_config3);
	sysctl_register_oid(&sysctl__dc_config4);
	sysctl_register_oid(&sysctl__dc_keyboard1);
	sysctl_register_oid(&sysctl__dc_keyboard2);
	sysctl_register_oid(&sysctl__dc_keyboard3);
	sysctl_register_oid(&sysctl__dc_keyboard4);
	for (int i = 0; i < MAX_KEYBOARDS; i++)
	{
		Keyboards[i].keyboard = NULL;
	}
	return res;
}

void com_baltaks_driver_DoubleCommand::free(void)
{
	sysctl_unregister_oid(&sysctl__dc);
	sysctl_unregister_oid(&sysctl__dc_config);
	sysctl_unregister_oid(&sysctl__dc_config1);
	sysctl_unregister_oid(&sysctl__dc_config2);
	sysctl_unregister_oid(&sysctl__dc_config3);
	sysctl_unregister_oid(&sysctl__dc_config4);
	sysctl_unregister_oid(&sysctl__dc_keyboard1);
	sysctl_unregister_oid(&sysctl__dc_keyboard2);
	sysctl_unregister_oid(&sysctl__dc_keyboard3);
	sysctl_unregister_oid(&sysctl__dc_keyboard4);
	//IOLog("Freeing DoubleCommand\n");
	super::free();
}

IOService *com_baltaks_driver_DoubleCommand::probe(IOService *provider, SInt32 *score)
{
	IOService *res = super::probe(provider, score);
	//IOLog("DoubleCommand Probing\n");
	return res;
}

bool com_baltaks_driver_DoubleCommand::start(IOService *provider)
{
	bool res = false;
	// Keep track of the super class, if we fail after starting this, stop it.
	bool superclass_started = false;
	res = super::start(provider);
	if (!res) { return res; }
	superclass_started = true;
	IOLog("Starting DoubleCommand\n");

#if __LP64__
	keyboard_notifier = addMatchingNotification(gIOMatchedNotification,
		serviceMatching("IOHIKeyboard"),
		((IOServiceMatchingNotificationHandler) &dc_matched),
		this, NULL, 0);
#else
	keyboard_notifier = addNotification(gIOMatchedNotification,
		serviceMatching("IOHIKeyboard"),
		((IOServiceNotificationHandler) &dc_matched),
		this, NULL, 0);
#endif

	if (keyboard_notifier == NULL)
	{
		IOLog("DoubleCommand could not add match notifier");
		if (superclass_started) { super::stop(provider); }
		return false;
	}

#if __LP64__
	terminated_notifier = addMatchingNotification(gIOTerminatedNotification,
		serviceMatching("IOHIKeyboard"),
		((IOServiceMatchingNotificationHandler) &dc_terminated),
		this, NULL, 0);
#else
	terminated_notifier = addNotification(gIOTerminatedNotification,
		serviceMatching("IOHIKeyboard"),
		((IOServiceNotificationHandler) &dc_terminated),
		this, NULL, 0);
#endif

	if (terminated_notifier == NULL)
	{
		IOLog("DoubleCommand could not add terminate notifier");
		if (superclass_started) { super::stop(provider); }
		return false;
	}

	return res;
}

void com_baltaks_driver_DoubleCommand::stop(IOService *provider)
{
	for (int i = 0; i < MAX_KEYBOARDS; i++)
	{
		if (Keyboards[i].keyboard != NULL)
		{
			return_keyboard(Keyboards[i].keyboard);
		}
	}

	if (keyboard_notifier)
	{
		keyboard_notifier->remove();
	}
	if (terminated_notifier)
	{
		terminated_notifier->remove();
	}

	// Shut down the keyboard manager.
	//keyBehaviorManager.cleanup();

	IOLog("Stopping DoubleCommand\n");
	super::stop(provider);
}

#if __LP64__
bool dc_matched(com_baltaks_driver_DoubleCommand * self,
	void * ref,
	IOService * serv,
	IONotifier * notifier)
#else
bool dc_matched(com_baltaks_driver_DoubleCommand * self,
	void * ref,
	IOService * serv)
#endif
{
	IOHIKeyboard * kbd = NULL;
	kbd = OSDynamicCast(IOHIKeyboard, serv);
	hijack_keyboard(kbd);
	return true;
}

#if __LP64__
bool dc_terminated(com_baltaks_driver_DoubleCommand * self,
	void * ref,
	IOService * serv,
    IONotifier * notifier)
#else
bool dc_terminated(com_baltaks_driver_DoubleCommand * self,
	void * ref,
	IOService * serv)
#endif
{
	//IOLog("DoubleCommand unloading keyboard\n");
	IOHIKeyboard * kbd = NULL;
	kbd = OSDynamicCast(IOHIKeyboard, serv);
	return_keyboard(kbd);
	return true;
}

/* Do I really need these? Do they even make sense? */
unsigned GcharCode = 0;
unsigned GcharSet = 0;
unsigned GorigCharCode = 0;
unsigned GorigCharSet = 0;
unsigned GkeyboardType = 0;
unsigned Gflavor = 0;
UInt64 Gguid = 0;

void
event(OSObject *target,
	unsigned   eventType,
	unsigned   flags,
	unsigned   key,
	unsigned   charCode,
	unsigned   charSet,
	unsigned   origCharCode,
	unsigned   origCharSet,
	unsigned   keyboardType,
	bool	   repeat,
	AbsoluteTime ts)
{
#ifdef MB_DEBUG
	IOLog("I found a keypress\n");
	IOLog("eventtype %d flags %d key %d keyboardtype %d\n", 
		eventType, flags, key, keyboardType);
#endif
	int r = 0;

	int i = find_client(false);
	if (i >= 0)
	{
		GcharCode = charCode;
		GcharSet = charSet;
		GorigCharCode = origCharCode;
		GorigCharSet = origCharSet;
		GkeyboardType = keyboardType;

		r = remap(&eventType, &flags, &key, &charCode, &charSet,
			&origCharCode, &origCharSet, &keyboardType, &repeat, &ts);
		if (r == kContinue)
		{
			Keyboards[i].event(target, eventType, flags, key, charCode,
				charSet, origCharCode, origCharSet, keyboardType, repeat, ts);
		}
		else if (r == kSwitch)
		{
#ifdef MB_DEBUG
			IOLog("sending special event type %d flags 0x%x key %d ",
				eventType, flags, key);
			IOLog("flavor %d guid %d ", Gflavor, Gguid);
			IOLog("repeat %d ts %d\n", repeat, ts);
#endif
// potentially here I could keep track of what keypress happened, and put
// in some repeats until I see the key up.
			Keyboards[i].special_event(target, eventType, flags, key, Gflavor,
				Gguid, repeat, ts);
		}
	}
}

void
specialEvent(OSObject * target,
	unsigned   eventType,
	unsigned   flags,
	unsigned   key,
	unsigned   flavor,
	UInt64	 guid,
	bool	   repeat,
	AbsoluteTime ts)
{
#ifdef MB_DEBUG
	IOLog("I found a special keypress\n");
	IOLog("eventtype %d flags %d key %d flavor %d\n", eventType, flags, key,
		flavor);
#endif
	int r = 0;

	int i = find_client(true);
	if (i >= 0) {
		Gflavor = flavor;
		Gguid = guid;

		r = special_remap(&eventType, &flags, &key,
			&flavor, &guid, &repeat, &ts);
		if (r == kContinue)
		{
			Keyboards[i].special_event(target, eventType, flags, key, flavor,
				guid, repeat, ts);
		}
		else if (r == kSwitch)
		{
#ifdef MB_DEBUG
			IOLog("sending hid event type %d flags 0x%x key %d ", eventType,
				flags, key);
			IOLog("charCode %d charSet %d ", GcharCode, GcharSet);
			IOLog("origCharCode %d origCharSet %d kbdType %d ",
				GorigCharCode, GorigCharSet, GkeyboardType);
			IOLog("repeat %d ts %d\n", repeat, ts);
#endif
			Keyboards[i].event(target, eventType, flags, key, GcharCode,
				GcharSet, GorigCharCode, GorigCharSet, GkeyboardType, repeat, ts);
		}
	}
}


int hijack_keyboard(IOHIKeyboard * kbd)
{
	bool hijack = false;
	const char * name;

	if (kbd == NULL)
	{
		IOLog("no keyboard!\n");
	}
	name = kbd->getName();

	// we only want to hijack certain things we're matched to.
	if (strcmp(name, "AppleADBKeyboard") == 0)
	{
		hijack = true;
	}
	else if (strcmp(name, "AppleADBButtons") == 0)
	{
		// do nothing
	}
	else if (strcmp(name, "IOHIDKeyboard") == 0)
	{
		hijack = true;
	}
	else if (strcmp(name, "IOHIDConsumer") == 0)
	{
		// do nothing
	}
	else if (strcmp(name, "ApplePS2Keyboard") == 0)
	{
		hijack = true;
	}
	else if (strcmp(name, "MicrosoftKeyboardUSB") == 0)
	{
		hijack = true;
	}
	else
	{
		IOLog("DoubleCommand does not support %s\n", name);
	}

	// if we haven't found something we can hijack, stop here.
	if (!hijack)
	{
#ifdef MB_DEBUG
		IOLog("DoubleCommand did not match %s\n", name);
#endif
		return 0;
	}

	for (int i = 0; i < MAX_KEYBOARDS; i++)
	{
		if (Keyboards[i].keyboard == NULL)
		{
			IOLog("Hijacking keyboard %d (%s)\n", i, kbd->getName());
			Keyboards[i].keyboard = kbd;
				Keyboards[i].event = kbd->_keyboardEventAction;
			Keyboards[i].special_event = kbd->_keyboardSpecialEventAction;

			if (!kbd->_keyboardEventAction)
			{
				IOLog("keyboard not ready?\n");
			}
			if (!kbd->_keyboardSpecialEventAction)
			{
				IOLog("keyboard not ready?\n");
			}

#ifndef MB_TESTING
			kbd->_keyboardEventAction = event;
			kbd->_keyboardSpecialEventAction = specialEvent;

			// Register the keyboard with the behavior manager.
			//if (!keyBehaviorManager.addKeyboard(kbd))
			//	IOLog("Unable to attach stage one filter.\n");

#endif

#if __LP64__
			IOLog("kea %08lX: ksea %08lX\n",
				(unsigned long) Keyboards[i].event,
				(unsigned long) Keyboards[i].special_event);
			IOLog("modded kea %08lX: modded ksea %08lX\n",
				(unsigned long) kbd->_keyboardEventAction,
				(unsigned long) kbd->_keyboardSpecialEventAction);
#else
			IOLog("kea %08X: ksea %08X\n",
				(unsigned long) Keyboards[i].event,
				(unsigned long) Keyboards[i].special_event);
			IOLog("modded kea %08X: modded ksea %08X\n",
				(unsigned long) kbd->_keyboardEventAction,
				(unsigned long) kbd->_keyboardSpecialEventAction);
#endif
			break;
		}
	}
	return 0;
}

int return_keyboard(IOHIKeyboard * kbd)
{
	if (kbd == NULL)
	{
		IOLog("no keyboard!\n");
	}

	for (int i = 0; i < MAX_KEYBOARDS; i++)
	{
		if (Keyboards[i].keyboard == kbd)
		{
			IOLog("Returning keyboard %d (%s)\n", i, kbd->getName());

			if (kbd->_keyboardEventAction == event)
			{
#ifndef MB_TESTING
				kbd->_keyboardEventAction = Keyboards[i].event;
#endif
			}
			else
			{
				IOLog("keyboard not ready?\n");
			}
			if (kbd->_keyboardSpecialEventAction == specialEvent)
			{
#ifndef MB_TESTING
				kbd->_keyboardSpecialEventAction = Keyboards[i].special_event;
#endif
			}
			else
			{
				IOLog("keyboard not ready?\n");
			}

			// Remove the keyboard from the behavior manager.
			//keyBehaviorManager.removeKeyboard(kbd);

#if __LP64__
			IOLog("hijacked values kea %08lX: ksea %08lX\n",
				(unsigned long) kbd->_keyboardEventAction,
				(unsigned long) kbd->_keyboardSpecialEventAction);
			IOLog("returned kea %08lX: modded ksea %08lX\n",
				(unsigned long) kbd->_keyboardEventAction,
				(unsigned long) kbd->_keyboardSpecialEventAction);
#else
			IOLog("hijacked values kea %08X: ksea %08X\n",
				(unsigned long) kbd->_keyboardEventAction,
				(unsigned long) kbd->_keyboardSpecialEventAction);
			IOLog("returned kea %08X: modded ksea %08X\n",
				(unsigned long) kbd->_keyboardEventAction,
				(unsigned long) kbd->_keyboardSpecialEventAction);
#endif
			Keyboards[i].keyboard = NULL;
			break;
		}
	}
	return 0;
}


// Get the index of the given keyboard.  Useful for diagnostic messages, etc..
int which_keyboard(IOHIKeyboard * kbd) {

	if (kbd) {
		for (int i = 0; i < MAX_KEYBOARDS; i++) {
			if (Keyboards[i].keyboard == kbd) return i;
		}
	}
	return -1;
}


/**
 * Find a client to send events to.  This isn't always so straightforward,
 * since on log out the OS tends to close our device, add a new device,
 * and/or other funny business.  If we've got this far, the stage 1 filter
 * should have ensured that there is at least one client to be found.
 */
int find_client(bool special) {

	// At least one of these has to match.
	for (int i = 0; i < MAX_KEYBOARDS; i++) {

		IOHIKeyboard *keyboard = Keyboards[i].keyboard;

		if (!keyboard) continue;

		if (!special && (keyboard->_keyboardEventAction == event))
			return i;

		if (special && (keyboard->_keyboardSpecialEventAction == specialEvent))
			return i;
	}
	return -1;
}
