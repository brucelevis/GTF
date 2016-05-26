//
//  NoiseApp.cpp
//  gtf
//
//  Created by David Gallardo on 26/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "NoiseApp.h"
#include "NoiseWindow.h"

void NoiseApp::readyToStart()
{
    m_window = new NoiseWindow("NoiseGenerator");
}