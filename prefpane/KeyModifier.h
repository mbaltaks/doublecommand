#import <Cocoa/Cocoa.h>


typedef enum
{
  ModifierKeyLocationLeftSide = 0,
  ModifierKeyLocationRightSide
} ModifierKeyLocation;

@interface KeyModifier : NSObject <NSCoding>
{
  unsigned int modifierKey;
  ModifierKeyLocation keyLocation;
}

-(id)initWithModifierKey:(unsigned int)ModifierKey keyLocation:(ModifierKeyLocation)KeyLocation;

@property(readwrite,assign,nonatomic)unsigned int modifierKey;
@property(readwrite,assign,nonatomic)ModifierKeyLocation keyLocation;

-(BOOL)isEqualToKeyModifier:(KeyModifier*)keyModifier;

@end
