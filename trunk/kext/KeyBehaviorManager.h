/*
 *  KeyBehaviorManager.h
 *  DoubleCommand
 *
 *  Created by Mike Ter Louw on 4/9/06.
 *
 */
#ifndef _KeyBehaviorManager_h
#define _KeyBehaviorManager_h

#include "KeyboardRemapper.h"


// Structure representing a key behavior modification.
typedef struct keyRedo {
	int type;
	UInt8 keyCode;
	UInt8 behaveLikeKeyCode;
	UInt8 translateToKeyCode;
	struct keyRedo *next;
} KeyRedo;


/* This is the keyboard behavior manager.  It manages behavior modifiers for
 * a set of keyboards.  It can interperet a settings variable and apply the
 * effects across a set of managed keyboards, and keep them all apprised of
 * changes to the settings variable.  It can also forward key modify/restore
 * requests to the set of keyboards.
 */
class KeyBehaviorManager {

protected:
	int config;
	KeyboardRemapper *keyboards;
	KeyRedo *keyMods;

	KeyboardRemapper **findKeyboard(IOHIKeyboard *);
	KeyRedo **findKeyMod(UInt8);

public:
	KeyBehaviorManager();

	bool addKeyboard(IOHIKeyboard *);
	void applyConfig();
	void cleanup();
	int getCurrentConfig();
	bool modifyKey(UInt8, int = DC_ALL_MODS, UInt8 = NX_NUMKEYCODES);
	void removeAllKeyboards();
	void removeKeyboard(IOHIKeyboard *);
	void restoreKey(UInt8, int = DC_ALL_MODS);
	void restoreAllKeys(int = DC_ALL_MODS);

};

#endif
