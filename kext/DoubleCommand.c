/*
 * $Id$
 *
 * Name: DoubleCommand.c
 * Project: DoubleCommand
 * Author: Michael Baltaks <mbaltaks@mac.com>
 * Creation Date: 2002-4-26
 * Last Modified: 2002-6-6
 * Originally based on iJect by Christian Starkjohann <cs@obdev.at> 
 * Tabsize: 4
 * Copyright: GNU General Public License version 2.0
 */

#include <mach/mach_types.h>
#include <sys/systm.h>
#include <sys/param.h>
#include <sys/sysctl.h>

extern int MBHidInit(void);
extern int MBHidExit(void);

/* the sysctl docs said to declare this */
struct sysctl_oid_list sysctl__dc_children;

/* a variable in MBHIDHack.cpp for configuring this program */
extern
int dcConfig;

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
	}
	return (error);
}

/* declare the new top level sysctl node "dc" */
SYSCTL_NODE(, OID_AUTO, dc, CTLFLAG_RW, 0, "DoubleCommand");

/* declare a new child of our new node and a handler function */
SYSCTL_PROC (_dc, OID_AUTO, config, CTLTYPE_INT|CTLFLAG_RW|CTLFLAG_ANYBODY, &dcConfig, 0, &dc_sysctl_config, "I", "Double Command Configuration");

/* kext init auto built by Project Builder */
kern_return_t DoubleCommand_start (kmod_info_t * ki, void * d)
{
	sysctl_register_oid(&sysctl__dc);
	sysctl_register_oid(&sysctl__dc_config);
    return MBHidInit() == 0 ? KERN_SUCCESS : KERN_FAILURE;
}

/* kext unload auto built by Project Builder */
kern_return_t DoubleCommand_stop (kmod_info_t * ki, void * d)
{
	sysctl_unregister_oid(&sysctl__dc);
	sysctl_unregister_oid(&sysctl__dc_config);
    return MBHidExit() == 0 ? KERN_SUCCESS : KERN_FAILURE;
}
