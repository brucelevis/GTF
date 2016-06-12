//
//  imgui_color_gradient.h
//  imgui extension
//
//  Created by David Gallardo on 11/06/16.

/*
 
 Usage:
 
 ImGradient gradient;
 
 ::BUTTON::
 if(ImGui::GradientButton(&gradient))
 {
    //set show editor flag to true/false
 }
 
 ::EDITOR::
 static ImGradientMark* draggingMark = nullptr;
 static ImGradientMark* selectedMark = nullptr;
 
 bool updated = ImGui::GradientEditor(&gradient, draggingMark, selectedMark);
 
 ::GET A COLOR::
 float color[3];
 gradient.getColorAt(0.3f, color); //position from 0 to 1
 
 ::MODIFY GRADIENT WITH CODE::
 gradient.getMarks().clear();
 gradient.addMark(0.0f, ImColor(0.2f, 0.1f, 0.0f));
 gradient.addMark(0.7f, ImColor(120, 200, 255));
 
 */

#pragma once

#include "imgui.h"

#include <list>

struct ImGradientMark
{
    float color[4];
    float position; //0 to 1
};

class ImGradient
{
public:
    ImGradient();
    ~ImGradient();
    
    void getColorAt(float position, float* color) const;
    void addMark(float position, ImColor const color);
    void removeMark(ImGradientMark* mark);
    void refreshCache();
    std::list<ImGradientMark*> & getMarks(){ return m_marks; }
private:
    void computeColorAt(float position, float* color) const;
    std::list<ImGradientMark*> m_marks;
    float m_cachedValues[256 * 3];
};

namespace ImGui
{
    bool GradientButton(ImGradient* gradient);
    
    bool GradientEditor(ImGradient* gradient,
                        ImGradientMark* & draggingMark,
                        ImGradientMark* & selectedMark);
    

}
