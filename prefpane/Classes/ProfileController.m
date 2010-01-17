#import "ProfileController.h"
#import "RemapListController.h"
#import "PersistenceController.h"
#import "KeyRemapEntry.h"
#import "KeyCombo.h"
#import "WindowShaker.h"

@interface ProfileController ()
@property(readwrite,copy)NSString* currentProfile;

-(void)setRemapList:(NSMutableArray*)array;

-(void)selectProfile:(NSString*)profile;
-(void)loadProfile:(NSString*)profile;
-(void)saveProfile:(NSString*)profile;
@end

static NSString* currentProfileKey = @"currentProfile";
static NSString* profilesKey = @"profiles";

@implementation ProfileController

@synthesize profilePicker;
@synthesize newProfileNameWindow;
@synthesize currentProfile;
@synthesize delegate;

-(void)awakeFromNib
{
  persistenceController = [[PersistenceController alloc] init];
  [self setCurrentProfile:[persistenceController getObjectForKey:currentProfileKey]];
  NSData* serializedProfiles = [persistenceController getObjectForKey:profilesKey];
  NSArray* profiles;
  if(serializedProfiles != nil)
    profiles = [NSKeyedUnarchiver unarchiveObjectWithData:serializedProfiles];
  else
  {
    [self setCurrentProfile:@"Default"];
    profiles = [NSArray arrayWithObject:currentProfile];    
  }
  
  [profilePicker addItemsWithObjectValues:profiles];
  
  [self selectProfile:currentProfile];
}

-(IBAction)createProfileButtonClicked:(NSButton*)sender
{
  [newProfileNameWindow setDelegate:self];
  [newProfileNameWindow resetWindow];
  [NSApp beginSheet:newProfileNameWindow
     modalForWindow:[NSApp mainWindow]
      modalDelegate:self
     didEndSelector:@selector(newProfileNameWindowDidEnd:returnCode:contextInfo:)
        contextInfo:nil]; 
}
-(BOOL)profileNameWindowCanAddNewEntry:(NSString*)newEntry
{
  for(NSString* profile in [profilePicker objectValues])
  {
    if([newEntry isEqualToString:profile])
      return NO;
  }
  return YES;
}
-(void)profileNameWindowAddNewEntry:(NSString *)newEntry
{
  [profilePicker addItemWithObjectValue:newEntry];
  
  NSData* serializedProfileList = [NSKeyedArchiver archivedDataWithRootObject:[profilePicker objectValues]];
  [persistenceController setObject:serializedProfileList forKey:profilesKey];
  
  [self selectProfile:newEntry];
}
-(void)newProfileNameWindowDidEnd:(NSPanel*)sheet returnCode:(int)returnCode contextInfo:(void*)contextInfo
{
  [newProfileNameWindow orderOut:self];
}
-(void)selectProfile:(NSString*)profile
{
  [self setCurrentProfile:profile];
  [persistenceController setObject:profile forKey:currentProfileKey];
  [profilePicker selectItemAtIndex:[profilePicker indexOfItemWithObjectValue:profile]];
  [profilePicker setObjectValue:[profilePicker objectValueOfSelectedItem]];
  [self loadProfile:profile];
  [delegate profileChanged];
}
-(void)loadProfile:(NSString*)profile
{
  NSData* serializedList = [persistenceController getObjectForKey:profile];
  NSArray* unserializedList = nil;
  if(serializedList == nil)
    unserializedList = [NSArray array];
  else
    unserializedList = [NSKeyedUnarchiver unarchiveObjectWithData:serializedList];
  
  
  
  [self setRemapList:[unserializedList mutableCopy]];
}
-(void)saveProfile:(NSString *)profile
{
  NSData* serializedList = [NSKeyedArchiver archivedDataWithRootObject:remapList];
  [persistenceController setObject:serializedList forKey:profile];
}
-(IBAction)deleteProfileButtonClicked:(NSButton*)sender
{
  if(![@"Default" isEqualToString:currentProfile])
  {
    [persistenceController removeObjectForKey:currentProfile];
    [profilePicker removeItemWithObjectValue:currentProfile];
    NSData* serializedProfileList = [NSKeyedArchiver archivedDataWithRootObject:[profilePicker objectValues]];
    [persistenceController setObject:serializedProfileList forKey:profilesKey];
    [self selectProfile:[profilePicker itemObjectValueAtIndex:0]];
  }
  else
  {
    [WindowShaker shakeWindow:[NSApp mainWindow]];
  }

}

-(NSArray*)remapList
{
  return [NSArray arrayWithArray:remapList];
}
-(void)setRemapList:(NSMutableArray *)array
{
  if(remapList != array)
  {
    [remapList release];
    remapList = [array retain];
  }  
}
-(void)addNewEntry:(KeyRemapEntry*)newEntry
{
  [remapList addObject:newEntry];
  [self saveProfile:currentProfile];
}
-(void)removeEntryAtIndex:(unsigned int)index
{
  [remapList removeObjectAtIndex:index];
  [self saveProfile:currentProfile];
}
-(void)removeAllEntries
{
  [remapList removeAllObjects];
  [self saveProfile:currentProfile];
}
-(unsigned int)count
{
  return [remapList count];
}
-(BOOL)comboExists:(KeyCombo*)combo
{
  for(KeyRemapEntry* entry in remapList)
  {
    KeyCombo* from = [entry remapFrom];
    if([from isEqualToCombo:combo]) return YES;
  }
  return NO;
}
-(void)persistSettings
{
  [persistenceController persistPreferencePaneSettings];
}

#pragma mark ComboBox Delegate Methods
-(void)comboBoxSelectionDidChange:(NSNotification *)notification
{
  NSString* newProfile = [profilePicker objectValueOfSelectedItem];
  if(newProfile != nil && ![newProfile isEqualToString:currentProfile])
    [self selectProfile:newProfile];
}

@end
