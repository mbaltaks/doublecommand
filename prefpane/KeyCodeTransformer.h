//Thanks to http://code.google.com/p/shortcuterecorder for unicode glyph values.

#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h>

#import "KeyCombo.h"

enum {
  KeyboardTabRightGlyph       = 0x21E5,
  KeyboardTabLeftGlyph        = 0x21E4,
  KeyboardCommandGlyph        = kCommandUnicode,
  KeyboardOptionGlyph         = kOptionUnicode,
  KeyboardShiftGlyph          = kShiftUnicode,
  KeyboardControlGlyph        = kControlUnicode,
  KeyboardReturnGlyph         = 0x2305,
  KeyboardReturnR2LGlyph      = 0x21A9,   
  KeyboardDeleteLeftGlyph     = 0x232B,
  KeyboardDeleteRightGlyph    = 0x2326,   
  KeyboardPadClearGlyph       = 0x2327,
  KeyboardLeftArrowGlyph      = 0x2190,
  KeyboardRightArrowGlyph     = 0x2192,
  KeyboardUpArrowGlyph        = 0x2191,
  KeyboardDownArrowGlyph      = 0x2193,
  KeyboardPageDownGlyph       = 0x21DF,
  KeyboardPageUpGlyph         = 0x21DE,
  KeyboardNorthwestArrowGlyph = 0x2196,
  KeyboardSoutheastArrowGlyph = 0x2198,
  KeyboardEscapeGlyph         = 0x238B,
  KeyboardHelpGlyph           = 0x003F,
  KeyboardUpArrowheadGlyph    = 0x2303,
};

@interface KeyCodeTransformer : NSObject {}
+(NSString*)stringRepresentationForKeyCombo:(KeyCombo*)keyCode;
@end
