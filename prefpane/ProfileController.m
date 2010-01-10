#import "ProfileController.h"
#import "RemapListController.h"
#import "PersistenceController.h"
#import "KeyRemapEntry.h"
#import "KeyCombo.h"

@interface ProfileController (Private)
-(NSData*)getSerializedList;
-(NSArray*)getUnserializedList;
@end

static NSString* remapListKey = @"remapList";
static NSString* currentProfileKey = @"currentProfile";

@implementation ProfileController

-(id)init
{
  if((self = [super init]))
  {
    persistenceController = [[PersistenceController alloc] init];
    remapList = [[self getUnserializedList] mutableCopy];
    if(remapList == nil)
      remapList = [[NSMutableArray array] retain];
    currentProfile = [persistenceController getObjectForKey:currentProfileKey];
  }
  return self;
}

-(NSArray*)getUnserializedList
{
  NSData* serializedList = [persistenceController getObjectForKey:remapListKey];
  return [NSKeyedUnarchiver unarchiveObjectWithData:serializedList];  
}
-(BOOL)doesComboExist:(KeyCombo*)combo
{
  for(KeyRemapEntry* entry in remapList)
  {
    KeyCombo* from = [entry remapFrom];
    if([from isEqualToCombo:combo]) return NO;
  }
  return YES;
}

-(void)saveCurrentProfile
{
  NSData* serializedList = [self getSerializedList];
  [persistenceController setObject:serializedList forKey:remapListKey];
  [persistenceController persistPreferencePaneSettings];  
}

-(void)addNewEntry:(KeyRemapEntry*)newEntry
{
  [remapList addObject:newEntry];
}
-(void)removeEntryAtIndex:(unsigned int)index
{
  [remapList removeObjectAtIndex:index];
}
-(void)removeAllEntries
{
  [remapList removeAllObjects];
}
-(unsigned int)count
{
  return [remapList count];
}



-(NSData*)getSerializedList
{
  return [NSKeyedArchiver archivedDataWithRootObject:remapList];
}

-(NSArray*)remapList
{
  return [NSArray arrayWithArray:remapList];
}

@end
