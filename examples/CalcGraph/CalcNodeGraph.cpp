//
//  CalcNodeGraph.cpp
//  gtf
//
//  Created by David Gallardo on 02/06/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "CalcNodeGraph.h"

#include "imgui.h"

const float NODE_SLOT_RADIUS = 6.0f;
const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x+rhs.x, lhs.y+rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x-rhs.x, lhs.y-rhs.y); }




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CalcNodeGraph::drawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2, int STEPS = 32)
{
    ImVec2 diff = p2 - p1;
    
    if(diff.x < 0)
    {
        diff.x = abs(diff.x) * 3;
        if(diff.x > 800)
            STEPS = 64;
    }
    else
    {
        diff.x = 0;
    }
    
	ImVec2 t1 = ImVec2(+180.0f + diff.x, 0.0f);
	ImVec2 t2 = ImVec2(+180.0f + diff.x, 0.0f);
    
	for (int step = 0; step <= STEPS; step++)
	{
		float t = (float)step / (float)STEPS;
		float h1 = +2*t*t*t - 3*t*t + 1.0f;
		float h2 = -2*t*t*t + 3*t*t;
		float h3 =    t*t*t - 2*t*t + t;
		float h4 =    t*t*t -   t*t;
		drawList->PathLineTo(ImVec2(h1*p1.x + h2*p2.x + h3*t1.x + h4*t2.x, h1*p1.y + h2*p2.y + h3*t1.y + h4*t2.y));
	}
    
	drawList->PathStroke(ImColor(200,200,100), false, 3.0f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CalcNodeGraph::isConnectorHovered(GTFNodeConnectionBase* c, ImVec2 offset)
{
	ImVec2 mousePos = ImGui::GetIO().MousePos;
	ImVec2 conPos = offset + ImVec2(c->posX, c->posY);
    
	float xd = mousePos.x - conPos.x;
	float yd = mousePos.y - conPos.y;
    
	return ((xd * xd) + (yd *yd)) < (NODE_SLOT_RADIUS * NODE_SLOT_RADIUS);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GTFNodeConnectionBase* CalcNodeGraph::getHoverCon(ImVec2 offset, ImVec2* pos)
{
	for (GTFNode* node : nodeList)
	{
		ImVec2 nodePos = ImVec2(node->posX, node->posY) + offset;
        
		for (GTFNodeConnectionBase* con : node->inputConnections)
		{
			if (isConnectorHovered(con, nodePos))
			{
				*pos = nodePos + ImVec2(con->posX, con->posY);
				return con;
			}
		}
        
		for (GTFNodeConnectionBase* con : node->outputConnections)
		{
			if (isConnectorHovered(con, nodePos))
			{
				*pos = nodePos + ImVec2(con->posX, con->posY);
				return con;
			}
		}
	}
	
	dragNode.con = 0;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CalcNodeGraph::updateDraging(ImVec2 offset)
{
	switch (dragState)
	{
		case DragState_Default:
		{
			ImVec2 pos;
			GTFNodeConnectionBase* con = getHoverCon(offset, &pos);
            
			if (con)
			{
				dragNode.con = con;
				dragNode.pos = pos;
				dragState = DragState_Hover;
				return;
			}
            
			break;
		}
            
		case DragState_Hover:
		{
			ImVec2 pos;
			GTFNodeConnectionBase* con = getHoverCon(offset, &pos);
            
			// Make sure we are still hovering the same node
            
			if (con != dragNode.con)
			{
				dragNode.con = 0;
				dragState = DragState_Default;
				return;
			}
            
			if (ImGui::IsMouseClicked(0) && dragNode.con)
				dragState = DragState_Draging;
            
			break;
		}
            
		case DragState_BeginDrag:
		{
			break;
		}
            
		case DragState_Draging:
		{
    		ImDrawList* drawList = ImGui::GetWindowDrawList();
            
			drawList->ChannelsSetCurrent(0); // Background
            
            
            if(dragNode.con->isInput)
            {
                drawHermite(drawList, ImGui::GetIO().MousePos, dragNode.pos);
            }
            else
            {
                drawHermite(drawList, dragNode.pos, ImGui::GetIO().MousePos);
            }
            
			if (!ImGui::IsMouseDown(0))
			{
				ImVec2 pos;
				GTFNodeConnectionBase* con = getHoverCon(offset, &pos);
                
				// Make sure we are still hovering the same node
                
				if (con == dragNode.con)
				{
					dragNode.con = 0;
					dragState = DragState_Default;
					return;
				}
                
				// Lets connect the nodes.
				// TODO: Make sure we connect stuff in the correct way!
                // TODO2: fixed at node type level, add rules for custom connections?
                if(con->isInput != dragNode.con->isInput)
                {
                    if(con->isInput)
                    {
                        con->input = dragNode.con;
                        con->isDirty = true;
                        dragNode.con->output.push_back(con);
                    }
                    else
                    {
                        con->output.push_back(dragNode.con);
                        dragNode.con->input = con;
                        dragNode.con->isDirty = true;
                    }
                    
                    
                }
                
                dragNode.con = 0;
                dragState = DragState_Default;
			}
            
			break;
		}
            
		case DragState_Connect:
		{
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CalcNodeGraph::displayNode(ImDrawList* drawList, ImVec2 offset, GTFNode* node, int& node_selected)
{
    int node_hovered_in_scene = -1;
    bool open_context_menu = false;
    
	ImGui::PushID(node->nodeId);
	ImVec2 node_rect_min = offset + ImVec2(node->posX, node->posY);
    
	// Display node contents first
	//drawList->ChannelsSetCurrent(1); // Foreground
    
    ImGui::SetCursorScreenPos(node_rect_min);
	ImGui::InvisibleButton("node", ImVec2(node->width, node->height));
    
	if (ImGui::IsItemHovered())
	{
		node_hovered_in_scene = node->nodeId;
		open_context_menu |= ImGui::IsMouseClicked(1);
        
        if(ImGui::IsMouseClicked(0))
        {
            selectedNode = node;
        }
	}
    
    bool node_moving_active = false;
	if (ImGui::IsItemActive() && !dragNode.con)
		node_moving_active = true;
    
    // Draw text bg area
    ImVec2 node_rect_max = node_rect_min + ImVec2(node->width, node->height);
    ImVec2 titleArea = node_rect_max;
	titleArea.y = node_rect_min.y + 30.0f;
    ImU32 node_bg_color = (node_hovered_in_scene == node->nodeId) ? ImColor(75,75,75) : ImColor(60,60,60);
	drawList->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
	drawList->AddRectFilled(node_rect_min + ImVec2(1,1), titleArea, ImColor(100,0,0), 4.0f);
	drawList->AddRect(node_rect_min, node_rect_max, ImColor(100,100,100), 4.0f);
    
	bool old_any_active = ImGui::IsAnyItemActive();
    
	// Draw title in center
    
	ImVec2 textSize = ImGui::CalcTextSize(node->type->displayName.c_str());
    
	ImVec2 pos = node_rect_min + NODE_WINDOW_PADDING;
	pos.x = node_rect_min.x + (node->width / 2) - textSize.x / 2;
    
	ImGui::SetCursorScreenPos(pos);
	//ImGui::BeginGroup(); // Lock horizontal position
	ImGui::Text("%s", node->type->displayName.c_str());
    
	//ImGui::SliderFloat("##value", &node->Value, 0.0f, 1.0f, "Alpha %.2f");
	//float dummy_color[3] = { node->Pos.x / ImGui::GetWindowWidth(), node->Pos.y / ImGui::GetWindowHeight(), fmodf((float)node->ID * 0.5f, 1.0f) };
	//ImGui::ColorEdit3("##color", &dummy_color[0]);
    
	// Save the size of what we have emitted and weither any of the widgets are being used
	bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
	//node->size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
	
    
	// Display node box
	//drawList->ChannelsSetCurrent(0); // Background
    
	offset.y += 40.0f;
    
	offset = node_rect_min;
    
    ImVec2 textOffset;
	textOffset.x = node_rect_min.x;
	textOffset.y = node_rect_min.y;
    textOffset.y += 40.0f;
    
	for (GTFNodeConnectionBase* con : node->inputConnections)
	{
		ImGui::SetCursorScreenPos(textOffset + ImVec2(10.0f, 0));
		
        auto numberCon = GTFNodeConnectionI32::CAST(con);
        
        if(numberCon)
        {
            ImGui::Text("%d", numberCon->data);
        }
        else
        {
            ImGui::Text("%s", con->desc->displayName.c_str());
        }
        
		ImColor conColor = ImColor(150, 150, 150);
        
		if (isConnectorHovered(con, node_rect_min))
			conColor = ImColor(200, 200, 200);
        
		drawList->AddCircleFilled(node_rect_min + ImVec2(con->posX, con->posY), NODE_SLOT_RADIUS, conColor);
        
		textOffset.y += textSize.y + 2.0f;
	}
    
	textOffset = node_rect_min;
	textOffset.y += 40.0f;
    
	for (GTFNodeConnectionBase* con : node->outputConnections)
	{
		textSize = ImGui::CalcTextSize(con->desc->displayName.c_str());
        
		ImGui::SetCursorScreenPos(textOffset + ImVec2(con->posX - (textSize.x + 10.0f), 0));
        
        auto numberCon = GTFNodeConnectionI32::CAST(con);
        
        if(numberCon)
        {
            ImGui::Text("%d", numberCon->data);
        }
        else
        {
            ImGui::Text("%s", con->desc->displayName.c_str());
        }
        
		ImColor conColor = ImColor(150, 150, 150);
        
		if (isConnectorHovered(con, node_rect_min))
			conColor = ImColor(200, 200, 200);
        
		drawList->AddCircleFilled(node_rect_min + ImVec2(con->posX, con->posY), NODE_SLOT_RADIUS, conColor);
        
		textOffset.y += textSize.y + 2.0f;
	}
    
    
	//for (int i = 0; i < node->outputConnections.size(); ++i)
	//	drawList->AddCircleFilled(offset + node->outputSlotPos(i), NODE_SLOT_RADIUS, ImColor(150,150,150,150));
    
	if (node_widgets_active || node_moving_active)
		node_selected = node->nodeId;
    
	if (node_moving_active && ImGui::IsMouseDragging(0))
    {
		node->posX += ImGui::GetIO().MouseDelta.x;
        node->posY += ImGui::GetIO().MouseDelta.y;
    }
    
	//ImGui::EndGroup();
    
	ImGui::PopID();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: Ugly fix: me

GTFNode* CalcNodeGraph::findNodeByCon(GTFNodeConnectionBase const * findCon)
{
	for (GTFNode* node : nodeList)
	{
		for (GTFNodeConnectionBase* con : node->inputConnections)
		{
			if (con == findCon)
				return node;
		}
        
		for (GTFNodeConnectionBase* con : node->outputConnections)
		{
			if (con == findCon)
				return node;
		}
	}
    
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CalcNodeGraph::renderLines(ImDrawList* drawList, ImVec2 offset)
{
	for (GTFNode* node : nodeList)
	{
		for (GTFNodeConnectionBase* con : node->inputConnections)
		{
			if (!con->input)
				continue;
            
			GTFNode* targetNode = findNodeByCon(con->input);
            
			if (!targetNode)
				continue;
            
			drawHermite(drawList,
                        offset + ImVec2(targetNode->posX, targetNode->posY) + ImVec2(con->input->posX, con->input->posY),
                        offset + ImVec2(node->posX, node->posY) + ImVec2(con->posX, con->posY));
		}
	}
}

void CalcNodeGraph::updateGUI()
{
    bool open_context_menu = false;
    int node_hovered_in_list = -1;
    int node_hovered_in_scene = -1;
    
	static int node_selected = -1;
    static ImVec2 scrolling = ImVec2(0.0f, 0.0f);
    
    ImGui::SameLine();
    ImGui::BeginGroup();
    
    // Create our child canvas
    //ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1,1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(40,40,40,200));
    ImGui::BeginChild("scrolling_region", ImVec2(0,0), true, ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoMove);
    ImGui::PushItemWidth(120.0f);
    
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->ChannelsSplit(2);
    //ImVec2 offset = ImGui::GetCursorScreenPos() - scrolling;
    
	//displayNode(draw_list, scrolling, s_emittable, node_selected);
	//displayNode(draw_list, scrolling, s_emitter, node_selected);
    
    ImVec2 curOffset = ImGui::GetCursorScreenPos();
    
    if(selectedNode != nullptr && !ImGui::IsMouseClicked(0))
    {
        
        nodeList.remove(selectedNode);
        nodeList.push_back(selectedNode);
        
        
        selectedNode = nullptr;
    }
    
	for (GTFNode* node : nodeList)
		displayNode(draw_list, curOffset - scrolling , node, node_selected);
    
	updateDraging(curOffset - scrolling);
	renderLines(draw_list, curOffset - scrolling);
    
    draw_list->ChannelsMerge();
    
    // Open context menu
    if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
    {
        node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
        open_context_menu = true;
    }
    if (open_context_menu)
    {
        ImGui::OpenPopup("context_menu");
        if (node_hovered_in_list != -1)
            node_selected = node_hovered_in_list;
        if (node_hovered_in_scene != -1)
            node_selected = node_hovered_in_scene;
    }
    
    // Draw context menu
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8,8));
    if (ImGui::BeginPopup("context_menu"))
    {
       /* if (ImGui::MenuItem("Load graph..."))
        {
        
             char path[1024];
             if (Dialog_open(path))
             {
             printf("file to load %s\n", path);
             }
        
        }
        
        if (ImGui::MenuItem("Save graph..."))
        {
             char path[1024];
             if (Dialog_save(path))
             {
             saveNodes(path);
             }
             
        }*/
        
        
		/*
         Node* node = node_selected != -1 ? &nodes[node_selected] : NULL;
         ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;
         if (node)
         {
         ImGui::Text("Node '%s'", node->Name);
         ImGui::Separator();
         if (ImGui::MenuItem("Rename..", NULL, false, false)) {}
         if (ImGui::MenuItem("Delete", NULL, false, false)) {}
         if (ImGui::MenuItem("Copy", NULL, false, false)) {}
         }
         */
        //else
        
		/*for (int i = 0; i < (int)sizeof_array(s_nodeTypes); ++i)
		{
			if (ImGui::MenuItem(s_nodeTypes[i].name))
			{
				Node* node = createNodeFromType(ImGui::GetIO().MousePos - curOffset + scrolling, &s_nodeTypes[i]);
				s_nodes.push_back(node);
			}
		}*/
        
        for (auto nodeTypeIt : this->m_graphType->nodeTypeRegistry)
        {
            if (ImGui::MenuItem(nodeTypeIt.second->displayName.c_str()))
			{
                ImVec2 nodePos = ImGui::GetIO().MousePos - curOffset + scrolling;
                createNode(nodeTypeIt.second->unique_id, nodePos.x, nodePos.y);
            }
        }
        
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    
    // Scrolling
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
        scrolling = scrolling - ImGui::GetIO().MouseDelta;
    
    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::EndGroup();
    
    //ImGui::End();
}