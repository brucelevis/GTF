//
//  main.cpp
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include <vector>
#include <iostream>

#include <gtf/App.h>

#include "MeshViewerWindow.h"

class MeshViewerApp : public gtf::App
{
public:
	static MeshViewerApp* instance;

	virtual void readyToStart()
	{
		MeshViewerApp::instance = this;
		viewerWindow = std::shared_ptr<MeshViewerWindow>(new MeshViewerWindow());
		registerWindow(viewerWindow.get());
		setMainWindow(viewerWindow.get());
		viewerWindow->setVisible(true);
	}

	std::shared_ptr<MeshViewerWindow> viewerWindow;
};

MeshViewerApp* MeshViewerApp::instance;

int main(int argc, const char * argv[])
{
	gtf::App* app = new MeshViewerApp();
	return app->run(argc, argv);

    return 0;
}
