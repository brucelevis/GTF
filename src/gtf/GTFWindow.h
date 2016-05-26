//
//  GTCWindow.h
//  gtf
//
//  Created by David Gallardo on 23/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once
#include <memory>

class GTFWindow
{
public:
    GTFWindow(){};
    GTFWindow(const char* title);    
    virtual void frame(){};
public:
    //window properties
    bool m_mousePressed[3] = { false, false, false };
    float m_mouseCoords[2] {0,0};
    unsigned long m_lastClock;
    unsigned int m_windowWidth, m_windowHeight;
    unsigned int m_backingWidth, m_backingHeight;
    
private:
    struct GTFNativeWindow* m_nativeWindow;
    
};