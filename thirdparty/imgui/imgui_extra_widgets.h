//
//  imgui_extra_widgets.h
//  MeshPainter
//
//  Created by David Gallardo on 20/02/16.
//  Copyright (c) 2016 David Gallardo. All rights reserved.
//

#pragma once

#include "imgui.h"

namespace ImGui
{
    bool ColorPicker4(float* col, bool show_alpha);
    bool ColorPicker3(float* col);

};