#import <Cocoa/Cocoa.h>

#import <AppKit/AppKit.h>

@class KeyRemapEntry;

@interface RemapListController : NSObject
{
  NSMutableArray* remapItems;
  
  IBOutlet NSTableView* remapList;
}

@property(readwrite,retain) NSTableView* remapList;

-(void)replaceAllEntriesWithArray:(NSArray*)newEntries;
-(void)addNewEntry:(KeyRemapEntry*)newEntry;
-(int)selectedEntryIndex;
-(void)removeEntryAtIndex:(int)index;
-(void)removeAllEntries;

@end
