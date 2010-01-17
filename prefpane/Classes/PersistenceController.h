#import <Cocoa/Cocoa.h>


@interface PersistenceController : NSObject
{
  NSString* preferencePaneFilePath;
  NSMutableDictionary* preferences;
}

-(void)setObject:(id)object forKey:(NSString*)key;
-(id)getObjectForKey:(NSString*)key;
-(void)removeObjectForKey:(NSString*)key;
-(void)persistPreferencePaneSettings;


@end
