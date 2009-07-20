/*
 *  KeyboardRemapper.h
 *  DoubleCommand
 *
 *  Created by Mike Ter Louw on 4/8/06.
 *
 */

#ifndef _KeyboardRemapper_h
#define _KeyboardRemapper_h


#define protected public
#define private public
#include <IOKit/hidsystem/IOHIKeyboardMapper.h>
#undef private

#include <IOKit/hidsystem/IOHIKeyboard.h>
#undef protected


/**
 * The different types of key modification are defined here.
 *
 * In order to understand what each mod type does, it's important to know the
 * different stages of key processing and how they interact.  Here's a diagram
 * of the part that's relevant to us:
 *
 * Mapper -> Keyboard Device -> DoubleCommand Stage 2 -> Keyboard Client
 *
 * For our purposes, the initial stage of processing happens at the key
 * mapper.  It will receive key code up/down events for each key pressed, and
 * pass them on to the keyboard device.
 *
 * The keyboard device then sets the device flags based on the received event,
 * and then sets up a timed repeat event for the key code if it's a "character
 * generating" key code.  (This repeat event will invoke our processing chain
 * at the mapper stage.)  The keyboard device then sends a key up/down or
 * flags-changed event to the DoubleCommand stage 2 filter.
 *
 * The stage 2 filter then performs complex processing on the device flags and
 * modifies the received event, and passes them on to the keyboard client.
 *
 *
 *
 * All that being said, what we're doing here is the mapper stage of
 * modification.  That is, we are affecting what events the keyboard device
 * will see, and in some cases, how it responds to those events.  The effect
 * of each mod available in this stage can be described as follows.  (For each
 * mod type there is a synonym used to pretty up the syntax of some methods.)
 *
 * DC_DEFAULT / DC_RESTORE
 * The key code will be passed untranslated from the mapper to the keyboard
 * device.  The keyboard device will generate events based on the key's
 * default behavior.
 *
 * DC_TRANSLATE / DC_TRANSLATE_TO
 * When the mapper receives an event on the key code, it will instead pass the
 * same event but a different key code to the keyboard device.  The keyboard
 * device will generate events based on the currently defined behavior (see
 * below) of the altered key code.
 *
 * DC_BEHAVIOR / DC_BEHAVE_LIKE
 * This has no effect on how the mapper responds to the key code.  However,
 * when the keyboard device receives an event on the key code, it will
 * generate events in the same way that it would generate events for some other
 * (specified) key code.
 *
 * DC_SUPPRESS
 * When the mapper receives an event on the key code, it will not forward the
 * event to the keyboard device.  The event will be dropped.  This mod will
 * shadow the effects of a DC_TRANSLATE mod on the same key code.
 *
 */
#define DC_DEFAULT			0x0
#define DC_RESTORE			0X0

#define DC_TRANSLATE		0x1
#define DC_TRANSLATE_TO		0x1

#define DC_BEHAVIOR			0x2
#define DC_BEHAVE_LIKE		0x2

#define DC_SUPPRESS			0x4

#define DC_ALL_MODS			0x7


// Structure representing a key modification.
typedef struct keyUndo {

	int type;
	UInt8 keyCode;

	// For DC_TRANSLATE type.
	UInt8 likeKeyCode;

	// For DC_BEHAVIOR type.
	unsigned char bits;
	unsigned char *mapping;

	struct keyUndo *next;

} KeyUndo;


/**
 * This is the keyboard remapper class.  It replaces the keyboard device's own
 * mapper, and passes events to it when/if/how it feels like it.
 */
class KeyboardRemapper: public IOHIKeyboardMapper {
	OSDeclareDefaultStructors(KeyboardRemapper)

protected:
	IOHIKeyboardMapper *__dc_keyMap;
	KeyUndo *__dc_keyMods;

	KeyUndo **__dc_findKeyMod(UInt8);
	unsigned char __dc_getOriginalKeyBits(UInt8);
	unsigned char *__dc_getOriginalKeyMapping(UInt8);
	KeyUndo *__dc_newKeyMod(UInt8);
	void __dc_restoreKey(KeyUndo *, UInt32);
	bool __dc_verifyStageTwo();

public:
	IOHIKeyboard *__dc_keyboard;
	KeyboardRemapper *__dc_next;

	bool __dc_attach(IOHIKeyboard *);
	bool __dc_attached();
	void __dc_detach();
	bool __dc_modifyKey(UInt8, UInt32 = DC_ALL_MODS, UInt8 = NX_NUMKEYCODES);
	void __dc_restoreKey(UInt8, UInt32 = DC_ALL_MODS);
	void __dc_restoreAllKeys(UInt32 = DC_ALL_MODS);

	// Wrappers for public IOHIKeyboardMapper interface methods.
	virtual UInt8 getParsedSpecialKey(UInt8);
	virtual bool init(IOHIKeyboard *, const UInt8 *, UInt32, bool);
	virtual void keyEventPostProcess(void);
	virtual const UInt8 *mapping();
	virtual UInt32 mappingLength();
	virtual IOReturn message(UInt32, IOService *, void *argument = 0);
	virtual bool serialize(OSSerialize *s) const;
	virtual	void setKeyboardTarget(IOService *);
	virtual IOReturn setParamProperties(OSDictionary *);
	virtual void translateKeyCode(UInt8, bool, kbdBitVector);
	virtual bool updateProperties(void);

	// Intercept standard OSObject calls to avoid sending them to IOHIKeyboardMapper.
	virtual void free();
	virtual void release() { OSObject::release(); }
	virtual void release(int when) { OSObject::release(when); }
	virtual void retain() { OSObject::retain(); }

};

#endif
