//
//  NoiseWindow.cpp
//  gtf
//
//  Created by David Gallardo on 26/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "NoiseWindow.h"

#include "glad.h"

void NoiseWindow::frame()
{
    float red = (m_mouseCoords[0] / float(m_windowWidth));
    float green = (m_mouseCoords[1] / float(m_windowHeight));
    glClearColor(red, green, 0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}