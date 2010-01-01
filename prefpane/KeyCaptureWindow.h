//
//  KeyCapturePanel.h
//  prefpane
//
//  Created by Tyler Bunnell on 12/31/09.
//  Copyright 2009 Slightly Sane Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "NSTextCaptureField.h"

@class KeyRemapEntry;

enum
{
  DCLeftButton,
  DCRightButton
};

@protocol KeyCapturePanelDelegate <NSObject>
-(BOOL)capturePanelCanAddNewEntry:(KeyRemapEntry*)newEntry;
-(void)capturePanelAddNewEntry:(KeyRemapEntry*)newEntry;
@end


@interface KeyCaptureWindow : NSWindow <NSTextCaptureFieldDelegate>
{
  NSTextField* currentFocus;
  NSString* defaultModifierMessage;
  
  unsigned int modifiers;
  
  BOOL fromRemapSet;
  BOOL toRemapSet;
  
  KeyRemapEntry* newRemapEntry;
  
  IBOutlet NSTextField* statusLabel;
  IBOutlet NSTextField* mapFrom;
  IBOutlet NSTextField* mapTo;
    
  IBOutlet NSWindow*     modifierPanel;
  IBOutlet NSTextField* modifierMessage;
  
  IBOutlet NSButton* acceptButton;
}

@property(readwrite,retain)NSTextField* statusLabel;
@property(readwrite,retain)NSTextField* mapFrom;
@property(readwrite,retain)NSTextField* mapTo;

@property(readwrite,retain)NSWindow* modifierPanel;
@property(readwrite,retain)NSTextField* modifierMessage;

-(void)clearFields;

-(IBAction)acceptButtonClicked:(NSButton*)sender;
-(IBAction)cancelButtonClicked:(NSButton*)sender;
-(IBAction)leftButtonClicked:(NSButton*)sender;
-(IBAction)rightButtonClicked:(NSButton*)sender;

@end
