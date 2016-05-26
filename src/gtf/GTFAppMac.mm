//
//  GTFAppMac.cpp
//  gtf
//
//  Created by David Gallardo on 23/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "glad.h"
#include "GTFWindow.h"
#include "GTFApp.h"

//GTFOpenGLView interface
@interface GTFOpenGLView : NSOpenGLView
{
}
@end

//------------------------------------------------------------------
// GTFAppDelegate
//------------------------------------------------------------------
@interface GTFAppDelegate : NSObject <NSApplicationDelegate>
{
@public
    GTFApp* cppAppInterface;
}
@end

@implementation GTFAppDelegate



- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
    return YES;
}



- (void)setupMenu
{
    NSMenu *mainMenuBar;
    NSMenu *appMenu;
    NSMenuItem *menuItem;
    
    mainMenuBar = [[NSMenu alloc] init];
    
    appMenu = [[NSMenu alloc] initWithTitle:@"GTF App"];
    menuItem = [appMenu addItemWithTitle:@"Quit IMGUI OSX Sample" action:@selector(terminate:) keyEquivalent:@"q"];
    [menuItem setKeyEquivalentModifierMask:NSCommandKeyMask];
    
    menuItem = [[NSMenuItem alloc] init];
    [menuItem setSubmenu:appMenu];
    
    [mainMenuBar addItem:menuItem];
    
    [appMenu release];
    [NSApp setMainMenu:mainMenuBar];
}

- (void)dealloc
{
    [super dealloc];
}



- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [self setupMenu];
    
    //_window = [self setupWindow: @"Main Window"];
    //_secondaryWindow = [self setupWindow: @"Secondary Window"];
    
    //GTFWindow::createNew("test1");
    //GTFWindow::createNew("test2");
    //GTFWindow::createNew("test3");
    //GTFWindow::createNew("test4");
    
    cppAppInterface->readyToStart();
    
}

@end



//------------------------------------------------------------------
// main
//------------------------------------------------------------------

struct GTFNativeApp
{
    GTFAppDelegate* delegate;
};

int GTFApp::run(int argc, const char * argv[])
{
    [[NSAutoreleasePool alloc] init];
    NSApp = [NSApplication sharedApplication];
    
    m_nativeApp = new GTFNativeApp;
    m_nativeApp->delegate = [[GTFAppDelegate alloc] init];
    m_nativeApp->delegate->cppAppInterface = this;
    
    [[NSApplication sharedApplication] setDelegate:m_nativeApp->delegate];
    [NSApp run];
    return NSApplicationMain(argc, argv);
}
