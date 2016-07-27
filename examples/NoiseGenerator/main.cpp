//
//  main.cpp
//  gtf
//
//  Created by David Gallardo on 23/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include <stdio.h>
#include "NoiseApp.h"

NoiseApp* NoiseApp::instance { nullptr };

int main(int argc, const char * argv[])
{
    gtf::App* app = new NoiseApp();
    return app->run(argc, argv);
}
