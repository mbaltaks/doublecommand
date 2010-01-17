#import "PreferencePaneController.h"

#import "RemapListController.h"
#import "PersistenceController.h"
#import "KeyCombo.h"
#import "KeyRemapEntry.h"

@interface PreferencePaneController ()
-(void)updateDeleterStatus;
-(BOOL)deletersShouldBeEnabled;
-(void)setDeletersEnabled:(BOOL)enabled;
@end

@implementation PreferencePaneController

@synthesize profileController;
@synthesize listController;
@synthesize captureWindow;
@synthesize statusLabel;
@synthesize activateButton;
@synthesize deactivateButton;
@synthesize deleteButton;
@synthesize clearButton;

-(void)mainViewDidLoad
{
  [listController replaceAllEntriesWithArray:[profileController remapList]];
  [profileController setDelegate:self];
}

-(void)didSelect
{  
  [self updateDeleterStatus];
}
-(void)didUnselect
{
  [profileController persistSettings];
}

#pragma mark Delegate Methods
-(void)profileChanged
{
  [listController replaceAllEntriesWithArray:[profileController remapList]];
}
-(BOOL)capturePanelCanAddNewEntry:(KeyRemapEntry*)newEntry
{
  return ![profileController comboExists:[newEntry remapFrom]];
}
-(void)capturePanelAddNewEntry:(KeyRemapEntry*)newEntry
{
  [profileController addNewEntry:newEntry];
  [listController addNewEntry:newEntry];
  [self updateDeleterStatus];

  //send new remap to kext
}


#pragma mark Button Methods
-(IBAction)activateDoubleCommandButtonClicked:(NSButton*)sender
{
  //do logic here to see if DC is running
  //if DC not running, execute kextload
  //send activate message
  //send current remap profile
  
  [statusLabel setStringValue:@"DoubleCommand is active."];
  [activateButton setEnabled:NO];
  [deactivateButton setEnabled:YES];
}
-(IBAction)deactivateDoubleCommandButtonClicked:(NSButton*)sender
{
  //check if DC running
  //if running, send deactivate message
  
  [statusLabel setStringValue:@"DoubleCommand is not active."];
  [deactivateButton setEnabled:NO];
  [activateButton setEnabled:YES];
}

-(IBAction)addNewEntryButtonClicked:(NSButton*)sender
{
  [captureWindow setDelegate:self];
  [captureWindow clearFields];
  [NSApp beginSheet:captureWindow
     modalForWindow:[[self mainView] window]
      modalDelegate:self
     didEndSelector:@selector(capturePanelDidEnd:returnCode:contextInfo:)
        contextInfo:nil];
}
-(void)capturePanelDidEnd:(NSPanel*)sheet returnCode:(int)returnCode contextInfo:(void*)contextInfo
{
  [captureWindow orderOut:self];
}
-(IBAction)deleteEntryButtonClicked:(NSButton*)sender
{
  int index = [listController selectedEntryIndex];
  if(index != -1)
  {
    [profileController removeEntryAtIndex:index];
    [listController removeEntryAtIndex:index];
  }
  else
  {
    NSBeep();
  }
  
  [self updateDeleterStatus];
}
-(IBAction)clearRemapListButtonClicked:(NSButton*)sender
{
  [profileController removeAllEntries];
  [listController removeAllEntries];
  [self updateDeleterStatus];
}
-(IBAction)saveProfileClicked:(NSMenuItem*)sender
{
}
-(IBAction)loadProfileClicked:(NSMenuItem*)sender
{
}

-(void)updateDeleterStatus
{
  [self setDeletersEnabled:[self deletersShouldBeEnabled]];
}
-(void)setDeletersEnabled:(BOOL)enabled
{
  [deleteButton setEnabled:enabled];
  [clearButton setEnabled:enabled];
}
-(BOOL)deletersShouldBeEnabled
{
  if([profileController count] > 0) return YES;
  
  return NO;
}

@end
