/*
 *  KeyBehaviorManager.cpp
 *  DoubleCommand
 *
 *  Created by Mike Ter Louw on 4/9/06.
 *
 */
#include "KeyBehaviorManager.h"
#include "Defines.h"

#define MKT_DEBUG 0

extern int dcConfig;

#define SETTING_ON(x) (!(prevConfig & (x)) && (config & (x)))
#define SETTING_OFF(x) ((prevConfig & (x)) && !(config & (x)))
#define DEBUGGING (MKT_DEBUG || (config & MB_DEBUG_OUTPUT))

//----------------------------------------------------------------------------
// Key behavior configuration method.
//----------------------------------------------------------------------------


// Check for and apply changes in the configuration.
void KeyBehaviorManager::applyConfig() {

	if (config == dcConfig) return;
	int prevConfig = config;
	config = dcConfig;

	if (DEBUGGING) {
	IOLog("DoubleCommand: Configuration changed.  Setting up filters.\n");
	}

	// Apply effects of the tile-command swap.
	if (SETTING_ON(SWAP_TILDE_AND_COMMAND)) {
		modifyKey(TILDE_KEY, DC_TRANSLATE_TO, COMMAND_KEY);
		modifyKey(COMMAND_KEY, DC_TRANSLATE_TO, TILDE_KEY);
	}

	// Remove effects of the tile-command swap.
	else if (SETTING_OFF(SWAP_TILDE_AND_COMMAND)) {
		restoreKey(TILDE_KEY, DC_TRANSLATE);
		restoreKey(COMMAND_KEY, DC_TRANSLATE);
	}

/*
	// Apply effects of some other setting.
	if (SETTING_ON(SOME_OTHER_SETTING)) {

		// Some possiblilities...
		modifyKey(KEY_ONE, DC_TRANSLATE_TO, KEY_TWO);
		modifyKey(KEY_THREE, DC_BEHAVE_LIKE, KEY_FOUR);
		modifyKey(KEY_FIVE, DC_SUPPRESS);

	}

	// Remove effects of some other setting.
	else if (SETTING_OFF(SOME_OTHER_SETTING)) {

		// Some possiblilities...
		restoreKey(KEY_ONE, DC_TRANSLATE);
		restoreKey(KEY_THREE, DC_BEHAVIOR);
		restoreKey(KEY_FIVE, DC_SUPPRESS);

		// ...or to completely restore them:
		restoreKey(KEY_ONE);
		restoreKey(KEY_THREE);
		restoreKey(KEY_FIVE);
	}
*/

}


//----------------------------------------------------------------------------
// Remainder of the KeyBehaviorManager class.
//----------------------------------------------------------------------------


// Constructor.
KeyBehaviorManager::KeyBehaviorManager() {
	config = 0;
	keyboards = NULL;
	keyMods = NULL;
}


// Manage key behavior modifications for a new keyboard.
bool KeyBehaviorManager::addKeyboard(IOHIKeyboard *keyboard) {

	if (!keyboard) return false;

	// Make sure we aren't already attached to the keyboard.
	KeyboardRemapper **pointer = findKeyboard(keyboard);
	if (pointer) return false;

	if (DEBUGGING) IOLog("Adding keyboard: %s\n", keyboard->getName());

	// Create the new keyboard remapper and then attach it to the keyboard.
	KeyboardRemapper *remapper = new KeyboardRemapper();
	if (!remapper->__dc_attach(keyboard)) {
		remapper->release();
		return false;
	}

	// Apply our current settings to the remapper.
	for (KeyRedo *redo = keyMods; redo; redo = redo->next) {

		if (redo->type & DC_TRANSLATE) remapper->
			__dc_modifyKey(redo->keyCode, DC_TRANSLATE_TO, redo->translateToKeyCode);

		if (redo->type & DC_BEHAVIOR) remapper->
			__dc_modifyKey(redo->keyCode, DC_BEHAVE_LIKE, redo->behaveLikeKeyCode);

		if (redo->type & DC_SUPPRESS) remapper->
			__dc_modifyKey(redo->keyCode, DC_SUPPRESS);
	}

	remapper->__dc_next = keyboards;
	keyboards = remapper;
	return true;
}


// Restore the keyboard manager to its initial state, freeing all memory.
void KeyBehaviorManager::cleanup() {
	removeAllKeyboards();
	restoreAllKeys();
}


