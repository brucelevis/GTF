//
//  CalcWindow.h
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include "GTFWindow.h"
#include "CalcNodeGraph.h"

class CalcWindow : public GTFWindow
{
public:
    CalcWindow(const char* title);
    ~CalcWindow(){};
    virtual void frame(double deltaTime) override;
    
    GTFNodeGraphType* calcGraphType;
    CalcNodeGraph* calcGraphInstance;
};
