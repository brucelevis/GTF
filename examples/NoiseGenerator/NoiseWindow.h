//
//  NoiseWindow.h
//  gtf
//
//  Created by David Gallardo on 26/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include "GTFWindow.h"
#include "GeneratorWorker.h"
#include "GTFRHI.h"

class NoiseWindow : public GTFWindow
{
public:
    NoiseWindow(const char* title);
    virtual void frame(double deltaTime) override;
    void postSetMainInit();
private:
    GeneratorWorker* m_worker;
    GeneratorInfo m_info;
    GTFRHITexture2DPtr m_texture;
    class GradientEditorWindow* m_gradientWindow {nullptr};
    int m_currentRes { 2 };
};