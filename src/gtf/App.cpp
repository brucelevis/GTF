//
//  App.cpp
//  gtf
//
//  Created by David Gallardo on 23/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "gtf/App.h"
#include "gtf/Window.h"
#include "ImGuiSetup.h"

#include <glfw/glfw3.h>

namespace gtf 
{
    
App* GAPP(nullptr);

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

App::App()
{
	GAPP = this;
}

App::~App()
{

}


int App::run(int argc, const char * argv[])
{
    //do preparation
    
    if (!glfwInit())
    {
        return -1;
    }
    
    glfwSetErrorCallback(error_callback);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
    
    readyToStart();
    
    if(m_registeredWindows.size() == 0)
    {
        glfwTerminate();
        return 2;
    }
        
    bool quit = false;
    
    std::list<Window*> toClose;
    
    while(!quit)
    {
        for(Window* window : m_registeredWindows)
        {
            window->preFrame(0.016);
            window->frame(0.016);
            window->postFrame(0.016);
            
            if(window->wantToClose())
            {
                toClose.push_back(window);
            }
        }
        
        for(Window* window : toClose)
        {
            m_registeredWindows.remove(window);
            
			window->invalidateLocalRenderContextObjects();

            if(m_registeredWindows.size() == 0)
            {
                ImGuiGL3_InvalidateSharedDeviceObjects();
            }
            
            quit |= (window == m_mainWindow);
            
            delete window;
        }
        
        toClose.clear();
        
        if(quit)
        {
            for(Window* window : m_registeredWindows)
            {
                delete window;
            }
            
            m_registeredWindows.clear();
        }
        
        quit |= (m_registeredWindows.size() == 0);
        
        glfwPollEvents();
    }
    
    glfwTerminate();
    
    return 0;
}

void App::registerWindow(class Window* window)
{
    m_registeredWindows.push_back(window);
    if(m_mainWindow == nullptr)
        m_mainWindow = window;
}

} //gtf