//
//  GTFWindowMac.cpp
//  gtf
//
//  Created by David Gallardo on 23/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "GTFWindow.h"

#import <Cocoa/Cocoa.h>

#include "imgui.h"
#include "GTFOpenGLRHI.h"


#include "IMGuiSetup.h"

/*
#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
static float highDpiScale = 1.0;
static bool g_mousePressed[3] = { false, false, false };
static float g_mouseCoords[2] {0,0};
static clock_t g_lastClock;
static unsigned int g_windowWidth, g_windowHeight;
static unsigned int g_backingWidth, g_backingHeight;*/

//------------------------------------------------------------------
// GTFOpenGLView
//------------------------------------------------------------------



@interface GTFOpenGLView : NSOpenGLView
{
@public
    
    GTFWindow* cppWindowInterface;
    NSTimer *animationTimer;
}
-(id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat *)format cppParent:(GTFWindow*)cppWin;
@end

NSWindow* setupWindow(GTFWindow* cppWindow, NSString* title, unsigned int width, unsigned int height);

struct GTFNativeWindow
{
    NSWindow* window;
    GTFOpenGLView* view;
};

GTFWindow::GTFWindow(const char* title, unsigned int width, unsigned int height)
{
    m_nativeWindow = new GTFNativeWindow;
    NSString* nsTitle = [NSString stringWithFormat:@"%s", title];
    m_nativeWindow->window = setupWindow(this, nsTitle, width, height);
    m_nativeWindow->view = [m_nativeWindow->window contentView];
}

NSWindow* setupWindow(GTFWindow* cppWindow, NSString* title, unsigned int width, unsigned int height)
{
    NSScreen *mainScreen = [NSScreen mainScreen];
    NSRect screenRect = [mainScreen visibleFrame];
    
    NSWindow* targetWindow;

    CGFloat xPos = (screenRect.size.width/2) - (width/2);
    CGFloat yPos = (screenRect.size.height/2) - (height/2);

    
    NSRect viewRect = NSMakeRect(xPos, yPos, width, height);
    
    targetWindow = [[NSWindow alloc] initWithContentRect:viewRect styleMask:NSTitledWindowMask|NSMiniaturizableWindowMask|NSResizableWindowMask|NSClosableWindowMask backing:NSBackingStoreBuffered defer:YES];
    [targetWindow setTitle:title];
    [targetWindow setOpaque:YES];
    
    [targetWindow makeKeyAndOrderFront:NSApp];
    
    NSOpenGLPixelFormatAttribute attrs[] =
    {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFADepthSize, 32,
        // Must specify the 3.2 Core Profile to use OpenGL 3.2
        NSOpenGLPFAOpenGLProfile,
        NSOpenGLProfileVersion3_2Core,
        0
    };
    
    NSOpenGLPixelFormat *format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
    GTFOpenGLView *view = [[GTFOpenGLView alloc] initWithFrame:targetWindow.frame pixelFormat:format cppParent:cppWindow];
    [format release];
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
    if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
        [view setWantsBestResolutionOpenGLSurface:YES];
#endif /*MAC_OS_X_VERSION_MAX_ALLOWED*/
    
    [targetWindow setContentView:view];
    
    if ([view openGLContext] == nil)
    {
        NSLog(@"No OpenGL Context!");
    }
    
    
    [[view openGLContext] makeCurrentContext];
    
    RHI->init();
    
    /*if(!gladLoadGL())
    {
        NSLog(@"gladLoadGL :: Something went wrong!\n");
    }
    else
    {
        NSLog(@"OpenGL Version %d.%d\n", GLVersion.major, GLVersion.minor);
    }*/
    
    [targetWindow orderFrontRegardless];

    return targetWindow;
}



@implementation GTFOpenGLView

-(void)animationTimerFired:(NSTimer*)timer
{
    [self setNeedsDisplay:YES];
}

-(id)initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat *)format cppParent:(GTFWindow*)cppWin
{
    self = [super initWithFrame:frameRect pixelFormat:format];
    if (self)
    {
        cppWindowInterface = cppWin;
        cppWindowInterface->m_lastClock = clock();
    }
    return(self);
}

