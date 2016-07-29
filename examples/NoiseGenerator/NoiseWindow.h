//
//  NoiseWindow.h
//  gtf
//
//  Created by David Gallardo on 26/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include <gtf/Window.h>
#include <gtf/RHI.h>
#include <memory>

#include "GeneratorWorker.h"


class NoiseWindow : public gtf::Window
{
public:
    NoiseWindow(const char* title);
	~NoiseWindow();
	virtual void frame(double deltaTime) override;
	virtual void postFrame(double deltaTime) override;
    void postSetMainInit();
	void nullifyGradientWindow() { m_gradientWindow = nullptr;  }
private:
    GeneratorWorker* m_worker;
    GeneratorInfo m_info;
    std::unique_ptr<gtf::RHITexture2D> m_texture;
    class GradientEditorWindow* m_gradientWindow {nullptr};
    int m_currentRes { 2 };
	bool m_createGradientEditorWindow{ false };
};