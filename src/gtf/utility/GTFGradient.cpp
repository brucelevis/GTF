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
    
}

void GTFGradient::reverseMarks()
{
    for(GTFGradientMarkPtr mark : marks)
    {
        mark->position = 1.0f - mark->position;
    }
}

void GTFGradient::addMark(float position, GTFColor color)
{
    position = glm::clamp(position, 0.0f, 1.0f);
    GTFGradientMarkPtr newMark = GTFGradientMark::create();
    newMark->position = position;
    newMark->color = color;
    marks.push_back(newMark);
}

void GTFGradient::removeMark(GTFGradientMarkPtr mark)
{
    marks.remove(mark);
}

GTFColor const GTFGradient::getColorAt(float position)
{
    position = glm::clamp(position, 0.0f, 1.0f);
    
    GTFGradientMarkPtr lower = nullptr;
    GTFGradientMarkPtr upper = nullptr;
    
    for(GTFGradientMarkPtr mark : marks)
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
        return GTFColor(0.0f);
    }
    
    if(upper == lower)
    {
        return upper->color;
    }
    else
    {
        float distance = upper->position - lower->position;
        float delta = (position - lower->position) / distance;
        
        //lerp
        GTFColor finalColor = ((1.0f - delta) * lower->color) + ((delta) * upper->color);
        return finalColor;
    }
}