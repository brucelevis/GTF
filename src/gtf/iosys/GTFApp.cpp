//
//  App.cpp
//  gtf
//
//  Created by David Gallardo on 23/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "GTFApp.h"
#include "GTFWindow.h"

#include <glfw/glfw3.h>

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int GTFApp::run(int argc, const char * argv[])
{
    //do preparation
    
    if (!glfwInit())
    {
        return -1;
    }
    
    glfwSetErrorCallback(error_callback);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
    
    readyToStart();
    
    bool quit = false;
    
    while(!quit)
    {
        for(GTFWindow* window : m_registeredWindows)
        {
            window->preFrame(0.016);
            window->frame(0.016);
            window->postFrame(0.016);
            quit |= window->mustQuitApp();
        }
    }
    
    glfwTerminate();
    
    return 0;
}

void GTFApp::registerWindow(class GTFWindow* window)
{
    m_registeredWindows.push_back(window);
}