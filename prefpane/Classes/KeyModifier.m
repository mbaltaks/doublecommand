#import "KeyModifier.h"

static NSString* modifierKeyKey = @"modifierKey";
static NSString* keyLocationKey = @"keyLocation";

@implementation KeyModifier

@synthesize modifierKey;
@synthesize keyLocation;

-(id)initWithModifierKey:(unsigned int)ModifierKey keyLocation:(ModifierKeyLocation)KeyLocation
{
  if((self = [super init]))
  {
    [self setModifierKey:ModifierKey];
    [self setKeyLocation:KeyLocation];
  }
  return self;
}

-(id)initWithCoder:(NSCoder *)decoder
{
  if((self = [super init]))
  {
    [self setModifierKey:[decoder decodeIntForKey:modifierKeyKey]];
    [self setKeyLocation:[decoder decodeIntForKey:keyLocationKey]];
  }
  return self;
}

-(void)encodeWithCoder:(NSCoder *)coder
{
  [coder encodeInt:modifierKey forKey:modifierKeyKey];
  [coder encodeInt:keyLocation forKey:keyLocationKey];
}

-(BOOL)isEqualToKeyModifier:(KeyModifier*)keyModifier
{
  return (([self modifierKey] == [keyModifier modifierKey]) &&
          ([self keyLocation] == [keyModifier keyLocation]));
}

@end
