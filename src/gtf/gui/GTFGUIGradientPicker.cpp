//
//  GTFGUIGradientPicker.cpp
//  gtf
//
//  Created by David Gallardo on 30/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "GTFGUIGradientPicker.h"

#include "imgui.h"
#include <glm/glm.hpp>

void GTFGUIGradientPicker::displayWidget(bool* showPopup, GTFGradient* gradient)
{
    if(!gradient) return;
    
    ImVec2 picker_pos = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    float maxWidth = glm::max(250.0f, ImGui::GetContentRegionAvailWidth() - 100.0f);
    
    float prevX = picker_pos.x;
    
    unsigned int colorAU32 = 0;
    unsigned int colorBU32 = 0;
    
    GTFGradientMark* prevMark = nullptr;
    for(auto markIt = gradient->marks.begin(); markIt != gradient->marks.end(); ++markIt )
    {
        GTFGradientMark* mark = *markIt;
        
        float from = prevX;
        float to = prevX = picker_pos.x + mark->position * maxWidth;
        
        
        if(prevMark == nullptr)
        {
            colorAU32 = mark->color.asHEXABGR(true);
            //colorAU32 = IM_COL32(mark->color.r * 255, mark->color.g * 255, mark->color.b * 255, 255);
        }
        else
        {
            colorAU32 = prevMark->color.asHEXABGR(true);
            //colorAU32 = IM_COL32(prevMark->color.r * 255, prevMark->color.g * 255, prevMark->color.b * 255, 255);
        }
        
        colorBU32 = mark->color.asHEXABGR(true);
        //colorBU32 = IM_COL32(mark->color.r * 255, mark->color.g * 255, mark->color.b * 255, 255);
        
        if(mark->position > 0.0)
        {
        
            draw_list->AddRectFilledMultiColor(ImVec2(from, picker_pos.y),
                                               ImVec2(to, picker_pos.y + 30),
                                               colorAU32, colorBU32, colorBU32, colorAU32);
        }
        
        prevMark = mark;
    }
    
    if(prevMark && prevMark->position < 1.0)
    {
        
        draw_list->AddRectFilledMultiColor(ImVec2(prevX, picker_pos.y),
                                           ImVec2(picker_pos.x + maxWidth, picker_pos.y + 30),
                                           colorAU32, colorBU32, colorBU32, colorAU32);
    }
    
    
    showPopupFor(showPopup, gradient);
}

void GTFGUIGradientPicker::showPopupFor(bool* showPopup, GTFGradient* gradient)
{
    if(!gradient) return;
    if(showPopup && !*showPopup) return;
    
}