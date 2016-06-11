//
//  GTFGUIGradientPicker.cpp
//  gtf
//
//  Created by David Gallardo on 30/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "GTFGUIGradientPicker.h"

#include "imgui.h"
#include "imgui_internal.h"

#include <glm/glm.hpp>
#include <iostream>



static const float GRADIENT_BAR_WIDGET_HEIGHT = 25;
static const float GRADIENT_BAR_EDITOR_HEIGHT = 40;
static const float GRADIENT_MARK_DELETE_DIFFY = 40;

bool GTFGUIGradientPicker::displayWidget(bool* showPopup,
                                         GTFGradient* gradient,
                                         GTFGradientMark* & draggingMark,
                                         GTFGradientMark* & selectedMark)
{
    if(!gradient) return false;
    
    bool modified = false;
    
    ImVec2 picker_pos = ImGui::GetCursorScreenPos();
   // ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    float maxWidth = glm::max(250.0f, ImGui::GetContentRegionAvailWidth() - 100.0f);
    
    ImGui::InvisibleButton("gradient_bar", ImVec2(maxWidth, GRADIENT_BAR_WIDGET_HEIGHT));
    
    if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
    {
        if(showPopup)
        {
            *showPopup = true;
            ImGui::SetNextWindowPosCenter();
            
        }
    }
    
    GTFGUIGradientPicker::drawBar(gradient, false, draggingMark, selectedMark, picker_pos, maxWidth, GRADIENT_BAR_WIDGET_HEIGHT);
    
    return showPopupFor(showPopup, gradient, draggingMark, selectedMark);
}

bool GTFGUIGradientPicker::showPopupFor(bool* showPopup,
                                        GTFGradient* gradient,
                                        GTFGradientMark* & draggingMark,
                                        GTFGradientMark* & selectedMark)
{
    if(!gradient) return false;
    if(showPopup && !*showPopup) return false;
    
    bool modified = false;

    ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiSetCond_Always);
    ImGui::SetNextWindowFocus();
    
    if(!ImGui::Begin("Color Gradient Editor", showPopup, ImGuiWindowFlags_NoResize ))
    {
        ImGui::End();
        return false;
    }
    
    ImVec2 picker_pos = ImGui::GetCursorScreenPos();
    picker_pos.x += 10;
    float maxWidth = ImGui::GetContentRegionAvailWidth() - 20;
    float barBottom = picker_pos.y + GRADIENT_BAR_EDITOR_HEIGHT;
    
    ImGui::InvisibleButton("gradient_editor_bar", ImVec2(maxWidth, GRADIENT_BAR_EDITOR_HEIGHT));
    
    if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
    {
        float pos = (ImGui::GetIO().MousePos.x - picker_pos.x) / maxWidth;
        
        GTFColor newMarkCol;
        gradient->getColorAt(pos, newMarkCol);
        gradient->addMark(pos, newMarkCol);
    }
    
    GTFGUIGradientPicker::drawBar(gradient, true, draggingMark, selectedMark, picker_pos, maxWidth, GRADIENT_BAR_EDITOR_HEIGHT);
    
    if(!ImGui::IsMouseDown(0) && draggingMark)
    {
        draggingMark = nullptr;
    }
    
    if(ImGui::IsMouseDragging(0) && draggingMark)
    {
        float increment = ImGui::GetIO().MouseDelta.x / maxWidth;
        
        if(increment != 0.0f)
        {
            draggingMark->position += increment;
            draggingMark->position = glm::clamp(draggingMark->position, 0.0f, 1.0f);
            gradient->refreshCache();
            modified = true;
        }
        
        float diffY = ImGui::GetIO().MousePos.y - barBottom;
        
        if(diffY >= GRADIENT_MARK_DELETE_DIFFY)
        {
            gradient->removeMark(draggingMark);
            draggingMark = nullptr;
            selectedMark = nullptr;
            modified = true;
        }
    }
    
    float pickedColor[4];
    
    if(selectedMark)
    {
        selectedMark->color.asF32A(pickedColor);
    }
    
    bool colorModified = ImGui::ColorPicker3(pickedColor);
    
    if(selectedMark && colorModified)
    {
        selectedMark->color.r = pickedColor[0];
        selectedMark->color.g = pickedColor[1];
        selectedMark->color.b = pickedColor[2];
        modified = true;
        gradient->refreshCache();
    }
    
    ImGui::End();
    
    return modified;
    
}