- (void)prepareOpenGL
{
    [super prepareOpenGL];
    
#ifndef DEBUG
    GLint swapInterval = 1;
    [[self openGLContext] setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
    if (swapInterval == 0)
    {
        NSLog(@"Error: Cannot set swap interval.");
    }
#endif
}

- (void)drawView
{
    //[self lockFocus];
    [[self openGLContext] makeCurrentContext];
    NSWindow *mainWindow = [self window];
    NSPoint mousePosition = [mainWindow mouseLocationOutsideOfEventStream];
    
    //convert to View
    mousePosition = [self convertPoint:mousePosition fromView:nil];
    
    cppWindowInterface->m_mouseCoords[0] = mousePosition.x;
    cppWindowInterface->m_mouseCoords[1] = mousePosition.y - 1.0f;
    
    clock_t thisclock = clock();
    unsigned long clock_delay = thisclock - cppWindowInterface->m_lastClock;
    //double milliseconds = clock_delay * 1000.0f / CLOCKS_PER_SEC;
    double deltaTime = clock_delay / double(CLOCKS_PER_SEC);
    //draw here

    cppWindowInterface->m_lastClock = thisclock;
    
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)cppWindowInterface->m_windowWidth, (float)cppWindowInterface->m_windowHeight);
    io.DeltaTime = deltaTime;
    io.MousePos = ImVec2((float)cppWindowInterface->m_mouseCoords[0], (float)cppWindowInterface->m_mouseCoords[1]);
    io.MouseDown[0] = cppWindowInterface->m_mousePressed[0];
    io.MouseDown[1] = cppWindowInterface->m_mousePressed[1];
    io.MouseDown[2] = cppWindowInterface->m_mousePressed[2];
    ImGui::NewFrame();
    
    cppWindowInterface->preFrame(deltaTime);
    cppWindowInterface->frame(deltaTime);
    cppWindowInterface->postFrame(deltaTime);
    
    ImGui::Render();
    [[self openGLContext] flushBuffer];
    
    
    //[self unlockFocus];
    
    if (!animationTimer)
    {
        animationTimer = [[NSTimer scheduledTimerWithTimeInterval:0.01666666 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES] retain];
    }
}

-(void)setViewportRect:(NSRect)bounds
{
    RHI->viewport(0, 0, bounds.size.width, bounds.size.height);
    cppWindowInterface->m_windowWidth = bounds.size.width;
	cppWindowInterface->m_windowHeight = bounds.size.height;
    
    if (cppWindowInterface->m_windowHeight == 0)
    {
        cppWindowInterface->m_windowHeight = 1;
	}
    
    
    ImGui::GetIO().DisplaySize = ImVec2((float)bounds.size.width, (float)bounds.size.height);
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
    if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
    {
        NSRect backing = [self convertRectToBacking:bounds];
        cppWindowInterface->m_backingWidth = backing.size.width;
        cppWindowInterface->m_backingHeight= backing.size.height;
        if (cppWindowInterface->m_backingHeight == 0)
        {
            cppWindowInterface->m_backingHeight = cppWindowInterface->m_windowHeight * 2;
        }
    }
    else
#endif /*MAC_OS_X_VERSION_MAX_ALLOWED*/
    {
        cppWindowInterface->m_backingWidth = cppWindowInterface->m_windowWidth;
        cppWindowInterface->m_backingHeight= cppWindowInterface->m_windowHeight;
    }
}

-(void)reshape
{
    [self setViewportRect:self.bounds];
    [[self openGLContext] update];
    [self drawView];
}

-(void)drawRect:(NSRect)bounds
{
    [self drawView];
}

#pragma mark -

-(BOOL)acceptsFirstResponder
{
    return(YES);
}

-(BOOL)becomeFirstResponder
{
    return(YES);
}

-(BOOL)resignFirstResponder
{
    return(YES);
}

// Flips coordinate system upside down on Y
-(BOOL)isFlipped
{
    return(YES);
}

#pragma mark Mouse and Key Events.

static bool mapKeymap(int* keymap)
{
    if(*keymap == NSUpArrowFunctionKey)
        *keymap = ImGuiKey_LeftArrow;
    else if(*keymap == NSDownArrowFunctionKey)
        *keymap = ImGuiKey_DownArrow;
    else if(*keymap == NSLeftArrowFunctionKey)
        *keymap = ImGuiKey_LeftArrow;
    else if(*keymap == NSRightArrowFunctionKey)
        *keymap = ImGuiKey_RightArrow;
    else if(*keymap == NSHomeFunctionKey)
        *keymap = ImGuiKey_Home;
    else if(*keymap == NSEndFunctionKey)
        *keymap = ImGuiKey_End;
    else if(*keymap == NSDeleteFunctionKey)
        *keymap = ImGuiKey_Delete;
    else if(*keymap == 25) // SHIFT + TAB
        *keymap = 9; // TAB
    else
        return true;
    
    return false;
}

