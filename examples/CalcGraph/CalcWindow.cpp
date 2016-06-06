//
//  CalcWindow.cpp
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "CalcWindow.h"
#include "CalcNodes.h"

#include "imgui.h"

CalcWindow::CalcWindow(const char* title) : GTFWindow(title, 1040, 720)
{
    calcGraphType = new GTFNodeGraphType(GTF_UID("CalcGraphType"));
    
    auto numberDesc = new GTFNodeConnectionDesc<GTFNodeConnectionI32>(GTF_UID("NumberConnection"), "Value");
    auto inputADesc = new GTFNodeConnectionDesc<GTFNodeConnectionI32>(GTF_UID("NumberInputConnectionA"), "InputA");
    auto inputBDesc = new GTFNodeConnectionDesc<GTFNodeConnectionI32>(GTF_UID("NumberInputConnectionB"), "InputB");
    
    GTFNodeTypeBase* nodeType = nullptr;
    nodeType = new GTFNodeType<CalcNumberNode>(GTF_UID("CalcNumberNode"), "Number");
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    nodeType = new GTFNodeType<CalcAddNode>(GTF_UID("CalcAddNode"), "Add");
    nodeType->inputConnectionsDesc.push_back(inputADesc);
    nodeType->inputConnectionsDesc.push_back(inputBDesc);
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    nodeType = new GTFNodeType<CalcSubstractNode>(GTF_UID("CalcSubstractNode"), "Substract");
    nodeType->inputConnectionsDesc.push_back(inputADesc);
    nodeType->inputConnectionsDesc.push_back(inputBDesc);
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    nodeType = new GTFNodeType<CalcMultiplyNode>(GTF_UID("CalcMultiplyNode"), "Multiply");
    nodeType->inputConnectionsDesc.push_back(inputADesc);
    nodeType->inputConnectionsDesc.push_back(inputBDesc);
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    nodeType = new GTFNodeType<CalcDivideNode>(GTF_UID("CalcDivideNode"), "Divide");
    nodeType->inputConnectionsDesc.push_back(inputADesc);
    nodeType->inputConnectionsDesc.push_back(inputBDesc);
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    calcGraphInstance = new CalcNodeGraph(calcGraphType);
    
    RHI->setClearColor(0.3, 0.3, 0.28, 1.0);
}

void CalcWindow::frame(double deltaTime)
{
    calcGraphInstance->updateNodes();
    
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoCollapse;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoResize;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_HorizontalScrollbar;
    
    
    int propertiesWith = 250;
    
    ImGui::SetNextWindowPos(ImVec2(m_windowWidth - propertiesWith - 20, 20), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(propertiesWith, m_windowHeight - 40), ImGuiSetCond_Always);
    
    ImGui::Begin("NodeProperties", nullptr, flags	);
    {
        ImGui::Text("Node Properties");
        ImGui::Separator();
        
        if(calcGraphInstance->selectedNodes.size() == 1)
        {
            CalcNode* node = CalcNode::CAST(calcGraphInstance->selectedNodes.front());
            if(node && node->type == ECalcNodeType::CNT_NUMBER)
            {
                
                node->dirty = ImGui::InputInt("Value", &node->number);
            }
            
        }
    }
    ImGui::End();
    
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(m_windowWidth - propertiesWith - 60, m_windowHeight - 40), ImGuiSetCond_Always);
    
    if(ImGui::Begin("Graph", nullptr, flags))
    {
        calcGraphInstance->updateGUI();
    }
    ImGui::End();
    
    
    
    // Rendering
    RHI->viewport(0, 0, m_windowWidth, m_windowHeight);
    RHI->clearColorAndDepthBuffers();
}
