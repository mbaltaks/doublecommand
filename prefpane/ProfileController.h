#import <Cocoa/Cocoa.h>

#import "NewProfileNameWindow.h"

@class RemapListController;
@class PersistenceController;
@class KeyRemapEntry;
@class KeyCombo;

@protocol ProfileControllerDelegate
-(void)profileChanged;
@end


@interface ProfileController : NSObject <NewProfileNameWindowDelegate>
{
  NSMutableArray* remapList;
  NSString* currentProfile;
  
  PersistenceController* persistenceController;
  
  id<ProfileControllerDelegate> delegate;

  IBOutlet NSComboBox* profilePicker;
  IBOutlet NewProfileNameWindow* newProfileNameWindow;
}

@property(readwrite,retain) NSComboBox* profilePicker;
@property(readwrite,retain) NewProfileNameWindow* newProfileNameWindow;
@property(readwrite,retain) id<ProfileControllerDelegate> delegate;


-(NSArray*)remapList;
-(void)addNewEntry:(KeyRemapEntry*)newEntry;
-(void)removeEntryAtIndex:(unsigned int)index;
-(void)removeAllEntries;
-(unsigned int)count;
-(BOOL)comboExists:(KeyCombo*)combo;
-(void)persistSettings;

-(IBAction)createProfileButtonClicked:(NSButton*)sender;
-(IBAction)deleteProfileButtonClicked:(NSButton*)sender;
@end
