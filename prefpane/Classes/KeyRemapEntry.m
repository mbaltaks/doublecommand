#import "KeyRemapEntry.h"

#import "KeyCombo.h"


static NSString* remapFromKey = @"remapFrom";
static NSString* remapToKey = @"remapTo";

@interface KeyRemapEntry (Private)
-(NSString*)stringRepresentationForKeyCode:(KeyCombo*)keyCode;
@end

@implementation KeyRemapEntry

@synthesize remapFrom;
@synthesize remapTo;

-(id)initWithRemapFrom:(KeyCombo*)RemapFrom remapTo:(KeyCombo*)RemapTo
{
  if((self = [super init]))
  {
    [self setRemapFrom:RemapFrom];
    [self setRemapTo:RemapTo];
  }
  return self;
}
-(id)initWithCoder:(NSCoder *)decoder
{
  if((self = [super init]))
  {
    [self setRemapFrom:[decoder decodeObjectForKey:remapFromKey]];
    [self setRemapTo:[decoder decodeObjectForKey:remapToKey]];
  }
  return self;
}
-(void)encodeWithCoder:(NSCoder *)coder
{
  [coder encodeObject:remapFrom forKey:remapFromKey];
  [coder encodeObject:remapTo forKey:remapToKey];
}

@end
