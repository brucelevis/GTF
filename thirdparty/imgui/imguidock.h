//
//  imguidock.h
//  MeshPainter
//
//  Created by David Gallardo on 19/02/16.
//  Copyright (c) 2016 David Gallardo. All rights reserved.
//

#pragma once

#include "imgui.h"

#include <string>

//namespace Lumix { namespace FS { class OsFile; } }


struct lua_State;


namespace ImGui
{
    
    
    IMGUI_API void ShutdownDock();
    IMGUI_API void RootDock(const ImVec2& pos, const ImVec2& size);
    IMGUI_API bool BeginDock(const char* label, bool* opened = nullptr, ImGuiWindowFlags extra_flags = 0);
    IMGUI_API void EndDock();
    IMGUI_API void SetDockActive();
    //IMGUI_API void SaveDock(Lumix::FS::OsFile& file);
    //IMGUI_API void LoadDock(lua_State* L);
    IMGUI_API void SaveDock(std::string const & path);
    IMGUI_API void LoadDock(std::string const & path);
    IMGUI_API bool IsAnyFloatingDockDragged();
    IMGUI_API void ResetLayout();
    
} // namespace ImGui
