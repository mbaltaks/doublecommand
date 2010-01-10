#import <Cocoa/Cocoa.h>


@class RemapListController;
@class PersistenceController;
@class KeyRemapEntry;
@class KeyCombo;

@interface ProfileController : NSObject
{
  NSMutableArray* remapList;
  NSString* currentProfile;
  PersistenceController* persistenceController;
}

-(NSArray*)remapList;

-(void)addNewEntry:(KeyRemapEntry*)newEntry;
-(void)removeEntryAtIndex:(unsigned int)index;
-(void)removeAllEntries;
-(unsigned int)count;
-(void)saveCurrentProfile;
-(BOOL)doesComboExist:(KeyCombo*)combo;
@end
