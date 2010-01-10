#import <Cocoa/Cocoa.h>

@class KeyCombo;

@interface KeyRemapEntry : NSObject <NSCoding>
{
  KeyCombo* remapFrom;
  KeyCombo* remapTo;
}

@property(readwrite,retain,nonatomic) KeyCombo* remapFrom;
@property(readwrite,retain,nonatomic) KeyCombo* remapTo;

-(id)initWithRemapFrom:(KeyCombo*)RemapFrom remapTo:(KeyCombo*)RemapTo;

@end
