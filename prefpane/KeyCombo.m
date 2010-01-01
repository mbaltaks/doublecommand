//
//  KeyCode.m
//  prefpane
//
//  Created by Tyler Bunnell on 12/31/09.
//  Copyright 2009 Slightly Sane Inc. All rights reserved.
//

#import "KeyCombo.h"


static NSString* modifierFlagsKey = @"modifierFlags";
static NSString* keyCodeKey = @"keyCode";

@implementation KeyCombo

@synthesize modifierFlags;
@synthesize keyCode;

-(id)initWithModifierFlags:(unsigned int)ModifierFlags keyCode:(unsigned int)KeyCode
{
  if((self = [super init]))
  {
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
  return (([self modifierFlags] == [combo modifierFlags]) &&
          ([self keyCode] == [combo keyCode]));
}

@end
