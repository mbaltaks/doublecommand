#import "DeNadolleDoubleCommandPrefsPane.h" 
#import <Security/AuthorizationTags.h>
// #import <sys/param.h>
#import <sys/sysctl.h>


@implementation DeNadolleDoubleCommandPrefsPane

//  --------------------------------------------------------------------------------------
//  all checkboxes off, please
//
- (IBAction)allOffPressed:(id)sender
{
	if (mEditVal != 0) {
		mEditVal = 0;
		int i;
		for (i = 0; i <= DCP_lastusedbit; i++) {
			id theCell = [checkBoxes cellWithTag: i];
			[theCell setState: FALSE];
		}
	}
	[editVal setStringValue: [NSString stringWithFormat:@"%d", mEditVal]];
}



//  --------------------------------------------------------------------------------------
//  somebody did click on a chekbox
//
- (IBAction)checkBoxClicked:(id)sender
{
	id cell = [sender selectedCell];
	int bit = [cell tag];
	BOOL setOn = ([cell state] == NSOnState);
	
	
	if ([self isBitSet:bit] != setOn) {
		int val = 1 << bit;
		int remove;
		if (setOn) {
		
		
			//Added by Sastira - sastira@gmail.com
			//This code will automatically deselect conflicting options in the preference pane.
			//Please note: I learned Objective-C yesterday, so there may be a much better way of doing this.
			switch(bit)
			{
				case CAPSLOCK_TO_CONTROL:
				case CAPSLOCK_TO_DELETE:
				case CAPSLOCK_TO_FORWARD_DELETE:
				case DISABLE_CAPSLOCK:
				{
					if (bit != CAPSLOCK_TO_CONTROL && [[checkBoxes cellWithTag:CAPSLOCK_TO_CONTROL] state] == NSOnState) 
					{
						remove = 1 << CAPSLOCK_TO_CONTROL;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:CAPSLOCK_TO_CONTROL] setState: FALSE];				
					}
					if (bit != CAPSLOCK_TO_DELETE && [[checkBoxes cellWithTag:CAPSLOCK_TO_DELETE] state] == NSOnState)
					{
						remove = 1 << CAPSLOCK_TO_DELETE;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:CAPSLOCK_TO_DELETE] setState: FALSE];	
					}
					if (bit != CAPSLOCK_TO_FORWARD_DELETE && [[checkBoxes cellWithTag:CAPSLOCK_TO_FORWARD_DELETE] state] == NSOnState) 
					{
						remove = 1 << CAPSLOCK_TO_FORWARD_DELETE;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:CAPSLOCK_TO_FORWARD_DELETE] setState: FALSE];					
					}
					if (bit != DISABLE_CAPSLOCK && [[checkBoxes cellWithTag:DISABLE_CAPSLOCK] state] == NSOnState) 
					{
						remove = 1 << DISABLE_CAPSLOCK;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:DISABLE_CAPSLOCK] setState: FALSE];					
					}					
				}
				break;
					
				case OPTION_R_TO_CONTROL:
				case OPTION_R_TO_FORWARD_DELETE:
				case OPTION_R_TO_ENTER:
				case DISABLE_COMMAND_AND_OPTION:
				{
					if (bit != OPTION_R_TO_CONTROL && [[checkBoxes cellWithTag:OPTION_R_TO_CONTROL] state] == NSOnState) 
					{
						remove = 1 << OPTION_R_TO_CONTROL;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:OPTION_R_TO_CONTROL] setState: FALSE];					
					}
					if (bit != OPTION_R_TO_FORWARD_DELETE && [[checkBoxes cellWithTag:OPTION_R_TO_FORWARD_DELETE] state] == NSOnState) 
					{
						remove = 1 << OPTION_R_TO_FORWARD_DELETE;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:OPTION_R_TO_FORWARD_DELETE] setState: FALSE];					
					}
					if (bit != DISABLE_COMMAND_AND_OPTION && [[checkBoxes cellWithTag:DISABLE_COMMAND_AND_OPTION] state] == NSOnState) 
					{
						remove = 1 << DISABLE_COMMAND_AND_OPTION;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:DISABLE_COMMAND_AND_OPTION] setState: FALSE];					
					}					
					if (bit != OPTION_R_TO_ENTER && [[checkBoxes cellWithTag:OPTION_R_TO_ENTER] state] == NSOnState) 
					{
						remove = 1 << OPTION_R_TO_ENTER;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:OPTION_R_TO_ENTER] setState: FALSE];					
					}					
				}
				break;
			
			case ENTER_TO_COMMAND:		
			case ENTER_TO_CONTROL:			
			case ENTER_TO_OPTION:				
			case ENTER_TO_FUNCTION:			
			case ENTER_TO_FORWARD_DELETE:
				{
					if (bit != ENTER_TO_COMMAND && [[checkBoxes cellAtRow:0 column:0] state] == NSOnState) 
					{
						remove = 1 << ENTER_TO_COMMAND;
						mEditVal &= (~remove);
						[[checkBoxes cellAtRow:0 column:0] setState: FALSE];					
					}
					if (bit != ENTER_TO_CONTROL && [[checkBoxes cellWithTag:ENTER_TO_CONTROL] state] == NSOnState) 
					{
						remove = 1 << ENTER_TO_CONTROL;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:ENTER_TO_CONTROL] setState: FALSE];					
					}
					if (bit != ENTER_TO_OPTION && [[checkBoxes cellWithTag:ENTER_TO_OPTION] state] == NSOnState) 
					{
						remove = 1 << ENTER_TO_OPTION;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:ENTER_TO_OPTION] setState: FALSE];					
					}
					if (bit != ENTER_TO_FUNCTION && [[checkBoxes cellWithTag:ENTER_TO_FUNCTION] state] == NSOnState) 
					{
						remove = 1 << ENTER_TO_FUNCTION;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:ENTER_TO_FUNCTION] setState: FALSE];					
					}
					if (bit != ENTER_TO_FORWARD_DELETE && [[checkBoxes cellWithTag:ENTER_TO_FORWARD_DELETE] state] == NSOnState) 
					{
						remove = 1 << ENTER_TO_FORWARD_DELETE;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:ENTER_TO_FORWARD_DELETE] setState: FALSE];					
					}
				}
				break;
				
				case COMMAND_TO_OPTION:
				case COMMAND_TO_CONTROL:
				{
					if (bit != COMMAND_TO_OPTION && [[checkBoxes cellWithTag:COMMAND_TO_OPTION] state] == NSOnState) 
					{
						remove = 1 << COMMAND_TO_OPTION;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:COMMAND_TO_OPTION] setState: FALSE];					
					}					
					if (bit != COMMAND_TO_CONTROL && [[checkBoxes cellWithTag:COMMAND_TO_CONTROL] state] == NSOnState) 
					{
						remove = 1 << COMMAND_TO_CONTROL;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:COMMAND_TO_CONTROL] setState: FALSE];					
					}
				}
				
				case SHIFT_DELETE_TO_FORWARD_DELETE:
				case SWAP_DELETE_AND_FORWARD_DELETE:
				{		
					if (bit != SHIFT_DELETE_TO_FORWARD_DELETE && [[checkBoxes cellWithTag:SHIFT_DELETE_TO_FORWARD_DELETE] state] == NSOnState) 
					{
						remove = 1 << SHIFT_DELETE_TO_FORWARD_DELETE;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:SHIFT_DELETE_TO_FORWARD_DELETE] setState: FALSE];					
					}
					if (bit != SWAP_DELETE_AND_FORWARD_DELETE && [[checkBoxes cellWithTag:SWAP_DELETE_AND_FORWARD_DELETE] state] == NSOnState) 
					{
						remove = 1 << SWAP_DELETE_AND_FORWARD_DELETE;
						mEditVal &= (~remove);
						[[checkBoxes cellWithTag:SWAP_DELETE_AND_FORWARD_DELETE] setState: FALSE];					
					}					
				}
				break;
			}
			//End addition by Sastira
			
			mEditVal |= val;
		} else {
			mEditVal &= (~val);
		}
	}
	[editVal setStringValue: [NSString stringWithFormat:@"%d", mEditVal]];
}



