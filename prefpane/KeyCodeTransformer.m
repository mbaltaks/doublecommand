#import "KeyCodeTransformer.h"


@interface KeyCodeTransformer (Private)
+(NSString*)stringRepresentationForModifiers:(unsigned int)modifiers;
+(NSString*)stringRepresentationForGlyph:(unsigned int)glyph;
+(NSString*)stringRepresentationForKeyCode:(unsigned int)keyCode;
@end

static NSDictionary* keyCodeAssociations;

@implementation KeyCodeTransformer

-(id)init
{
  return nil;
}

+(NSString*)stringRepresentationForKeyCombo:(KeyCombo*)keyCombo
{
  @synchronized(self)
  {
    if(keyCodeAssociations == nil)
    {
      NSBundle* bundle = [NSBundle bundleForClass:[self class]];
      NSString* dictionaryFilePath = [bundle pathForResource:@"KeyCodeAssociations" ofType:@"plist"];
      keyCodeAssociations = [NSDictionary dictionaryWithContentsOfFile:dictionaryFilePath];
    }
  }
  return [NSString stringWithFormat:@"%@%@",
          [self stringRepresentationForModifiers:[keyCombo modifierFlags]],
          [self stringRepresentationForKeyCode:[keyCombo keyCode]]];
}

+(NSString*)stringRepresentationForModifiers:(unsigned int)modifiers
{
  NSMutableString* constructor = [NSMutableString stringWithCapacity:5];
	
	if(modifiers & NSCommandKeyMask)
    [constructor appendString:[self stringRepresentationForGlyph:KeyboardCommandGlyph]];
  if(modifiers & NSAlternateKeyMask)
    [constructor appendString:[self stringRepresentationForGlyph:KeyboardOptionGlyph]];
	if(modifiers & NSControlKeyMask)
    [constructor appendString:[self stringRepresentationForGlyph:KeyboardControlGlyph]];
	if(modifiers & NSShiftKeyMask)
    [constructor appendString:[self stringRepresentationForGlyph:KeyboardShiftGlyph]];
	
	return [NSString stringWithString:constructor];
}
+(NSString*)stringRepresentationForGlyph:(unsigned int)glyph
{
  return [NSString stringWithFormat:@"%C",glyph];
}

+(NSString*)stringRepresentationForKeyCode:(unsigned int)keyCode
{
  return [keyCodeAssociations objectForKey:[NSString stringWithFormat:@"%i",keyCode]];
}


@end
