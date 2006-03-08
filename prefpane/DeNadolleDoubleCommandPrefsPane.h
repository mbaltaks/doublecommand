/* DeNadolleDoubleCommandPrefsPane */

#import <PreferencePanes/NSPreferencePane.h>
#import <CoreFoundation/CoreFoundation.h> 
#import <Security/Authorization.h>

#define DCP_lastusedbit  23
NSString * systemPrefsPath = @"/Library/StartupItems/DoubleCommand/DoubleCommand.pref";
NSString * userPrefsRelPath = @"Library/Preferences/DoubleCommand.pref";
NSString * sysPrefsWriteTool = @"/Library/PreferencePanes/DoubleCommandPreferences.prefPane/Contents/Resources/prefWriter";


@interface DeNadolleDoubleCommandPrefsPane : NSPreferencePane
{
    IBOutlet id allOffButton;
    IBOutlet id checkBoxes; // NSMatrix containig all the NSButton-Checkboxes
    IBOutlet id editVal;
	
    IBOutlet id showUserButton;
    IBOutlet id showSystemButton;
    IBOutlet id showActiveButton;
	
    IBOutlet id setUserButton;
    IBOutlet id setSystemButton;
    IBOutlet id setActiveButton;

    IBOutlet id userVal;
    IBOutlet id systemVal;
    IBOutlet id activeVal;
	
	AuthorizationRef mAuthRef;
	
	unsigned int mUserVal;
	unsigned int mSystemVal;
	unsigned int mActiveVal;
	unsigned int mEditVal;
	
	NSString * mUserPrefPath;
	
}

/*
	all checkboxes off
*/
- (IBAction)allOffPressed:(id)sender;

- (IBAction)checkBoxClicked:(id)sender;

- (IBAction)setUserPressed:(id)sender;
- (IBAction)setSystemPressed:(id)sender;
- (IBAction)setActivePressed:(id)sender;

- (IBAction)showUserPressed:(id)sender;
- (IBAction)showSystemPressed:(id)sender;
- (IBAction)showActivePressed:(id)sender;

/* will be called from the App */
- (void) mainViewDidLoad;
- (void) didSelect;

/* internal tools */
- (BOOL) isBitSet:(int)bit;
- (OSStatus) tryAuthorization;
- (void) destroyAuthorization;
- (BOOL) readSystemSettings;
- (OSStatus) writeSystemSettings;
- (BOOL) readUserSettings;
- (BOOL) writeUserSettings;
- (BOOL) readActiveSettings;
- (OSStatus) writeActiveSettings;
- (void) refreshCheckBoxes;

@end
