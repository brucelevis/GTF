//
//  ImGuiSetup.h
//  TexComposer
//
//  Created by David Gallardo on 18/05/16.
//
//

#include "imgui.h"

void GTFIMGUI_InitImGui();
void ImGuiGL3_CreateFontsTexture();
bool ImGuiGL3_CreateDeviceObjects();
void ImGuiGL3_RenderDrawLists(ImDrawData* draw_data);
void ImGuiGL3_InvalidateDeviceObjects();