static void resetKeys()
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[io.KeyMap[ImGuiKey_A]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_C]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_V]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_X]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_Y]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_Z]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_LeftArrow]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_RightArrow]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_Tab]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_UpArrow]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_DownArrow]] = false;
    io.KeysDown[io.KeyMap[ImGuiKey_Tab]] = false;
}

-(void)keyUp:(NSEvent *)theEvent
{
    NSString *str = [theEvent characters];
    ImGuiIO& io = ImGui::GetIO();
    int len = (int)[str length];
    for(int i = 0; i < len; i++)
    {
        int keymap = [str characterAtIndex:i];
        mapKeymap(&keymap);
        if(keymap < 512)
        {
            io.KeysDown[keymap] = false;
        }
    }
}

-(void)keyDown:(NSEvent *)theEvent
{
    NSString *str = [theEvent characters];
    ImGuiIO& io = ImGui::GetIO();
    int len = (int)[str length];
    for(int i = 0; i < len; i++)
    {
        int keymap = [str characterAtIndex:i];
        if(mapKeymap(&keymap) && !io.KeyCtrl)
            io.AddInputCharacter(keymap);
            if(keymap < 512)
            {
                if(io.KeyCtrl)
                {
                    // we must reset in case we're pressing a sequence
                    // of special keys while keeping the command pressed
                    resetKeys();
                }
                io.KeysDown[keymap] = true;
            }
    }
}

- (void)flagsChanged:(NSEvent *)event
{
    unsigned int flags;
    flags = [event modifierFlags] & NSDeviceIndependentModifierFlagsMask;
    ImGuiIO& io = ImGui::GetIO();
    bool wasKeyShift= io.KeyShift;
    bool wasKeyCtrl = io.KeyCtrl;
    io.KeyShift     = flags & NSShiftKeyMask;
    io.KeyCtrl      = flags & NSCommandKeyMask;
    bool keyShiftReleased = wasKeyShift && !io.KeyShift;
    bool keyCtrlReleased  = wasKeyCtrl  && !io.KeyCtrl;
    if(keyShiftReleased || keyCtrlReleased)
    {
        // we must reset them as we will not receive any
        // keyUp event if they where pressed during shift or command
        resetKeys();
    }
}

-(void)mouseDown:(NSEvent *)theEvent
{
    int button = (int)[theEvent buttonNumber];
    cppWindowInterface->m_mousePressed[button] = true;
}

-(void)rightMouseDown:(NSEvent *)theEvent
{
    int button = (int)[theEvent buttonNumber];
    cppWindowInterface->m_mousePressed[button] = true;
}

-(void)otherMouseDown:(NSEvent *)theEvent
{
    int button = (int)[theEvent buttonNumber];
    cppWindowInterface->m_mousePressed[button] = true;
}

-(void)mouseUp:(NSEvent *)theEvent
{
    int button = (int)[theEvent buttonNumber];
    cppWindowInterface->m_mousePressed[button] = false;
}

-(void)rightMouseUp:(NSEvent *)theEvent
{
    int button = (int)[theEvent buttonNumber];
    cppWindowInterface->m_mousePressed[button] = false;
}

-(void)otherMouseUp:(NSEvent *)theEvent
{
    int button = (int)[theEvent buttonNumber];
    cppWindowInterface->m_mousePressed[button] = false;
}

- (void)scrollWheel:(NSEvent *)event
{
    double deltaX, deltaY;
    
#if MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
    if (floor(NSAppKitVersionNumber) > NSAppKitVersionNumber10_6)
    {
        deltaX = [event scrollingDeltaX];
        deltaY = [event scrollingDeltaY];
        
        if ([event hasPreciseScrollingDeltas])
        {
            deltaX *= 0.1;
            deltaY *= 0.1;
        }
    }
    else
#endif /*MAC_OS_X_VERSION_MAX_ALLOWED*/
    {
        deltaX = [event deltaX];
        deltaY = [event deltaY];
    }
    
    if (fabs(deltaX) > 0.0 || fabs(deltaY) > 0.0)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += deltaY * 0.1f;
    }
}

-(void)dealloc
{
    
    [super dealloc];
}

@end

