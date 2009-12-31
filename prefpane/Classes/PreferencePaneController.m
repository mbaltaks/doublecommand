#import "PreferencePaneController.h"

#import "RemapListController.h"
#import "KeyCombo.h"
#import "KeyRemapEntry.h"

@implementation PreferencePaneController

@synthesize listController;
@synthesize captureWindow;


-(void)didSelect
{
  remapList = [[NSMutableArray array] retain];
}


#pragma mark Delegate Methods
-(BOOL)capturePanelCanAddNewEntry:(KeyRemapEntry*)newEntry
{
  KeyCombo* newFrom = [newEntry remapFrom];
    
  for(KeyRemapEntry* entry in remapList)
  {
    KeyCombo* from = [entry remapFrom];
  
    if(([from modifierFlags] == [newFrom modifierFlags]) &&
       ([from keyCode] == [newFrom keyCode])) return NO;
  }

  return YES;
}
-(void)capturePanelAddNewEntry:(KeyRemapEntry*)newEntry
{
  [remapList addObject:newEntry];
  [listController addNewEntry:newEntry];
}


#pragma mark Button Methods
-(IBAction)activateDoubleCommandButtonClicked:(NSButton*)sender
{
}
-(IBAction)deactivateDoubleCommandButtonClicked:(NSButton*)sender
{
}
-(IBAction)persistForUserButtonClicked:(NSButton*)sender
{
}
-(IBAction)persistForSystemButtonClicked:(NSButton*)sender
{
}

-(IBAction)addNewEntryButtonClicked:(NSButton*)sender
{
  [captureWindow setDelegate:self];
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
}
-(IBAction)clearRemapListButtonClicked:(NSButton*)sender
{
}
-(IBAction)saveRemapListItemClicked:(NSMenuItem*)sender
{
}
-(IBAction)loadRemapListItemClicked:(NSMenuItem*)sender
{
}

/*
 KeyCombo* from = [[[KeyCombo alloc] initWithModifierFlags:NSCommandKeyMask keyCode:1] autorelease];
 KeyCombo* to = [[[KeyCombo alloc] initWithModifierFlags:NSControlKeyMask keyCode:2] autorelease];
 KeyRemapEntry* newEntry = [[[KeyRemapEntry alloc] initWithRemapFrom:from remapTo:to] autorelease];
 [listController addNewEntry:newEntry];*/

@end
