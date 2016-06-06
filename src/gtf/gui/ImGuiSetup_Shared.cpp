//
//  ImGuiSetup_Shared.cpp
//  TexComposer
//
//  Created by David Gallardo on 18/05/16.
//
//

#include "ImGuiSetup.h"

#include "imgui.h"

void GTFIMGUI_InitImGui()
{
    ImGuiIO& io = ImGui::GetIO();
    // Time elapsed since last frame, in seconds
    // (in this sample app we'll override this every frame because our time step is variable)
    // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_Tab] = 9;
    io.KeyMap[ImGuiKey_LeftArrow] = ImGuiKey_LeftArrow;
    io.KeyMap[ImGuiKey_RightArrow] = ImGuiKey_RightArrow;
    io.KeyMap[ImGuiKey_UpArrow] = ImGuiKey_UpArrow;
    io.KeyMap[ImGuiKey_DownArrow] = ImGuiKey_DownArrow;
    io.KeyMap[ImGuiKey_Home] = ImGuiKey_Home;
    io.KeyMap[ImGuiKey_End] = ImGuiKey_End;
    io.KeyMap[ImGuiKey_Delete] = ImGuiKey_Delete;
    io.KeyMap[ImGuiKey_Backspace] = 127;
    io.KeyMap[ImGuiKey_Enter] = 13;
    io.KeyMap[ImGuiKey_Escape] = 27;
    io.KeyMap[ImGuiKey_A] = 'a';
    io.KeyMap[ImGuiKey_C] = 'c';
    io.KeyMap[ImGuiKey_V] = 'v';
    io.KeyMap[ImGuiKey_X] = 'x';
    io.KeyMap[ImGuiKey_Y] = 'y';
    io.KeyMap[ImGuiKey_Z] = 'z';
    
    io.DeltaTime = 1.0f/60.0f;
    
    //io.RenderDrawListsFn = ImImpl_RenderDrawLists;
    io.RenderDrawListsFn = ImGuiGL3_RenderDrawLists;
    
    //LoadFontsTexture();
    ImGuiGL3_CreateDeviceObjects();
}