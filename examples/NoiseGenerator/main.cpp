//
//  main.cpp
//  gtf
//
//  Created by David Gallardo on 23/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include <stdio.h>

#include "NoiseApp.h"

int main(int argc, const char * argv[])
{
    GTFApp* app = new NoiseApp();
    return app->run(argc, argv);
}
