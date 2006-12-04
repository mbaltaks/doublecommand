/*
*  KeyboardRemapper.cpp
*  DoubleCommand
*
*  Created by Mike Ter Louw on 4/8/06.
*
*/
#include <IOKit/IOReturn.h>
#include "KeyboardRemapper.h"
#include "DoubleCommand.h"
#include "Defines.h"

#define MKT_DEBUG 0

extern int dcConfig;
extern dc_keyboard Keyboards[MAX_KEYBOARDS];


#define DEBUGGING (MKT_DEBUG || (dcConfig & MB_DEBUG_OUTPUT))



// Begin KeyboardMapper class definition.
OSDefineMetaClassAndStructors(KeyboardRemapper, IOHIKeyboardMapper)


// Attach the key map prefilter to a keyboard.
bool KeyboardRemapper::__dc_attach(IOHIKeyboard *kbd) {

	__dc_keyboard = NULL;
	__dc_keyMap = NULL;
	__dc_keyMods = NULL;
	__dc_next = NULL;

	// Make sure we have a keyboard with a key mapper.
	if (!kbd) return false;
	if (!kbd->_keyMap) return false;

	__dc_keyboard = kbd;
	__dc_keyMap = kbd->_keyMap;

	/* The ordering of statements below is very deliberate. */

	// Make sure the original key mapper sticks around.
	__dc_keyMap->retain();

	// Attach ourselves to the keyboard.
	kbd->_keyMap = this;

	// Have the keyboard register our mapper.
	kbd->updateProperties();

	IOLog("DoubleCommand: Attached filter to keyboard %d.\n", which_keyboard(__dc_keyboard));
	return true;
}


// Check to see if we're still attached to a keyboard.
bool KeyboardRemapper::__dc_attached() {
	if (!__dc_keyboard || !__dc_keyMap) return false;
	return true;
}


// Restore the original keyboard mapper.
void KeyboardRemapper::__dc_detach() {

	// Make sure we were actually initialized.
	if (!__dc_attached()) return;

	__dc_restoreAllKeys();

	/* The ordering of statements below is very deliberate. */

	// Make sure it is us who is attached to the keyboard.
	if (__dc_keyboard->_keyMap && (__dc_keyboard->_keyMap == this)) {

		// Restore the original key mapper.
		__dc_keyboard->_keyMap = __dc_keyMap;

		// Have the keyboard register the original mapper.
		__dc_keyboard->updateProperties();

		IOLog("DoubleCommand: Detached filter from keyboard %d.\n",
			which_keyboard(__dc_keyboard));
	}

	// Release our reference to the original mapper.
	__dc_keyMap->release();

	__dc_keyboard = NULL;
	__dc_keyMap = NULL;
}


// Find the preserved flags for a given key code.
KeyUndo **KeyboardRemapper::__dc_findKeyMod(UInt8 keyCode) {

	KeyUndo *undo, **pointer;

	for (pointer = &__dc_keyMods; undo = *pointer; pointer = &(undo->next)) {
		if (undo->keyCode == keyCode) return pointer;
	}
	return NULL;
}


// Get the original key flags/bits for a given key.
unsigned char KeyboardRemapper::__dc_getOriginalKeyBits(UInt8 keyCode) {

	KeyUndo **pointer = __dc_findKeyMod(keyCode);
	if (pointer) {
		KeyUndo *undo = *pointer;
		return undo->bits;
	} else {
		return __dc_keyMap->_parsedMapping.keyBits[keyCode];
	}
}


// Get the original key mapping for a given key.
unsigned char *KeyboardRemapper::__dc_getOriginalKeyMapping(UInt8 keyCode) {

	KeyUndo **pointer = __dc_findKeyMod(keyCode);
	if (pointer) {
		KeyUndo *undo = *pointer;
		return undo->mapping;
	} else {
		return __dc_keyMap->_parsedMapping.keyDefs[keyCode];
	}
}


// Modify a key in one or more ways.
bool KeyboardRemapper::__dc_modifyKey(UInt8 keyCode, int type, UInt8 likeKeyCode) {

	// Do some initial processing based on the modification type.
	if (!(type & DC_ALL_MODS)) return false;

	if (type & DC_RESTORE) {
		__dc_restoreKey(keyCode, DC_ALL_MODS);
		return true;
	}

	if (type & (DC_TRANSLATE_TO | DC_BEHAVE_LIKE)) {
		if (likeKeyCode >= NX_NUMKEYCODES) return false;
	}


	// Record the undo information for the modified key code, and configure the mod.
	KeyUndo *undo = __dc_newKeyMod(keyCode);
	if (!undo) return false;

	if (type & DC_TRANSLATE_TO) {
		undo->type |= DC_TRANSLATE;
		undo->likeKeyCode = likeKeyCode;
	}

	if (type & DC_BEHAVE_LIKE) {
		undo->type |= DC_BEHAVIOR;
		__dc_keyMap->_parsedMapping.keyBits[keyCode] = __dc_getOriginalKeyBits(likeKeyCode);
		__dc_keyMap->_parsedMapping.keyDefs[keyCode] = __dc_getOriginalKeyMapping(likeKeyCode);
	}

	if (type & DC_SUPPRESS) {
		undo->type |= DC_SUPPRESS;
	}

	if (DEBUGGING) IOLog("Modified key.  key: %d/0x%x, like: %d/0x%x, type: %d, keyboard: %d\n",
		keyCode, keyCode, likeKeyCode, likeKeyCode, type, which_keyboard(__dc_keyboard)
	);

	return true;
}


