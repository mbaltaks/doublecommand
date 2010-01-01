//
//  KeyCapturePanel.m
//  prefpane
//
//  Created by Tyler Bunnell on 12/31/09.
//  Copyright 2009 Slightly Sane Inc. All rights reserved.
//

#import <Quartz/Quartz.h>

#import "KeyCaptureWindow.h"

#import "KeyRemapEntry.h"
#import "KeyCombo.h"
#import "KeyCodeTransformer.h"

@interface KeyCaptureWindow (Private)
-(void)scanForModifiers;
-(void)displayModifierButtonSelectionPanel:(NSString*)currentModifier;
-(void)shakeWindow;
-(CAKeyframeAnimation*)shakeAnimation:(NSRect)frame;
-(void)displayDefaultMessage;
-(void)displayFromMessage;
-(void)displayToMessage;
-(void)displaySameShortcutMessage;
-(void)displayRemapExists;
@end


@implementation KeyCaptureWindow

@synthesize statusLabel;
@synthesize mapFrom;
@synthesize mapTo;
@synthesize modifierMessage;
@synthesize modifierPanel;

-(void)awakeFromNib
{
  newRemapEntry = [[KeyRemapEntry alloc] init];
  [self displayDefaultMessage];
  [acceptButton setEnabled:NO];
}

-(void)clearFields
{
  [mapFrom setStringValue:@""];
  [mapTo setStringValue:@""];
}

-(void)textFieldFocusChanged:(NSTextField*)textField
{
  if(currentFocus != mapFrom && textField == mapFrom)
  {
    [newRemapEntry setRemapFrom:nil];
    fromRemapSet = NO;
    [self displayFromMessage];
  }
  else if(currentFocus != mapTo && textField == mapTo)
  {
    [newRemapEntry setRemapTo:nil];
    toRemapSet = NO;
    [self displayToMessage];
  }
  [textField setStringValue:@" "];
  currentFocus = textField;
}

-(void)keyDown:(NSEvent*)event
{
  KeyCombo* newCombo = [[[KeyCombo alloc] initWithModifierFlags:[event modifierFlags] keyCode:[event keyCode]] autorelease];
  NSString* comboStringRepresentation = [KeyCodeTransformer stringRepresentationForKeyCombo:newCombo];
  
  BOOL success = YES;
  
  if(currentFocus == mapFrom)
  {
    if(![[newRemapEntry remapTo] isEqualToCombo:newCombo])
    {
      [newRemapEntry setRemapFrom:newCombo];
      fromRemapSet = YES;
    }
    else
    {
      [self displaySameShortcutMessage];
      [self shakeWindow];
      comboStringRepresentation = @"";
      success = NO;
    }

  }
  else
  {
    if(![[newRemapEntry remapFrom] isEqualToCombo:newCombo])
    {
      [newRemapEntry setRemapTo:newCombo];
      toRemapSet = YES;
    }
    else
    {
      [self displaySameShortcutMessage];
      [self shakeWindow];
      comboStringRepresentation = @"";
      success = NO;
    }

  }

  [currentFocus setStringValue:comboStringRepresentation];
  currentFocus = nil;
  
  if(success)
  {
    [self displayDefaultMessage];
    if(fromRemapSet && toRemapSet)
      [acceptButton setEnabled:YES];
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
    {
      [[self delegate] capturePanelAddNewEntry:newRemapEntry];
      [newRemapEntry release];
      newRemapEntry = [[KeyRemapEntry alloc] init];
      [NSApp endSheet:self returnCode:NSOKButton];
    }
    else
    {
      [self shakeWindow];
      [self displayRemapExists];
    }

  }
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
-(void)shakeWindow
{
  NSWindow* window = self;
  [window setAnimations:[NSDictionary dictionaryWithObject:[self shakeAnimation:[window frame]] forKey:@"frameOrigin"]];
	[[window animator] setFrameOrigin:[window frame].origin];
}
-(CAKeyframeAnimation*)shakeAnimation:(NSRect)frame
{
  int numberOfShakes = 4;
  float durationOfShake = 0.5f;
  float vigourOfShake = 0.05f;
  CAKeyframeAnimation *shakeAnimation = [CAKeyframeAnimation animation];
  
  CGMutablePathRef shakePath = CGPathCreateMutable();
  CGPathMoveToPoint(shakePath, NULL, NSMinX(frame), NSMinY(frame));
  int index;
  for (index = 0; index < numberOfShakes; ++index)
  {
    CGPathAddLineToPoint(shakePath, NULL, NSMinX(frame) - frame.size.width * vigourOfShake, NSMinY(frame));
    CGPathAddLineToPoint(shakePath, NULL, NSMinX(frame) + frame.size.width * vigourOfShake, NSMinY(frame));
  }
  CGPathCloseSubpath(shakePath);
  shakeAnimation.path = shakePath;
  shakeAnimation.duration = durationOfShake;
  return shakeAnimation;
}

-(void)displayDefaultMessage
{
  [statusLabel setStringValue:@"Click in a text field to begin capturing."];
}
-(void)displayFromMessage
{
  [statusLabel setStringValue:@"Capturing shortcut to map from."];
}
-(void)displayToMessage
{
  [statusLabel setStringValue:@"Capturing shortcut to map to."];
}
-(void)displaySameShortcutMessage
{
  [statusLabel setStringValue:@"You may not assign a shortcut to itself."];
}
-(void)displayRemapExists
{
  [statusLabel setStringValue:@"This remap already exists."];
}

@end
