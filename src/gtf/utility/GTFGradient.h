//
//  GTFGradient.h
//  gtf
//
//  Created by David Gallardo on 30/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include <list>


struct GTFGradientMark
{
    float color[3];
    float position; //0 to 1
};

class GTFGradient
{
public:
    GTFGradient();
    void addMark(float position, float* color);
    void removeMark(GTFGradientMark* mark);
    void getColorAt(float position, float* color);
    
    std::list<GTFGradientMark*> marks;
};
