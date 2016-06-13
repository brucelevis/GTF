//
//  NoiseApp.h
//  gtf
//
//  Created by David Gallardo on 11/06/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include "GTFApp.h"
#include "NoiseWindow.h"

class NoiseApp : public GTFApp
{
public:
    static NoiseApp* instance;
    
    virtual void readyToStart()
    {
        instance = this;
		noiseWindow = new NoiseWindow("NoiseGenerator");
        registerWindow(noiseWindow);
        setMainWindow(noiseWindow);
		noiseWindow->postSetMainInit();
    }

    NoiseWindow* noiseWindow;
};