//  --------------------------------------------------------------------------------------
//  User wants to save user prefs
//
- (IBAction)setUserPressed:(id)sender {
	mUserVal = mEditVal;
	if (! [self writeUserSettings]) {
		NSRunAlertPanel(@"DoubleCommand Prefs",
			@"Could not write your user prefs. Sorry.",
			@"Oh dear", nil,  nil);
	}
	[self readUserSettings];
}

//  --------------------------------------------------------------------------------------
//  User wants to save system prefs
//
- (IBAction)setSystemPressed:(id)sender {
	mSystemVal = mEditVal;
	OSStatus err = [self writeSystemSettings];
	if ( (err != 0) && (err != errAuthorizationCanceled)) {
		NSString * errStr = [NSString stringWithFormat: @"Could not write system prefs, error: %d", err];
		NSRunAlertPanel(@"DoubleCommand Prefs",
			errStr,
			@"Oh dear", nil,  nil);
	}
	
	// read again
	// but it seems that we need to wait a litte 
	// or else we would read the old value again before the new one was saved.

	NSTimeInterval waitingTime = 0.1;                      // half a second
	NSDate *recoverDate = [[NSDate date] addTimeInterval:waitingTime + [[NSDate date] timeIntervalSinceNow]];
	[NSThread sleepUntilDate: recoverDate];                 // Blocks the current thread 	
	
	[self readSystemSettings];
	
	
}

		

