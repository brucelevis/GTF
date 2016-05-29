//
//  App.h
//  gtf
//
//  Created by David Gallardo on 23/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once
#include <memory>

class GTFApp
{
public:
    virtual void readyToStart() = 0;
    int run(int argc, const char * argv[]);

private:
    //MacOSX
    struct GTFNativeApp* m_nativeApp;
};