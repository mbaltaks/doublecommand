//
//  PersistenceController.h
//  prefpane
//
//  Created by Tyler Bunnell on 1/4/10.
//  Copyright 2010 Slightly Sane Inc. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface PersistenceController : NSObject
{
  NSString* preferencePaneFilePath;
  NSMutableDictionary* preferences;
}

-(void)setObject:(id)object forKey:(NSString*)key;
-(id)getObjectForKey:(NSString*)key;
-(void)persistPreferencePaneSettings;


@end
