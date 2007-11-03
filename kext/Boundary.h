#ifndef _BOUNDARY_H
#define _BOUNDARY_H

#include <mach/mach_types.h>
#include <sys/systm.h>
#include <sys/types.h>
#include <sys/sysctl.h>
//#include "KeyBehaviorManager.h"
#include "Common.h"

/* the sysctl docs said to declare this */
//struct sysctl_oid_list sysctl__dc_children;

/* variables in Substitute.cpp for configuring this program */
extern int dcConfigAll;
extern int dcConfig1;
extern int dcConfig2;
extern int dcConfig3;
extern int dcConfig4;
extern int dcKeyboard1;
extern int dcKeyboard2;
extern int dcKeyboard3;
extern int dcKeyboard4;

extern KeyBehaviorManager keyBehaviorManager;

/* this is the function that handles getting and setting the config */
static int
dc_sysctl_config SYSCTL_HANDLER_ARGS
{
	int error = 0;
	if (arg1)
	{
		error = SYSCTL_OUT(req, arg1, sizeof(int));
	}
	else
	{
		error = SYSCTL_OUT(req, &arg2, sizeof(int));
	}
	if (error || !req->newptr)
	{
		return (error);
	}

	if (!arg1)
	{
		error = EPERM;
	}
	else
	{
		error = SYSCTL_IN(req, arg1, sizeof(int));
		//keyBehaviorManager.applyConfig();
	}
	return (error);
}


/* declare the new top level sysctl node "dc" */
SYSCTL_NODE(, OID_AUTO, dc, CTLFLAG_RW, 0, "DoubleCommand");
SYSCTL_DECL(_dc_config);


/* declare a new child of our new node and a handler function */
SYSCTL_PROC (_dc, OID_AUTO, config,
	CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY,
	&dcConfigAll, 0, &dc_sysctl_config, "I",
	"Double Command Configuration");

SYSCTL_PROC (_dc, OID_AUTO, config1, CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY, &dcConfig1, 0, &dc_sysctl_config, "I", "Double Command Configuration 1");
SYSCTL_PROC (_dc, OID_AUTO, config2, CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY, &dcConfig2, 0, &dc_sysctl_config, "I", "Double Command Configuration 2");
SYSCTL_PROC (_dc, OID_AUTO, config3, CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY, &dcConfig3, 0, &dc_sysctl_config, "I", "Double Command Configuration 3");
SYSCTL_PROC (_dc, OID_AUTO, config4, CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY, &dcConfig4, 0, &dc_sysctl_config, "I", "Double Command Configuration 4");
SYSCTL_PROC (_dc, OID_AUTO, keyboard1, CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY, &dcKeyboard1, 0, &dc_sysctl_config, "I", "Double Command Keyboard 1");
SYSCTL_PROC (_dc, OID_AUTO, keyboard2, CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY, &dcKeyboard2, 0, &dc_sysctl_config, "I", "Double Command Keyboard 2");
SYSCTL_PROC (_dc, OID_AUTO, keyboard3, CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY, &dcKeyboard3, 0, &dc_sysctl_config, "I", "Double Command Keyboard 3");
SYSCTL_PROC (_dc, OID_AUTO, keyboard4, CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY, &dcKeyboard4, 0, &dc_sysctl_config, "I", "Double Command Keyboard 4");

#endif // _BOUNDARY_H
