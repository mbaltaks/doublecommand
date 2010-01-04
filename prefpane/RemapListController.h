//
//  RemapListController.h
//  prefpane
//
//  Created by Tyler Bunnell on 12/31/09.
//  Copyright 2009 Slightly Sane Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import <AppKit/AppKit.h>

@class KeyRemapEntry;

@interface RemapListController : NSObject
{
  NSMutableArray* remapItems;
  
  IBOutlet NSTableView* remapList;
}

@property(readwrite,retain) NSTableView* remapList;

-(void)addNewEntry:(KeyRemapEntry*)newEntry;
-(int)selectedEntryIndex;
-(void)removeEntryAtIndex:(int)index;
-(void)removeAllEntries;

@end