void GTFGUIGradientPicker::drawBar(GTFGradient* gradient,
                                   bool withMarks,
                                   GTFGradientMark* & draggingMark,
                                   GTFGradientMark* & selectedMark,
                                   struct ImVec2 const & bar_pos,
                                   float maxWidth,
                                   float height)
{
    unsigned int colorAU32 = 0;
    unsigned int colorBU32 = 0;
    float prevX = bar_pos.x;
    float barBottom = bar_pos.y + height;
    GTFGradientMark* prevMark = nullptr;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    
    draw_list->AddRectFilled(ImVec2(bar_pos.x - 2, bar_pos.y - 2),
                             ImVec2(bar_pos.x + maxWidth + 2, barBottom + 2),
                             IM_COL32(100, 100, 100, 255));
    
    if(gradient->marks.size() == 0)
    {
        draw_list->AddRectFilled(ImVec2(bar_pos.x, bar_pos.y),
                                 ImVec2(bar_pos.x + maxWidth, barBottom),
                                 IM_COL32(255, 255, 255, 255));
        
    }
    
    for(auto markIt = gradient->marks.begin(); markIt != gradient->marks.end(); ++markIt )
    {
        GTFGradientMark* mark = *markIt;
        
        float from = prevX;
        float to = prevX = bar_pos.x + mark->position * maxWidth;
        
        if(prevMark == nullptr)
        {
            colorAU32 = mark->color.asHEXABGR(true);
        }
        else
        {
            colorAU32 = prevMark->color.asHEXABGR(true);
        }
        
        colorBU32 = mark->color.asHEXABGR(true);
        
        if(mark->position > 0.0)
        {
            
            draw_list->AddRectFilledMultiColor(ImVec2(from, bar_pos.y),
                                               ImVec2(to, barBottom),
                                               colorAU32, colorBU32, colorBU32, colorAU32);
        }
        
        prevMark = mark;
    }
    
    if(prevMark && prevMark->position < 1.0)
    {
        
        draw_list->AddRectFilledMultiColor(ImVec2(prevX, bar_pos.y),
                                           ImVec2(bar_pos.x + maxWidth, barBottom),
                                           colorBU32, colorBU32, colorBU32, colorBU32);
    }
    
    
    if(withMarks)
    {
        for(auto markIt = gradient->marks.begin(); markIt != gradient->marks.end(); ++markIt )
        {
            GTFGradientMark* mark = *markIt;
            
            if(!selectedMark)
            {
                selectedMark = mark;
            }
            
            float to = bar_pos.x + mark->position * maxWidth;
            
            if(prevMark == nullptr)
            {
                colorAU32 = mark->color.asHEXABGR(true);
            }
            else
            {
                colorAU32 = prevMark->color.asHEXABGR(true);
            }
            
            colorBU32 = mark->color.asHEXABGR(true);
            
        
            
            
            draw_list->AddTriangleFilled(ImVec2(to, bar_pos.y + (height - 6)),
                                                     ImVec2(to - 6, barBottom),
                                                     ImVec2(to + 6, barBottom), IM_COL32(100, 100, 100, 255));
            
            draw_list->AddRectFilled(ImVec2(to - 6, barBottom),
                                                 ImVec2(to + 6, bar_pos.y + (height + 12)),
                                                 IM_COL32(100, 100, 100, 255), 1.0f, 1.0f);
            
            draw_list->AddRectFilled(ImVec2(to - 5, bar_pos.y + (height + 1)),
                                                 ImVec2(to + 5, bar_pos.y + (height + 11)),
                                                 IM_COL32(0, 0, 0, 255), 1.0f, 1.0f);
            
            if(selectedMark == mark)
            {
                draw_list->AddTriangleFilled(ImVec2(to, bar_pos.y + (height - 3)),
                                                         ImVec2(to - 4, barBottom + 1),
                                                         ImVec2(to + 4, barBottom + 1), IM_COL32(0, 255, 0, 255));
                
                draw_list->AddRect(ImVec2(to - 5, bar_pos.y + (height + 1)),
                                               ImVec2(to + 5, bar_pos.y + (height + 11)),
                                               IM_COL32(0, 255, 0, 255), 1.0f, 1.0f);
            }
            
            draw_list->AddRectFilledMultiColor(ImVec2(to - 3, bar_pos.y + (height + 3)),
                                                           ImVec2(to + 3, bar_pos.y + (height + 9)),
                                                           colorBU32, colorBU32, colorBU32, colorBU32);
            
            ImGui::SetCursorScreenPos(ImVec2(to - 6, barBottom));
            ImGui::InvisibleButton("mark", ImVec2(12, 12));
            
            if(ImGui::IsItemHovered())
            {
                if(ImGui::IsMouseClicked(0))
                {
                    selectedMark = mark;
                    draggingMark = mark;
                }
            }
            
            
            prevMark = mark;
        }
    }
    
    ImGui::SetCursorScreenPos(ImVec2(bar_pos.x, bar_pos.y + height + ((withMarks) ? 20.0f : 10.0f)));
}