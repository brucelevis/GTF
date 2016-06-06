//
//  main.cpp
//  gtf
//
//  Created by David Gallardo on 23/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include <stdio.h>


#include "GTFApp.h"
#include "NoiseWindow.h"

class NoiseApp : public GTFApp
{
public:
    virtual void readyToStart()
    {
        m_window = new NoiseWindow("NoiseGenerator");
        registerWindow(m_window);
    }
private:
    GTFWindow* m_window;
};

int main(int argc, const char * argv[])
{
    GTFApp* app = new NoiseApp();
    return app->run(argc, argv);
}
