#import <Cocoa/Cocoa.h>

@protocol NSTextCaptureFieldDelegate <NSObject>
-(void)textFieldFocusChanged:(NSTextField*)view;
@end


@interface NSTextCaptureField : NSTextField
@end
