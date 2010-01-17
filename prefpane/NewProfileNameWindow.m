#import "NewProfileNameWindow.h"
#import "WindowShaker.h"


@implementation NewProfileNameWindow

@synthesize captureWindowProfileName;
@synthesize delegate;

-(IBAction)acceptButtonClicked:(NSButton*)sender
{
  if(([[captureWindowProfileName stringValue] length] > 0) && 
     [delegate profileNameWindowCanAddNewEntry:[captureWindowProfileName stringValue]])
  {
    [delegate profileNameWindowAddNewEntry:[captureWindowProfileName stringValue]];
    [NSApp endSheet:self returnCode:NSOKButton];
  }
  else
  {
    [WindowShaker shakeWindow:self];
  }

}
-(IBAction)cancelButtonClicked:(NSButton*)sender
{
  [NSApp endSheet:self returnCode:NSCancelButton];
}

-(void)resetWindow
{
  [captureWindowProfileName setStringValue:@""];
}


@end
