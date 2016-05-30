//
//  GTFGradient.h
//  gtf
//
//  Created by David Gallardo on 30/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include <list>
#include <memory>
#include "GTFColor.h"
#include "GTFLangUtils.h"


typedef std::shared_ptr<class GTFGradientMark> GTFGradientMarkPtr;
class GTFGradientMark
{
public:
    static GTFGradientMarkPtr create() { return GTFGradientMarkPtr(new GTFGradientMark); }
    GTFColor color;
    float position; //0 to 1
private:
    GTF_DEFAULT_CTORS_EMPTY_IMPL(GTFGradientMark)
};

class GTFGradient
{
public:
    GTFGradient();
    ~GTFGradient();
    void addMark(float position, GTFColor color);
    void removeMark(GTFGradientMarkPtr mark);
    void reverseMarks();
    GTFColor const getColorAt(float position);
    
    std::list<GTFGradientMarkPtr> marks;
};
