#ifndef _BOUNDARY_H
#define _BOUNDARY_H

#include <mach/mach_types.h>
#include <sys/systm.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include "KeyBehaviorManager.h"
#include "Defines.h"

/* the sysctl docs said to declare this */
//struct sysctl_oid_list sysctl__dc_children;

/* a variable in MBHIDHack.h for configuring this program */
extern
int dcConfig;

extern parameter params[];

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
		keyBehaviorManager.applyConfig();
	}
	return (error);
}


/* declare the new top level sysctl node "dc" */
SYSCTL_NODE(, OID_AUTO, dc, CTLFLAG_RW, 0, "DoubleCommand");
SYSCTL_DECL(_dc_config);


SYSCTL_STRUCT(_dc, OID_AUTO, param0, CTLFLAG_RW|CTLFLAG_ANYBODY, 
              &(params[0]), _parameter, "Parameter 0");



/* declare a new child of our new node and a handler function */
SYSCTL_PROC (_dc, OID_AUTO, config,
	CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY,
	&dcConfig, 0, &dc_sysctl_config, "I",
	"Double Command Configuration");

#endif // _BOUNDARY_H
