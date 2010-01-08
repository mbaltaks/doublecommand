//
//  PersistenceController.m
//  prefpane
//
//  Created by Tyler Bunnell on 1/4/10.
//  Copyright 2010 Slightly Sane Inc. All rights reserved.
//

#import "PersistenceController.h"


@implementation PersistenceController

-(id)init
{
  if((self = [super init]))
  {    
    NSString* fileName = [NSString stringWithFormat:@"%@.plist",[[NSBundle bundleForClass:[self class]] bundleIdentifier]];
    //filePath = [[NSBundle bundleForClass:[self class]] bundlePath];
    preferencePaneFilePath = @"Users";
    NSRange search = [preferencePaneFilePath rangeOfString:@"Users"];
    if(search.location == NSNotFound)
      preferencePaneFilePath = @"/Library/Preferences";
    else
      preferencePaneFilePath = [@"~/Library/Preferences" stringByExpandingTildeInPath];
    
    preferencePaneFilePath = [[NSString stringWithFormat:@"%@/%@",preferencePaneFilePath,fileName] retain];
    preferences = [[NSDictionary dictionaryWithContentsOfFile:preferencePaneFilePath] retain];
    if(preferences == nil)
      preferences = [[NSMutableDictionary alloc] init];
  }
  return self;
}
-(void)dealloc
{
  [preferencePaneFilePath release];
  [preferences release];
  [super dealloc];
}

-(void)setObject:(id)object forKey:(NSString*)key
{
  [preferences setObject:object forKey:key];
}
-(id)getObjectForKey:(NSString*)key
{
  return [preferences objectForKey:key];
}

-(void)persistPreferencePaneSettings
{
  [preferences writeToFile:preferencePaneFilePath atomically:YES];
}


@end
