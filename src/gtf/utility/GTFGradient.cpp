//
//  GTFGradient.cpp
//  gtf
//
//  Created by David Gallardo on 30/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "GTFGradient.h"

#include <string.h>
#include <glm/glm.hpp>

GTFGradient::GTFGradient()
{
    float black[3] = {0.2,0.2,0.0};
    addMark(0.0f, black);
    
    float yellow[3] = {0.5,0.5,0.1};
    addMark(0.2f, yellow);
    
    float white[3] = {1,1,0.8};
    addMark(1.0f, white);
}

void GTFGradient::addMark(float position, float* color)
{
    GTFGradientMark* newMark = new GTFGradientMark();
    newMark->position = position;
    memcpy(newMark->color, color, sizeof(float) * 3);
    marks.push_back(newMark);
}

void GTFGradient::removeMark(GTFGradientMark* mark)
{
    marks.remove(mark);
    delete mark;
}

void GTFGradient::getColorAt(float position, float* color)
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
        return;
    }
    
    if(upper == lower)
    {
        memcpy(color, upper->color, sizeof(float) * 3);
        return;
    }
    else
    {
        glm::vec3 colorA(lower->color[0], lower->color[1], lower->color[2]);
        glm::vec3 colorB(upper->color[0], upper->color[1], upper->color[2]);
        float distance = upper->position - lower->position;
        float delta = (position - lower->position) / distance;
        
        //lerp
        glm::vec3 finalColor = ((1.0f - delta) * colorA) + ((delta) * colorB);
        color[0] = finalColor.x;
        color[1] = finalColor.y;
        color[2] = finalColor.z;
    }
}