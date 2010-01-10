#import <Cocoa/Cocoa.h>


@interface KeyCombo : NSObject <NSCoding>
{
  NSArray* modifierKeys;
  unsigned int modifierFlags;
  unsigned int keyCode;
}

-(id)initWithModifierKeys:(NSArray*)ModifierKeys modifierFlags:(unsigned int)modifierFlags keyCode:(unsigned int)KeyCode;

@property(readwrite,retain,nonatomic) NSArray* modifierKeys;
@property(readwrite,assign,nonatomic) unsigned int modifierFlags;
@property(readwrite,assign,nonatomic) unsigned int keyCode;

-(BOOL)isEqualToCombo:(KeyCombo*)combo;

@end
