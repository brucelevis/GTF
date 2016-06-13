//
//  ImGuiSetup.h
//  TexComposer
//
//  Created by David Gallardo on 18/05/16.
//
//

#include "imgui.h"

struct ImGuiLocalGLProperties
{
	int          _ShaderHandle{ 0 }, _VertHandle{ 0 }, _FragHandle{ 0 };
	int          _AttribLocationTex{ 0 }, _AttribLocationProjMtx{ 0 };
	int          _AttribLocationPosition{ 0 }, _AttribLocationUV{ 0 }, _AttribLocationColor{ 0 };
	unsigned int _VboHandle{ 0 }, _VaoHandle{ 0 }, _ElementsHandle{ 0 };
};

void GTFIMGUI_InitImGui();
void ImGuiGL3_CreateFontsTexture();
bool ImGuiGL3_CreateLocalDeviceObjects(ImGuiLocalGLProperties & prop);
void ImGuiGL3_RenderDrawLists(ImDrawData* draw_data, ImGuiLocalGLProperties & prop);
void ImGuiGL3_InvalidateLocalDeviceObjects(ImGuiLocalGLProperties & prop);
void ImGuiGL3_InvalidateSharedDeviceObjects();