// Make one key behave similarly to another key.
KeyUndo *KeyboardRemapper::__dc_newKeyMod(UInt8 keyCode) {

	if (!__dc_attached()) return NULL;
	if (keyCode >= NX_NUMKEYCODES) return NULL;

	KeyUndo **pointer = __dc_findKeyMod(keyCode);
	KeyUndo *undo;

	if (pointer) undo = *pointer;
	else {
		undo = (KeyUndo *) IOMalloc(sizeof(KeyUndo));

		undo->type = DC_DEFAULT;
		undo->keyCode = keyCode;

		// Remember these values in case the key behavior gets modified.
		undo->bits = __dc_keyMap->_parsedMapping.keyBits[keyCode];
		undo->mapping = __dc_keyMap->_parsedMapping.keyDefs[keyCode];

		undo->next = __dc_keyMods;
		__dc_keyMods = undo;
	}

	return undo;
}


// Restore a single key's behavior.
void KeyboardRemapper::__dc_restoreKey(UInt8 keyCode, int type) {

	if (!__dc_attached()) return;

	// Find the key mod.
	KeyUndo **pointer = __dc_findKeyMod(keyCode);
	if (!pointer) return;

	// Restore the key mod.
	KeyUndo *undo = *pointer;
	__dc_restoreKey(undo, type);

	// Remove from our list of mods if the key is back to the default state.
	if (undo->type == DC_DEFAULT) {

		if (DEBUGGING) IOLog("Freeing undo data for key %d on keyboard %d.\n",
			keyCode, which_keyboard(__dc_keyboard));

		*pointer = undo->next;
		IOFree(undo, sizeof(KeyUndo));;
	}

	return;
}


// Restore a single key's behavior (internal helper function, doesn't unlink saved settings).
void KeyboardRemapper::__dc_restoreKey(KeyUndo *undo, int type) {

	// Figure out what we need to restore.
	int restore = undo->type & type;

	// Restore a key translation.
	if (restore & DC_TRANSLATE) {
		undo->type ^= DC_TRANSLATE;
	}

	// Restore a behavior modification.
	if (restore & DC_BEHAVIOR) {
		__dc_keyMap->_parsedMapping.keyBits[undo->keyCode] = undo->bits;
		__dc_keyMap->_parsedMapping.keyDefs[undo->keyCode] = undo->mapping;
		undo->type ^= DC_BEHAVIOR;
	}

	// Restore a key suppression.
	if (restore & DC_SUPPRESS) {
		undo->type ^= DC_SUPPRESS;
	}

	if (DEBUGGING) IOLog("Restored key.  key: %d/0x%x, types: 0x%x, keyboard: %d\n",
		undo->keyCode, undo->keyCode, restore, which_keyboard(__dc_keyboard)
	);
}


// Restore the entire key map to original settings.
void KeyboardRemapper::__dc_restoreAllKeys(int type) {

	if (DEBUGGING) IOLog("Restoring all key modifications...  type: 0x%x, keyboard: %d.\n",
		which_keyboard(__dc_keyboard));

	KeyUndo **pointer = &__dc_keyMods;
	KeyUndo *undo;

	while (undo = *pointer) {
		__dc_restoreKey(undo, type);

		// If the key has been fully restored, delete the undo data.
		if (undo->type == DC_DEFAULT) {

			if (DEBUGGING) IOLog("Freeing undo data for key %d on keyboard %d.\n",
				undo->keyCode, which_keyboard(__dc_keyboard));

			*pointer = undo->next;
			IOFree(undo, sizeof(KeyUndo));
		}
		else pointer = &(undo->next);
	}
}


// Verify that our stage-two filter is engaged.
bool KeyboardRemapper::__dc_verifyStageTwo() {

	if (!__dc_keyboard) return false;
	if (!__dc_keyboard->_keyboardEventAction) return false;

	// Verify that our even actions are in place.
	if (__dc_keyboard->_keyboardEventAction != event) {

		// Find the keyboard structure pertaining to this keyboard.
		for (int i = 0; i < MAX_KEYBOARDS; i++) {
			if (Keyboards[i].keyboard != __dc_keyboard) continue;

			// Re-hook the events.
			Keyboards[i].event = __dc_keyboard->_keyboardEventAction;
			Keyboards[i].special_event = __dc_keyboard->_keyboardSpecialEventAction;
			__dc_keyboard->_keyboardEventAction = event;
			__dc_keyboard->_keyboardSpecialEventAction = specialEvent;
		}
	}
	return true;
}


