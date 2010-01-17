#import <Cocoa/Cocoa.h>

#import "NSTextCaptureField.h"

@class KeyRemapEntry;

@protocol KeyCapturePanelDelegate <NSObject>
-(BOOL)capturePanelCanAddNewEntry:(KeyRemapEntry*)newEntry;
-(void)capturePanelAddNewEntry:(KeyRemapEntry*)newEntry;
@end


@interface KeyCaptureWindow : NSWindow <NSTextCaptureFieldDelegate>
{
  NSTextField* currentFocus;
  NSString* defaultModifierMessage;
  
  unsigned int modifiers;
  unsigned int previousFlagCount;
  unsigned int previousModifierFlags;
  
  BOOL fromRemapSet;
  BOOL toRemapSet;
  
  KeyRemapEntry* newRemapEntry;
  NSMutableArray* remapFromModifierKeys;
  NSMutableArray* remapToModifierKeys;
  
  IBOutlet NSTextField* statusLabel;
  IBOutlet NSTextField* mapFrom;
  IBOutlet NSTextField* mapTo;
    
  IBOutlet NSButton* acceptButton;
}

@property(readwrite,retain) NSTextField* statusLabel;
@property(readwrite,retain) NSTextField* mapFrom;
@property(readwrite,retain) NSTextField* mapTo;

-(void)clearFields;

-(IBAction)acceptButtonClicked:(NSButton*)sender;
-(IBAction)cancelButtonClicked:(NSButton*)sender;

@end
