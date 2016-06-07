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
class GTFApp
{
public:
    GTFApp();
    virtual ~GTFApp();
    virtual void readyToStart() = 0;
    int run(int argc, const char * argv[]);
    void registerWindow(class GTFWindow* window);
    GTFWindow* getMainWindow() const { return m_mainWindow; }
    void setMainWindow(GTFWindow* mw) { m_mainWindow = mw; }
    
protected:
    std::list<class GTFWindow*> m_registeredWindows;
    class GTFWindow* m_mainWindow;
};

extern GTFApp* GTFAPP;