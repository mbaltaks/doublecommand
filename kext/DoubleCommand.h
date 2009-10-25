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

#if __LP64__
bool dc_matched(com_baltaks_driver_DoubleCommand * self,
	void * ref,
	IOService * serv,
	IONotifier * notifier);
#else
bool dc_matched(com_baltaks_driver_DoubleCommand * self,
	void * ref,
	IOService * serv);
#endif

#if __LP64__
bool dc_terminated(com_baltaks_driver_DoubleCommand * self,
	void * ref,
	IOService * serv,
	IONotifier * notifier);
#else
bool dc_terminated(com_baltaks_driver_DoubleCommand * self,
	void * ref,
	IOService * serv);
#endif

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
	unsigned * keyboardType,
	bool * repeat,
	AbsoluteTime * ts);

int special_remap(unsigned * eventType,
	unsigned * flags,
	unsigned * key,
	unsigned * flavor,
	UInt64 * guid,
	bool * repeat,
	AbsoluteTime * ts);


// The Ugly Hack Part.
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
int which_keyboard(IOHIKeyboard * kbd);
int find_client(bool special);

#endif // _DoubleCommand_h
