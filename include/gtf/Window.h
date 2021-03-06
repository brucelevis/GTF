//
//  GTCWindow.h
//  gtf
//
//  Created by David Gallardo on 23/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include <memory>
#include <string>

namespace gtf 
{
class Window
{
public:
    //Window(){};
    Window(const char* title, unsigned int width, unsigned int height);
    virtual ~Window();
    virtual void preFrame(double deltaTime);
    virtual void frame(double deltaTime);
    virtual void postFrame(double deltaTime);
    virtual void setVisible(bool visible);
    
    virtual void mouseMove(double x, double y);
    virtual void mouseButton(unsigned int button, bool pressed);
    virtual void mouseScroll(double xoffset, double yoffset);
    virtual void keyEvent(int key, int scancode, int action, int mods);
    virtual void charInputEvent(unsigned int charCode);
    virtual void resize(int newWidth, int newHeight);
    virtual void fileDrop(int count, const char** paths);
    
    virtual bool wantToClose();
    virtual void stopClosing();
    
	void invalidateLocalRenderContextObjects();

public:
    //window properties
    bool m_mousePressed[3] = { false, false, false };
    float m_mouseCoords[2] {0,0};
    unsigned long m_lastClock;
    unsigned int m_windowWidth, m_windowHeight;
    unsigned int m_backingWidth, m_backingHeight;
    
protected:
    bool m_mustQuitApp { false };
    std::string m_title;
    
private:
    struct GTFNativeWindow* m_nativeWindow;
    struct ImGuiContext* m_imguiContext;
    
};
} //namespace gtf