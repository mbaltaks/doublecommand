#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>

#import "KeyCombo.h"
#import "KeyModifier.h"


enum
{
  KeyboardCommandGlyph        = kCommandUnicode,
  KeyboardOptionGlyph         = kOptionUnicode,
  KeyboardShiftGlyph          = kShiftUnicode,
  KeyboardControlGlyph        = kControlUnicode
};

enum
{
  ShiftKeyCode = 56,
  ControlKeyCode = 59,
  OptionKeyCode = 58,
  CommandKeyCode = 55,
  ShiftKeyCodeRight = 60,
  ControlKeyCodeRight = 62,
  OptionKeyCodeRight = 61,
  CommandKeyCodeRight = 54
};

@interface KeyCodeTransformer : NSObject {}
+(NSString*)stringRepresentationForKeyCombo:(KeyCombo*)keyCode;
+(NSString*)stringRepresentationForModifiers:(unsigned int)modifiers;
+(ModifierKeyLocation)locationForKeyCode:(unsigned int)keyCode;
@end
