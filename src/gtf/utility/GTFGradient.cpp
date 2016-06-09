//
//  GTFGradient.cpp
//  gtf
//
//  Created by David Gallardo on 30/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "GTFGradient.h"
#include "GTFColor.h"

#include <string.h>
#include <glm/glm.hpp>

GTFGradient::GTFGradient()
{
    addMark(0.0f, GTFCOLORS::BLACK);
    addMark(1.0f, GTFCOLORS::WHITE);
    
}

GTFGradient::~GTFGradient()
{
	for (GTFGradientMark* mark : marks)
	{
		delete mark;
	}
}

void GTFGradient::reverseMarks()
{
    for(GTFGradientMark* mark : marks)
    {
        mark->position = 1.0f - mark->position;
    }
}

void GTFGradient::addMark(float position, GTFColor color)
{
    position = glm::clamp(position, 0.0f, 1.0f);
	GTFGradientMark* newMark = new GTFGradientMark();
    newMark->position = position;
    newMark->color = color;
    marks.push_back(newMark);
    refreshCache();
}

void GTFGradient::removeMark(GTFGradientMark* mark)
{
    marks.remove(mark);
    refreshCache();
}

void GTFGradient::getColorAt(float position, GTFColor& color) const
{
    position = glm::clamp(position, 0.0f, 1.0f);
    
    int cachePos = position * 255;
    color = m_cachedValues[cachePos];
}

void GTFGradient::computeColorAt(float position, GTFColor& color) const
{
    position = glm::clamp(position, 0.0f, 1.0f);
    
    GTFGradientMark* lower = nullptr;
    GTFGradientMark* upper = nullptr;
    
    for(GTFGradientMark* mark : marks)
    {
        if(mark->position < position)
        {
            if(!lower || lower->position < mark->position)
            {
                lower = mark;
            }
        }
        
        if(mark->position >= position)
        {
            if(!upper || upper->position > mark->position)
            {
                upper = mark;
            }
        }
    }
    
    if(upper && !lower)
    {
        lower = upper;
    }
    else if(!upper && lower)
    {
        upper = lower;
    }
    else if(!lower && !upper)
    {
        //color[0] = color[1] = color[2] = 0;
        //color = GTFColor(0.0f);
        color.r = color.g = color.b = 0.0f;
    }
    
    if(upper == lower)
    {
        color = upper->color;
    }
    else
    {
        float distance = upper->position - lower->position;
        float delta = (position - lower->position) / distance;
        
        //lerp
        color = ((1.0f - delta) * lower->color) + ((delta) * upper->color);
    }
}

void GTFGradient::refreshCache()
{
    for(int i = 0; i < 256; ++i)
    {
        computeColorAt(i/255.0f, m_cachedValues[i]);
    }
}