//
//  NoiseWindow.h
//  gtf
//
//  Created by David Gallardo on 26/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "GTFWindow.h"

class NoiseWindow : public GTFWindow
{
public:
    NoiseWindow(const char* title) : GTFWindow(title){};
    virtual void frame();
};