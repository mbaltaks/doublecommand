#import <Quartz/Quartz.h>

#import "WindowShaker.h"


@interface WindowShaker ()
+(CAKeyframeAnimation*)shakeAnimation:(NSRect)frame;
@end

@implementation WindowShaker

+(void)shakeWindow:(NSWindow*)window
{
  [window setAnimations:[NSDictionary dictionaryWithObject:[WindowShaker shakeAnimation:[window frame]] forKey:@"frameOrigin"]];
	[[window animator] setFrameOrigin:[window frame].origin];  
}
+(CAKeyframeAnimation*)shakeAnimation:(NSRect)frame
{
  int numberOfShakes = 4;
  float durationOfShake = 0.5f;
  float vigourOfShake = 0.05f;
  CAKeyframeAnimation *shakeAnimation = [CAKeyframeAnimation animation];
  
  CGMutablePathRef shakePath = CGPathCreateMutable();
  CGPathMoveToPoint(shakePath, NULL, NSMinX(frame), NSMinY(frame));
  int index;
  for (index = 0; index < numberOfShakes; ++index)
  {
    CGPathAddLineToPoint(shakePath, NULL, NSMinX(frame) - frame.size.width * vigourOfShake, NSMinY(frame));
    CGPathAddLineToPoint(shakePath, NULL, NSMinX(frame) + frame.size.width * vigourOfShake, NSMinY(frame));
  }
  CGPathCloseSubpath(shakePath);
  shakeAnimation.path = shakePath;
  shakeAnimation.duration = durationOfShake;
  return shakeAnimation;
}


@end
