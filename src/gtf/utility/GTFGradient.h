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


struct GTFGradientMark
{
	GTFColor color;
	float position; //0 to 1	
};

class GTFGradient
{
public:
    GTFGradient();
    ~GTFGradient();
    void addMark(float position, GTFColor color);
    void removeMark(GTFGradientMark* mark);
    void reverseMarks();
    void refreshCache();
    void getColorAt(float position, GTFColor& color) const;
    void computeColorAt(float position, GTFColor& color) const;
    
	std::list<GTFGradientMark*> const & getMarksRef() const { return marks; };
    
private:
	std::list<GTFGradientMark*> marks;
    
    GTFColor m_cachedValues[256];
};