//  --------------------------------------------------------------------------------------
//  User wants to activate active prefs
//
- (IBAction)setActivePressed:(id)sender {
	mActiveVal = mEditVal;
	OSStatus err = [self writeActiveSettings];
	if (err) {
		NSString * errStr = [NSString stringWithFormat: @"Could not activate settings, error: %d", err];
		NSRunAlertPanel(@"DoubleCommand Prefs",
			errStr,
			@"Oh dear", nil,  nil);
	}
	[self readActiveSettings];
}
	

//  --------------------------------------------------------------------------------------
//  get the UserValue into edit mode
//
- (IBAction)showUserPressed:(id)sender {
	mEditVal = mUserVal;
	[self refreshCheckBoxes];
}


//  --------------------------------------------------------------------------------------
//  get the SystemValue into edit mode
//
- (IBAction)showSystemPressed:(id)sender {
	mEditVal = mSystemVal;
	[self refreshCheckBoxes];
}

//  --------------------------------------------------------------------------------------
//  get the ActiveValue into edit mode
//
- (IBAction)showActivePressed:(id)sender {
	mEditVal = mActiveVal;
	[self refreshCheckBoxes];
}





//  --------------------------------------------------------------------------------------
//  we were just loaded 
//
- (void)mainViewDidLoad {
	mAuthRef = nil;
	mUserPrefPath = NSHomeDirectory();
	mUserPrefPath = [mUserPrefPath stringByAppendingPathComponent: userPrefsRelPath];
	[mUserPrefPath retain];
	
	if (! [self readActiveSettings]) {
		NSRunAlertPanel(@"DoubleCommand Prefs",
			@"DoubleCommand seems not to be running at the moment.\nYou can save prefs but can't activate settings.\nMaybe you need to reinstall...",
			@"Oh dear", nil,  nil);
	}
	// the other prefs will be fetched in didSelect in a tick :)
}


//  --------------------------------------------------------------------------------------
//  every time th prefs gets selected
//
- (void) didSelect {
	[self readSystemSettings];
	[self readUserSettings];
	[self readActiveSettings];
	mEditVal = mActiveVal;
	[self refreshCheckBoxes];
}


//  --------------------------------------------------------------------------------------
//  check a bit in mEditValue
//
- (BOOL)isBitSet:(int)bit {
	return ((mEditVal & (1 << bit)) != 0);
}

//  --------------------------------------------------------------------------------------
//  read System Prefs from Disk
//
- (BOOL) readSystemSettings {
	NSFileManager *manager = [NSFileManager defaultManager];
	BOOL hasSettings = YES;
	if ([manager fileExistsAtPath: systemPrefsPath]) {
		NSString *thePrefsStr = [ NSString stringWithContentsOfFile:systemPrefsPath];
		mSystemVal = [thePrefsStr intValue];
		[systemVal setStringValue: [NSString stringWithFormat:@"%d", mSystemVal]];
	} else {
		mSystemVal = 0;
		[systemVal setStringValue:@"n/a"];
		hasSettings = NO;
	}
	[showSystemButton setEnabled:hasSettings];
	return hasSettings;
}

//  --------------------------------------------------------------------------------------
//  write System prefs to Disk
//
- (OSStatus) writeSystemSettings {
	OSStatus err = 0;
	
	if (mAuthRef == nil) {
		err = [self tryAuthorization];
	}
	if (! err) {
		const char * const args[] = { [[NSString stringWithFormat:@"%d", mSystemVal] UTF8String], NULL };
		err = AuthorizationExecuteWithPrivileges (mAuthRef, [sysPrefsWriteTool UTF8String], 
										kAuthorizationFlagDefaults, (char * const *) args, nil);
	}
	return err;
}


