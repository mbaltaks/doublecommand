//
//  NSTextCaptureField.h
//  prefpane
//
//  Created by Tyler Bunnell on 1/1/10.
//  Copyright 2010 Slightly Sane Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol NSTextCaptureFieldDelegate <NSObject>
-(void)textFieldFocusChanged:(NSTextField*)view;
@end


@interface NSTextCaptureField : NSTextField
@end
