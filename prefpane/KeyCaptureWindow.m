//
//  KeyCapturePanel.m
//  prefpane
//
//  Created by Tyler Bunnell on 12/31/09.
//  Copyright 2009 Slightly Sane Inc. All rights reserved.
//

#import "KeyCaptureWindow.h"

#import "KeyRemapEntry.h"


@interface KeyCaptureWindow (Private)
-(void)scanForModifiers;
-(void)displayModifierButtonSelectionPanel:(NSString*)currentModifier;
@end


@implementation KeyCaptureWindow

@synthesize statusLabel;
@synthesize mapFrom;
@synthesize mapTo;
@synthesize modifierMessage;
@synthesize modifierPanel;

-(void)textFieldFocusChanged:(NSTextField*)textField
{
  if(textField == mapFrom)
  {
    //set up to capture into map from
    [mapFrom setStringValue:nil];
  }
  else
  {
    //set up to capture into map to
    [mapTo setStringValue:nil];
  }
}

-(void)scanForModifiers
{
  NSString* currentModifier = nil;
  
  if(modifiers & NSCommandKeyMask)
  {
    currentModifier = @"Command";
    modifiers ^= NSCommandKeyMask;
  }
  else if(modifiers & NSAlternateKeyMask)
  {
    currentModifier = @"Option";
    modifiers ^= NSAlternateKeyMask;
  }
	else if(modifiers & NSControlKeyMask)
  {
    currentModifier = @"Control";
    modifiers ^= NSControlKeyMask;
  }
	else if(modifiers & NSShiftKeyMask)
  {
    currentModifier = @"Shift";
    modifiers ^= NSShiftKeyMask;
  }
  
  [self displayModifierButtonSelectionPanel:currentModifier];
  
}
-(void)displayModifierButtonSelectionPanel:(NSString*)currentModifier
{
  [NSApp beginSheet:modifierPanel
     modalForWindow:self
      modalDelegate:self
     didEndSelector:@selector(modifierPanelDidEnd:returnCode:contextInfo:) 
        contextInfo:nil];
}
-(void)modifierPanelDidEnd:(NSPanel*)sheet returnCode:(int)returnCode contextInfo:(void*)contextInfo
{
  if(returnCode==DCLeftButton)
    NSLog(@"Left");
  else
  {
    NSLog(@"right");
  }
  [modifierPanel orderOut:self];
}


-(IBAction)acceptButtonClicked:(NSButton*)sender
{
  if(fromRemapSet && toRemapSet)
  {
    if([[self delegate] capturePanelCanAddNewEntry:newRemapEntry])
      [[self delegate] capturePanelAddNewEntry:newRemapEntry];
  }
  [NSApp endSheet:self returnCode:NSOKButton];
}
-(IBAction)cancelButtonClicked:(NSButton*)sender
{
  [NSApp endSheet:self returnCode:NSCancelButton];
}

-(IBAction)leftButtonClicked:(NSButton*)sender
{
  [NSApp endSheet:modifierPanel returnCode:DCLeftButton];
}
-(IBAction)rightButtonClicked:(NSButton*)sender
{
  [NSApp endSheet:modifierPanel returnCode:DCRightButton];
}



@end
