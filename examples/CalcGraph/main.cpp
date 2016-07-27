//
//  main.cpp
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include <stdio.h>
#include <gtf/App.h>
#include "CalcWindow.h"

class CalcApp : public gtf::App
{
public:
    virtual void readyToStart() override
    {
        m_window = new CalcWindow("CalcGraph");
        registerWindow(m_window);
        m_window->setVisible(true);
    }
private:
    gtf::Window* m_window;
};

int main(int argc, const char * argv[])
{
    gtf::App* app = new CalcApp();
    return app->run(argc, argv);
}
