//
//  CalcWindow.h
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include <gtf/Window.h>
#include "CalcNodeGraph.h"

class CalcWindow : public gtf::Window
{
public:
    CalcWindow(const char* title);
    ~CalcWindow(){};
    virtual void frame(double deltaTime) override;
    
    gtf::NodeGraphType* calcGraphType;
    CalcNodeGraph* calcGraphInstance;
};
