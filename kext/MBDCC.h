#ifndef _MBDCC_h
#define _MBDCC_h

#include "MBHIDHack.h"

#define MAXREMAPS 255



// pinched from uControl
/* Simply a key and flag pair */
typedef struct _keyflagpair {
    unsigned key;
    unsigned flag;
} keyflagpair;

enum _op {
        kSwap    = 1,
        kConvert = 2,
        kSquelch = 3, };

/* This parameter defines the operation, the particular keyboard-type
   and the remap that should occur. */
typedef struct _parameter {
    enum _op op;
    unsigned kbtype;
    keyflagpair src;
    keyflagpair dest;
} parameter;

/* This special parameter is for keys that can't be bound as
   abstractly as a param, so they only require the pair they map to
   since what they're mapping from is builtin to the code.  (capslock
   and enter are examples of this.)  */
typedef struct _special {
    unsigned options; /* bitfield */
    unsigned kbtype;
    keyflagpair replace;
} special;

#define PARAMETER_COUNT 8

parameter params[PARAMETER_COUNT];

// end pinched from uControl



typedef struct keySeq
{
	unsigned eventType;
	unsigned flags;
	unsigned key;
	unsigned charCode;
	unsigned charSet;
	unsigned origCharCode;
	unsigned origCharSet;
	unsigned keyboardType;
	bool repeat;
	int remapID;
	int sequenceNumber;
	bool allKeyboards;
} keySeq;


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


//----------------------------------------------------------------------------
int ModifierToKey(unsigned * eventType,
              unsigned * addFlags,
              unsigned * removeFlags,
              unsigned * key,
              unsigned desired_flags,
              unsigned desired_key,
              unsigned desired_eventType)
{
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

#endif // _MBDCC_h
