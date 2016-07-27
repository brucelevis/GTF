//
//  App.h
//  gtf
//
//  Created by David Gallardo on 23/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once
#include <memory>
#include <list>

namespace gtf 
{
	
class App
{
public:
    App();
    virtual ~App();
    virtual void readyToStart() = 0;
    int run(int argc, const char * argv[]);
    void registerWindow(class Window* window);
    Window* getMainWindow() const { return m_mainWindow; }
    void setMainWindow(Window* mw) { m_mainWindow = mw; }
    
protected:
    std::list<class Window*> m_registeredWindows;
    class Window* m_mainWindow;
};

extern App* GAPP;

} //namespace gtf