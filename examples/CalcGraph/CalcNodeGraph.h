//
//  CalcNodeGraph.h
//  gtf
//
//  Created by David Gallardo on 02/06/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include "CalcNodes.h"
#include "imgui.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum DragState
{
	DragState_Default,
	DragState_Hover,
	DragState_BeginDrag,
	DragState_Draging,
	DragState_Connect,
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct DragNode
{
	ImVec2 pos;
	GTFNodeConnectionBase* con;
};



class CalcNodeGraph : public GTFNodeGraph
{
public:
    CalcNodeGraph(GTFNodeGraphType* type ) : GTFNodeGraph(type){};
    void updateGUI() override;
    GTFNode* selectedNode { nullptr };
    
    
private:
    void drawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2, int STEPS);
    bool isConnectorHovered(GTFNodeConnectionBase* c, ImVec2 offset);
    GTFNodeConnectionBase* getHoverCon(ImVec2 offset, ImVec2* pos);
    void updateDraging(ImVec2 offset);
    void displayNode(ImDrawList* drawList, ImVec2 offset, GTFNode* node, int& node_selected);
    GTFNode* findNodeByCon(GTFNodeConnectionBase* findCon);
    void renderLines(ImDrawList* drawList, ImVec2 offset);
    
    DragNode dragNode;
    DragState dragState { DragState_Default };
};