// Find a keyboard in our list of managed keyboards.
KeyboardRemapper **KeyBehaviorManager::findKeyboard(IOHIKeyboard *keyboard) {

	KeyboardRemapper *remapper, **pointer;

	for (pointer = &keyboards; remapper = *pointer; pointer = &(remapper->__dc_next)) {
		if (remapper->__dc_keyboard == keyboard) return pointer;
	}
	return NULL;
}


// Find a key behavior modification in our list of mods.
KeyRedo **KeyBehaviorManager::findKeyMod(UInt8 keyCode) {

	KeyRedo *redo, **pointer;

	for (pointer = &keyMods; redo = *pointer; pointer = &(redo->next)) {
		if (redo->keyCode == keyCode) return pointer;
	}
	return NULL;
}


// Get the currently active settings.
int KeyBehaviorManager::getCurrentConfig() {
	return config;
}


// Make one key act like another key, across all managed keyboards.
bool KeyBehaviorManager::modifyKey(UInt8 keyCode, int type, UInt8 likeKeyCode) {

	for (KeyboardRemapper *remapper = keyboards; remapper; remapper = remapper->__dc_next) {
		if (!remapper->__dc_modifyKey(keyCode, type, likeKeyCode)) {
			restoreKey(keyCode, type);
			return false;
		}
	}

	KeyRedo **pointer = findKeyMod(keyCode);
	KeyRedo *redo;

	if (pointer) redo = *pointer;
	else {
		redo = (KeyRedo *) IOMalloc(sizeof(KeyRedo));
		redo->type = 0;
		redo->next = keyMods;
		keyMods = redo;
	}

	redo->type |= type;
	redo->keyCode = keyCode;
	if (type & DC_TRANSLATE_TO) redo->translateToKeyCode = likeKeyCode;
	if (type & DC_BEHAVE_LIKE) redo->behaveLikeKeyCode = likeKeyCode;
	return true;
}


// Stop managing key behavior modifications for a keyboard.
void KeyBehaviorManager::removeKeyboard(IOHIKeyboard *keyboard) {

	if (!keyboard) return;

	if (DEBUGGING) IOLog("Removing keyboard: %s\n", keyboard->getName());

	KeyboardRemapper **pointer = findKeyboard(keyboard);
	if (!pointer) return;

	KeyboardRemapper *remapper = *pointer;
	*pointer = remapper->__dc_next;
	remapper->__dc_detach();
	remapper->release();
}


// Stop managing key behavior modifications for all keyboards.
void KeyBehaviorManager::removeAllKeyboards() {

	if (DEBUGGING) IOLog("Removing all keyboard remappers...\n");

	while (keyboards) removeKeyboard(keyboards->__dc_keyboard);
}


// Restore a single key's behavior, across all managed keyboards.
void KeyBehaviorManager::restoreKey(UInt8 keyCode, int type) {

	for (KeyboardRemapper *remapper = keyboards; remapper; remapper = remapper->__dc_next) {
		remapper->__dc_restoreKey(keyCode, type);
	}

	KeyRedo **pointer = findKeyMod(keyCode);
	if (!pointer) return;

	KeyRedo *redo = *pointer;
	int restored = redo->type & type;
	redo->type ^= restored;

	if (redo->type == DC_DEFAULT) {

		if (DEBUGGING) IOLog("Manager: Freeing redo data for key %d.\n", keyCode);

		*pointer = redo->next;
		IOFree(redo, sizeof(KeyRedo));;
	}
	return;
}


// Restore the entire key map to original settings, across all managed keyboards.
void KeyBehaviorManager::restoreAllKeys(int type) {

	for (KeyboardRemapper *remapper = keyboards; remapper; remapper = remapper->__dc_next) {
		remapper->__dc_restoreAllKeys(type);
	}

	KeyRedo **pointer = &keyMods;
	KeyRedo *redo;

	while (redo = *pointer) {

		int restored = redo->type & type;
		redo->type ^= restored;

		// If the key has been fully restored, delete the redo data.
		if (redo->type == DC_DEFAULT) {

			if (DEBUGGING) IOLog("Manager: Freeing redo data for key %d.\n", redo->keyCode);

			*pointer = redo->next;
			IOFree(redo, sizeof(KeyRedo));
		}
		else pointer = &(redo->next);
	}
}


