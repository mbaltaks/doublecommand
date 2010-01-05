#import "PreferencePaneController.h"

#import "RemapListController.h"
#import "PersistenceController.h"
#import "KeyCombo.h"
#import "KeyRemapEntry.h"

@interface PreferencePaneController (Private)
-(void)updateDeleterStatus;
-(BOOL)deletersShouldBeEnabled;
-(void)setDeletersEnabled:(BOOL)enabled;
-(NSData*)getSerializedList;
-(NSArray*)getUnserializedList;
@end

static NSString* remapListKey = @"remapList";

@implementation PreferencePaneController

@synthesize listController;
@synthesize captureWindow;
@synthesize statusLabel;
@synthesize activateButton;
@synthesize deactivateButton;
@synthesize deleteButton;
@synthesize clearButton;

-(void)mainViewDidLoad
{
  persistenceController = [[PersistenceController alloc] init];
  
  remapList = [[self getUnserializedList] mutableCopy];
  if(remapList == nil)
    remapList = [[NSMutableArray array] retain];
  [listController replaceAllEntriesWithArray:remapList];
}

-(void)didSelect
{  
  [self updateDeleterStatus];
}
-(NSArray*)getUnserializedList
{
  NSData* serializedList = [persistenceController getObjectForKey:remapListKey];
  return [NSKeyedUnarchiver unarchiveObjectWithData:serializedList];  
}

-(void)didUnselect
{
  NSData* serializedList = [self getSerializedList];
  [persistenceController setObject:serializedList forKey:remapListKey];
  [persistenceController persistPreferencePaneSettings];
}
-(NSData*)getSerializedList
{
  return [NSKeyedArchiver archivedDataWithRootObject:remapList];
}

#pragma mark Delegate Methods
-(BOOL)capturePanelCanAddNewEntry:(KeyRemapEntry*)newEntry
{
  KeyCombo* newFrom = [newEntry remapFrom];
    
  for(KeyRemapEntry* entry in remapList)
  {
    KeyCombo* from = [entry remapFrom];
    if([from isEqualToCombo:newFrom]) return NO;
  }

  return YES;
}
-(void)capturePanelAddNewEntry:(KeyRemapEntry*)newEntry
{
  [remapList addObject:newEntry];
  [listController addNewEntry:newEntry];
  [self updateDeleterStatus];
}


#pragma mark Button Methods
-(IBAction)activateDoubleCommandButtonClicked:(NSButton*)sender
{
  //do logic here to see if DC is running
  [statusLabel setStringValue:@"DoubleCommand is active."];
  [activateButton setEnabled:NO];
  [deactivateButton setEnabled:YES];
}
-(IBAction)deactivateDoubleCommandButtonClicked:(NSButton*)sender
{
  [statusLabel setStringValue:@"DoubleCommand is not active."];
  [deactivateButton setEnabled:NO];
  [activateButton setEnabled:YES];
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
    [remapList removeObjectAtIndex:index];
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
  [remapList removeAllObjects];
  [listController removeAllEntries];
  [self updateDeleterStatus];
}
-(IBAction)saveRemapListItemClicked:(NSMenuItem*)sender
{
}
-(IBAction)loadRemapListItemClicked:(NSMenuItem*)sender
{
}

-(void)updateDeleterStatus
{
  [self setDeletersEnabled:[self deletersShouldBeEnabled]];
}
-(BOOL)deletersShouldBeEnabled
{
  if([remapList count] > 0) return YES;
  
  return NO;
}
-(void)setDeletersEnabled:(BOOL)enabled
{
  [deleteButton setEnabled:enabled];
  [clearButton setEnabled:enabled];
}

/*
 KeyCombo* from = [[[KeyCombo alloc] initWithModifierFlags:NSCommandKeyMask keyCode:1] autorelease];
 KeyCombo* to = [[[KeyCombo alloc] initWithModifierFlags:NSControlKeyMask keyCode:2] autorelease];
 KeyRemapEntry* newEntry = [[[KeyRemapEntry alloc] initWithRemapFrom:from remapTo:to] autorelease];
 [listController addNewEntry:newEntry];*/

@end
