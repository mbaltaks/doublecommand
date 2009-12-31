#import <PreferencePanes/PreferencePanes.h>

#import "KeyCaptureWindow.h"


@class RemapListController;

@interface PreferencePaneController : NSPreferencePane <KeyCapturePanelDelegate>
{
  NSMutableArray* remapList;
  IBOutlet RemapListController* listController;
  IBOutlet KeyCaptureWindow* captureWindow;
}

@property(readwrite,retain)RemapListController* listController; 
@property(readwrite,retain)KeyCaptureWindow* captureWindow;

-(IBAction)activateDoubleCommandButtonClicked:(NSButton*)sender;
-(IBAction)deactivateDoubleCommandButtonClicked:(NSButton*)sender;
-(IBAction)persistForUserButtonClicked:(NSButton*)sender;
-(IBAction)persistForSystemButtonClicked:(NSButton*)sender;

-(IBAction)addNewEntryButtonClicked:(NSButton*)sender;
-(IBAction)deleteEntryButtonClicked:(NSButton*)sender;
-(IBAction)clearRemapListButtonClicked:(NSButton*)sender;
-(IBAction)saveRemapListItemClicked:(NSMenuItem*)sender;
-(IBAction)loadRemapListItemClicked:(NSMenuItem*)sender;

@end
