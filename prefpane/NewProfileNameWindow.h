#import <Cocoa/Cocoa.h>

@protocol NewProfileNameWindowDelegate <NSObject>
-(BOOL)profileNameWindowCanAddNewEntry:(NSString*)newEntry;
-(void)profileNameWindowAddNewEntry:(NSString*)newEntry;
@end

@interface NewProfileNameWindow : NSWindow
{
  id<NewProfileNameWindowDelegate> delegate;
  IBOutlet NSTextField* captureWindowProfileName;
}

@property(readwrite,retain) NSTextField* captureWindowProfileName;
@property(readwrite,retain) id<NewProfileNameWindowDelegate> delegate;

-(void)resetWindow;

-(IBAction)acceptButtonClicked:(NSButton*)sender;
-(IBAction)cancelButtonClicked:(NSButton*)sender;

@end
