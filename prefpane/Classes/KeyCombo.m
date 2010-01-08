//
//  KeyCode.m
//  prefpane
//
//  Created by Tyler Bunnell on 12/31/09.
//  Copyright 2009 Slightly Sane Inc. All rights reserved.
//

#import "KeyCombo.h"

#import "KeyModifier.h"

static NSString* modifierFlagsKey = @"modifierFlags";
static NSString* keyCodeKey = @"keyCode";

@implementation KeyCombo

@synthesize modifierKeys;
@synthesize modifierFlags;
@synthesize keyCode;

-(id)initWithModifierKeys:(NSArray*)ModifierKeys modifierFlags:(unsigned int)ModifierFlags keyCode:(unsigned int)KeyCode
{
  if((self = [super init]))
  {
    [self setModifierKeys:ModifierKeys];
    [self setModifierFlags:ModifierFlags];
    [self setKeyCode:KeyCode];    
  }
  return self;
}

-(id)initWithCoder:(NSCoder *)decoder
{
  if((self = [super init]))
  {
    [self setModifierFlags:[decoder decodeIntForKey:modifierFlagsKey]];
    [self setKeyCode:[decoder decodeIntForKey:keyCodeKey]];
  }
  return self;
}

-(void)encodeWithCoder:(NSCoder *)coder
{
  [coder encodeInt:modifierFlags forKey:modifierFlagsKey];
  [coder encodeInt:keyCode forKey:keyCodeKey];
}

-(BOOL)isEqualToCombo:(KeyCombo*)combo
{
  //If the flags and the keycode are the same, check the location of the mod keys.
  if(([self modifierFlags] == [combo modifierFlags]) &&
     ([self keyCode] == [combo keyCode]))
  {
    for(KeyModifier* keyModifierLeft in modifierKeys)
    {
      for(KeyModifier* keyModifierRight in [combo modifierKeys])
      {
        if(![keyModifierLeft isEqualToKeyModifier:keyModifierRight])
          return NO;
      }
    }
  }
  else
  {
    return NO;
  }

  return YES;
}

@end