//-----------------------------------------------------------------------------
// Wrappers For IOHIKeyboardMapper Inherited Methods.
//-----------------------------------------------------------------------------


UInt8 KeyboardRemapper::getParsedSpecialKey(UInt8 logical) {
//	if (DEBUGGING) IOLog("Wrapper: getParsedSpecialKey()\n");
	if (__dc_keyMap) return __dc_keyMap->getParsedSpecialKey(logical);
	return 0;
}


bool KeyboardRemapper::init(IOHIKeyboard *delegate, const UInt8 *mapping, UInt32 mappingLength, bool mappingShouldBeFreed) {
	if (DEBUGGING) IOLog("Wrapper: init()\n");
	if (__dc_keyMap) return __dc_keyMap->init(delegate, mapping, mappingLength, mappingShouldBeFreed);
	return false;
}


void KeyboardRemapper::keyEventPostProcess(void) {
//	if (DEBUGGING) IOLog("Wrapper: keyEventPostProcess()\n");
	if (__dc_keyMap) __dc_keyMap->keyEventPostProcess();
}


const UInt8 *KeyboardRemapper::mapping() {
	if (DEBUGGING) IOLog("Wrapper: mapping()\n");
	if (__dc_keyMap) return __dc_keyMap->mapping();
	return NULL;
}


UInt32 KeyboardRemapper::mappingLength() {
	if (DEBUGGING) IOLog("Wrapper: mappingLength()\n");
	if (__dc_keyMap) return __dc_keyMap->mappingLength();
	return 0;
}


IOReturn KeyboardRemapper::message(UInt32 type, IOService *provider, void *argument) {
	if (DEBUGGING) IOLog("Wrapper: message()\n");
	if (__dc_keyMap) return __dc_keyMap->message(type, provider, argument);
	return kIOReturnSuccess;
}


bool KeyboardRemapper::serialize(OSSerialize *s) const {
	if (DEBUGGING) IOLog("Wrapper: serialize()\n");
	if (__dc_keyMap) return __dc_keyMap->serialize(s);
	return false;
}


void KeyboardRemapper::setKeyboardTarget(IOService *keyboardTarget) {
	if (DEBUGGING) IOLog("Wrapper: setKeyboardTarget()\n");
	if (__dc_keyMap) __dc_keyMap->setKeyboardTarget(keyboardTarget);
}


IOReturn KeyboardRemapper::setParamProperties(OSDictionary * dict) {
//	if (DEBUGGING) IOLog("Wrapper: setParamProperties()\n");
	if (__dc_keyMap) return __dc_keyMap->setParamProperties(dict);
	return kIOReturnSuccess;
}


bool KeyboardRemapper::updateProperties(void) {
	if (DEBUGGING) IOLog("Wrapper: updateProperties()\n");
	if (__dc_keyMap) return __dc_keyMap->updateProperties();
	return false;
}


// Intercept for key code translate method.
void KeyboardRemapper::translateKeyCode(UInt8 keyCode, bool keyDown, kbdBitVector keyBits) {

	// Verify that we are attached to a keyboard.
	if (!__dc_keyMap) return;

	// If our stage-two filter is not attached, don't modify the event.
	if (!__dc_verifyStageTwo()) {
		__dc_keyMap->translateKeyCode(keyCode, keyDown, keyBits);
		return;
	}

	if (DEBUGGING) IOLog("Filtering: In.... keyCode: %d:%x, keyDown: %d, keyBits: %x, repeat %d\n",
		keyCode, keyCode, keyDown, *keyBits, __dc_keyboard->_codeToRepeat);

	UInt8 remapKeyCode = keyCode;
	bool translating = false;

	// Check to see if we're modifying the key.
	KeyUndo **pointer = __dc_findKeyMod(keyCode);
	KeyUndo *undo = NULL;

	if (pointer) {
		undo = *pointer;

		// Check for key suppression.
		if (undo->type & DC_SUPPRESS) return;

		// Check for key translation.
		if (undo->type & DC_TRANSLATE) {

			translating = true;
			remapKeyCode = undo->likeKeyCode;

			// Trick the IOHIKeyboard into cancelling a repeat event.
			if (!keyDown && (__dc_keyboard->_codeToRepeat == keyCode)) {
				__dc_keyboard->_codeToRepeat = undo->likeKeyCode;
			}
		}
	}

	if (DEBUGGING) IOLog("Filtering: Out... keyCode: %d:%x\n", remapKeyCode, remapKeyCode);

	// Pass the (possibly) translated key code to the real key mapper.
	__dc_keyMap->translateKeyCode(remapKeyCode, keyDown, keyBits);

	// Trick the keyboard into repeating the untranslated key code.
	if (translating && keyDown && (__dc_keyboard->_codeToRepeat == undo->likeKeyCode)) {
		__dc_keyboard->_codeToRepeat = keyCode;
	}
}


//-----------------------------------------------------------------------------
// Wrappers For Standard OSObject Methods.
//-----------------------------------------------------------------------------


// Detach our filter before freeing.
void KeyboardRemapper::free() {
	__dc_detach();
	OSObject::free();
}


