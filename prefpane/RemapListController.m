//
//  RemapListController.m
//  prefpane
//
//  Created by Tyler Bunnell on 12/31/09.
//  Copyright 2009 Slightly Sane Inc. All rights reserved.
//

#import "RemapListController.h"

#import "KeyRemapEntry.h"
#import "KeyCodeTransformer.h"

@implementation RemapListController

@synthesize remapList;

-(id)init
{
  if(self = [super init])
  {
    remapItems = [[NSMutableArray array] retain];
  }
  return self;
}

-(void)addNewEntry:(KeyRemapEntry*)newEntry
{
  NSString* remapFrom = [KeyCodeTransformer stringRepresentationForKeyCombo:[newEntry remapFrom]];
  NSString* remapTo = [KeyCodeTransformer stringRepresentationForKeyCombo:[newEntry remapTo]];
  NSArray* remaps = [NSArray arrayWithObjects:remapFrom,remapTo,nil];
  NSArray* remapKeys = [NSArray arrayWithObjects:@"remapFrom",@"remapTo",nil];
  NSDictionary* newDictionaryEntry = [NSDictionary dictionaryWithObjects:remaps forKeys:remapKeys];
  
  [remapItems addObject:newDictionaryEntry];
  [remapList reloadData];
}
-(int)selectedEntryIndex
{
  return [remapList selectedRow];
}
-(void)deleteEntryAtIndex:(int)index
{
  [remapItems removeObjectAtIndex:index];
}


#pragma mark Datasource Methods
-(int)numberOfRowsInTableView:(NSTableView *)tableView
{
  return [remapItems count];
}

-(id)tableView:(NSTableView *)tableView
      objectValueForTableColumn:(NSTableColumn *)tableColumn
                            row:(int)rowIndex
{
  NSDictionary* entry = [remapItems objectAtIndex:rowIndex];
  return [entry objectForKey:[tableColumn identifier]];
}

@end
