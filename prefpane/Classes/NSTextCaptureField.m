#import "NSTextCaptureField.h"


@implementation NSTextCaptureField

-(void)mouseDown:(NSEvent*)event
{
  [[self delegate] textFieldFocusChanged:self];
}

@end
