#ifndef _DoubleCommand_h
#define _DoubleCommand_h

#include <IOKit/IOService.h>

class com_baltaks_driver_DoubleCommand : public IOService
{
	OSDeclareDefaultStructors(com_baltaks_driver_DoubleCommand)
public:
	virtual bool init(OSDictionary *dictionary = 0);
	virtual void free(void);
	virtual IOService *probe(IOService *provider, SInt32 *score);
	virtual bool start(IOService *provider);
	virtual void stop(IOService *provider);

	IONotifier * keyboard_notifier;
	IONotifier * terminated_notifier;
};

bool dc_matched(com_baltaks_driver_DoubleCommand * self,
	void * ref,
	IOService * serv);
bool dc_terminated(com_baltaks_driver_DoubleCommand * self,
	void * ref,
	IOService * serv);

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
	bool       repeat,
	AbsoluteTime ts);

void
specialEvent(OSObject * target,
	unsigned   eventType,
	unsigned   flags,
	unsigned   key,
	unsigned   flavor,
	UInt64     guid,
	bool       repeat,
	AbsoluteTime ts);

int remap(unsigned * eventType,
	unsigned * flags,
	unsigned * key,
	unsigned * charCode,
	unsigned * charSet,
	unsigned * origCharCode,
	unsigned * origCharSet,
	unsigned * keyboardType);


const unsigned MAX_KEYBOARDS = 4;

#define protected public
#include <IOKit/hidsystem/IOHIKeyboard.h>
#undef protected

#include <IOKit/hidsystem/IOLLEvent.h>

typedef struct _dc_keyboard
{
	KeyboardEventAction event;
	KeyboardSpecialEventAction special_event;
	IOHIKeyboard * keyboard;
} dc_keyboard;

int hijack_keyboard(IOHIKeyboard * kbd);
int return_keyboard(IOHIKeyboard * kbd);

#endif // _DoubleCommand_h