//  --------------------------------------------------------------------------------------
//  read User Prefs from Disk
//
- (BOOL) readUserSettings {
	NSFileManager *manager = [NSFileManager defaultManager];
	BOOL hasSettings = YES;
	if ([manager fileExistsAtPath: mUserPrefPath]) {
		NSString *thePrefsStr = [ NSString stringWithContentsOfFile:mUserPrefPath];
		mUserVal = [thePrefsStr intValue];
		[userVal setStringValue: [NSString stringWithFormat:@"%d", mUserVal]];
	} else {
		mUserVal = 0;
		[userVal setStringValue:@"n/a"];
		hasSettings = NO;
	}
	[showUserButton setEnabled:hasSettings];
	return hasSettings;
}


//  --------------------------------------------------------------------------------------
//  write User Prefs to Disk
//
- (BOOL) writeUserSettings
{
    BOOL ret = NO;
	NSString * thePrefs = [NSString stringWithFormat: @"%d", mUserVal];
	ret = [thePrefs writeToFile:mUserPrefPath atomically:YES];
    // defaults write com.apple.loginwindow LoginHook /Library/StartupItems/DoubleCommand/config.command
    /*NSTask * setting = [[NSTask alloc] init];
    [setting setLaunchPath:@"/usr/bin/defaults"];
    [setting setArguments:[NSArray arrayWithObjects:@"write", @"com.apple.loginwindow", @"LoginHook", @"/Library/StartupItems/DoubleCommand/login.command", nil]];
    [setting launch];
    [setting release];*/
    return ret;
}


//  --------------------------------------------------------------------------------------
//  read Active Settings from sysctl
//
- (BOOL) readActiveSettings {
	BOOL hasSettings = YES;

	char *name = "dc.config";
	size_t len = 4;
	int errCode = 0;
	int val = 0;

    errCode = sysctlbyname(name, &val, &len, NULL, 0);

    if(errCode == 0)  {
		mActiveVal = (unsigned int)val;
		[activeVal setStringValue: [NSString stringWithFormat:@"%d", mActiveVal]];
	} else {
		mActiveVal = 0;
		hasSettings = NO;
		[activeVal setStringValue:@"n/a"];
	}

	[showActiveButton setEnabled:hasSettings];
	[setActiveButton setEnabled:hasSettings];
	return hasSettings;
}


//  --------------------------------------------------------------------------------------
//  write Active Settings to sysctl
//
- (OSStatus) writeActiveSettings {
    char *name = "dc.config";
    u_int len = 4;
    OSStatus errCode = 0;

    errCode = sysctlbyname(name, NULL, 0, &mActiveVal, len);
    return (errCode);
}





//  --------------------------------------------------------------------------------------
//  try to get the root authorization to save global prefs
//  might return errAuthorizationCanceled, which is not an error but sill failed
//
- (OSStatus) tryAuthorization {
	OSStatus err = 0;
	
	// check if the tool is there and some other things:
	NSFileManager *manager = [NSFileManager defaultManager];
	if (! ([manager fileExistsAtPath: sysPrefsWriteTool])) {
		err = -43;
	}
	
	if (!err) {
		// get the root auth
		AuthorizationItem theAItem;
		theAItem.name = kAuthorizationRightExecute;
		theAItem.flags = 0;
		theAItem.value = (void *)[sysPrefsWriteTool UTF8String];
		theAItem.valueLength = [sysPrefsWriteTool length];
		
		AuthorizationItemSet theASet;
		theASet.count = 1;
		theASet.items = &theAItem;
		
		UInt32 aFlags =  kAuthorizationFlagExtendRights | kAuthorizationFlagInteractionAllowed | kAuthorizationFlagPreAuthorize;
						
		err = AuthorizationCreate(&theASet, kAuthorizationEmptyEnvironment, aFlags, &mAuthRef);
	}
	return err;
}

//  --------------------------------------------------------------------------------------
//  unauthorizise
//
- (void) destroyAuthorization {
	if (mAuthRef) {
		AuthorizationFree (mAuthRef, kAuthorizationFlagDestroyRights);
		mAuthRef = 0;
	}
}

//  --------------------------------------------------------------------------------------
//  clean up before dying
//
- (void) dealloc {
	[self destroyAuthorization];
	[mUserPrefPath retain];
	[super dealloc];
}


//  --------------------------------------------------------------------------------------
//  some Value was set mEditVal and we should set the checkboxes to match
//
- (void) refreshCheckBoxes {
	int i;
	for (i = 0; i <= DCP_lastusedbit; i++) {
		id theCell = [checkBoxes cellWithTag: i];
		if (theCell) [theCell setState: [self isBitSet:i]];
	}
	[editVal setStringValue: [NSString stringWithFormat:@"%d", mEditVal]];
}
	



@end
