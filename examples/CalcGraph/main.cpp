//
//  main.cpp
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include <stdio.h>


#include "GTFApp.h"
#include "CalcWindow.h"

class CalcApp : public GTFApp
{
public:
    virtual void readyToStart() override
    {
        m_window = new CalcWindow("CalcGraph");
        registerWindow(m_window);
    }
private:
    GTFWindow* m_window;
};

int main(int argc, const char * argv[])
{
    GTFApp* app = new CalcApp();
    return app->run(argc, argv);
}
