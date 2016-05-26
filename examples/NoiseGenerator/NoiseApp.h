//
//  NoiseApp.h
//  gtf
//
//  Created by David Gallardo on 26/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "GTFApp.h"
#include "GTFWindow.h"

class NoiseApp : public GTFApp
{
public:
    virtual void readyToStart();
private:
    GTFWindow* m_window;
};
