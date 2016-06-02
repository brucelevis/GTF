//
//  GTFNodeGraph.cpp
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "GTFNodeGraph.h"
#include "GTFErrorHandling.h"

#include <glm/glm.hpp>

#include "imgui.h"

void GTFNodeGraphType::registerNodeType(GTFNodeTypeBase* nodeType)
{
    GTF_ASSERT(nodeType != nullptr, "Node type is NULL");
    
    auto nt = nodeTypeRegistry.find(nodeType->unique_id);

    if(nt == nodeTypeRegistry.end())
    {
        nodeTypeRegistry.insert(std::make_pair(nodeType->unique_id, nodeType));
    }
}

GTFNode* GTFNodeGraph::createNode(GTFUniqueId typeId, int atX, int atY)
{
    auto typeIt = m_graphType->nodeTypeRegistry.find(typeId);
    if(typeIt != m_graphType->nodeTypeRegistry.end())
    {
        GTFNodeTypeBase* nodeType = typeIt->second;
        GTFNode* node = nodeType->createNodeInstance();
        node->nodeId = nodeCounter++;
        
        ImVec2 inputTextSize(0.0f, 0.0f);
        ImVec2 outputText(0.0f, 0.0f);
        
        ImVec2 titleSize = ImGui::CalcTextSize(node->type->displayName.c_str());
        
        titleSize.y *= 3;
        
        for(GTFNodeConnectionDescBase* desc : nodeType->inputConnectionsDesc)
        {
            auto con = desc->createConnectionInstance(true);
            
            ImVec2 textSize = ImGui::CalcTextSize(desc->displayName.c_str());
            inputTextSize.x = std::max<float>(textSize.x, inputTextSize.x);
            
            
            con->posX = 0;
            con->posY = titleSize.y + inputTextSize.y + textSize.y / 2.0f;
            
            inputTextSize.y += textSize.y;
            inputTextSize.y += 4.0f;		// size between text entries
            
            node->inputConnections.push_back(con);
        }
        
        inputTextSize.x += 40.0f;
        float xStart = inputTextSize.x;
        
        float textSizeYOutputs = 0;
        for (GTFNodeConnectionDescBase* desc : nodeType->outputConnectionsDesc)
        {
            ImVec2 textSize = ImGui::CalcTextSize(desc->displayName.c_str());
            inputTextSize.x = std::max<float>(xStart + textSize.x, inputTextSize.x);
            textSizeYOutputs += textSize.y + 4.0f;
        }
        
        inputTextSize.y = std::max(inputTextSize.y, textSizeYOutputs);
        
        node->width = inputTextSize.x;
        node->height = inputTextSize.y + titleSize.y + 4.0f;
        
        inputTextSize.y = 0.0f;
        
        for(GTFNodeConnectionDescBase* desc : nodeType->outputConnectionsDesc)
        {
            auto con = desc->createConnectionInstance(false);
            
            ImVec2 textSize = ImGui::CalcTextSize(desc->displayName.c_str());
            
            con->posX = node->width;
            con->posY = titleSize.y + inputTextSize.y + textSize.y / 2.0f;
            
            inputTextSize.y += textSize.y;
            inputTextSize.y += 4.0f;		// size between text entries
            
            node->outputConnections.push_back(con);
        }
        
        node->posX = atX;
        node->posY = atY;
        
        //node->width = 80;
        //node->height = 40 + glm::max(40.0f, node->inputConnections.size() * 20.0f);
        
        nodeList.push_back(node);
        return node;
    }
    
    return nullptr;
}

void GTFNodeGraph::updateNodes()
{
    for (GTFNode* node : nodeList)
    {
        node->update();
    }
}

GTFNodeGraph::~GTFNodeGraph()
{
    
}
