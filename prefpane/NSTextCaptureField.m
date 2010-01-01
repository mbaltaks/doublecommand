//
//  NSTextCaptureField.m
//  prefpane
//
//  Created by Tyler Bunnell on 1/1/10.
//  Copyright 2010 Slightly Sane Inc. All rights reserved.
//

#import "NSTextCaptureField.h"


@implementation NSTextCaptureField

-(void)mouseDown:(NSEvent*)event
{
  [[self delegate] textFieldFocusChanged:self];
}

@end
