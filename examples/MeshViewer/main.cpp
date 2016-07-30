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
#include <gtf/Window.h>
#include <gtf/RHI.h>
#include <gtf/StaticMesh.h>

class MeshViewerWindow : public gtf::Window
{
public:
	MeshViewerWindow()
		: gtf::Window("MeshViewer", 800, 600)
		, m_attrList(sizeof(gtf::StaticMesh::VertexData))
	{
		/*
		struct VertexData
	{
		float posX{ 0.0f }, posY{ 0.0f }, posZ{ 0.0f };
		float colorR{ 1.0f }, colorG{ 1.0f }, colorB{ 1.0f }, colorA{ 1.0f };
		float normX{ 0.0f }, normY{ 0.0f }, normZ{ 0.0f };
		float tanX{ 0.0f }, tanY{ 0.0f }, tanZ{ 0.0f };
		float biTanX{ 0.0f }, biTanY{ 0.0f }, biTanZ{ 0.0f };
		float tcX{ 0.0f }, tcY{ 0.0f };
	};
		*/
		m_attrList.addAttribute(offsetof(gtf::StaticMesh::VertexData, posX), 3, gtf::ERHIValueType::RHI_FLOAT, false);
		m_attrList.addAttribute(offsetof(gtf::StaticMesh::VertexData, colorR), 4, gtf::ERHIValueType::RHI_FLOAT, false);
		m_attrList.addAttribute(offsetof(gtf::StaticMesh::VertexData, normX), 3, gtf::ERHIValueType::RHI_FLOAT, false);
		m_attrList.addAttribute(offsetof(gtf::StaticMesh::VertexData, tanX), 3, gtf::ERHIValueType::RHI_FLOAT, false);
		m_attrList.addAttribute(offsetof(gtf::StaticMesh::VertexData, biTanX), 3, gtf::ERHIValueType::RHI_FLOAT, false);
		m_attrList.addAttribute(offsetof(gtf::StaticMesh::VertexData, tcX), 2, gtf::ERHIValueType::RHI_FLOAT, false);
	}

	~MeshViewerWindow()
	{

	}

	virtual void fileDrop(int count, const char** paths)
	{
		if (count != 1) return;
		if (m_loader.loadFromFile(paths[0], m_mesh))
		{
			m_vaos.clear();
			int shapeCount = m_mesh.getShapeCount();
			for (int s = 0; s < shapeCount; ++s)
			{
				gtf::StaticMesh::Shape const * shape = m_mesh.getShape(s);
				std::shared_ptr<gtf::RHIVAO> newVAO(gtf::GRHI->createVertexBufferObject());
				newVAO->setup(m_attrList, shape->data, shape->vertexCount);
			}
		}
	}

	virtual void frame(double deltaTime) override
	{

	}

private:
	gtf::StaticMesh m_mesh;
	gtf::StaticMeshLoader m_loader;
	std::vector< std::shared_ptr<gtf::RHIVAO> > m_vaos;
	gtf::RHIVertexAttributeList m_attrList;
};

class MeshViewerApp : public gtf::App
{
public:
	static MeshViewerApp* instance;

	virtual void readyToStart()
	{
		//MeshViewerApp::instance = this;
		viewerWindow = std::shared_ptr<MeshViewerWindow>(new MeshViewerWindow());
		registerWindow(viewerWindow.get());
		setMainWindow(viewerWindow.get());
		viewerWindow->setVisible(true);
	}

	~MeshViewerApp()
	{

	}

	std::shared_ptr<MeshViewerWindow> viewerWindow;
};

int main(int argc, const char * argv[])
{
/*
	gtf::StaticMesh mesh;
	gtf::StaticMeshLoader loader;

	if (loader.loadFromFile("woodbarrel.fbx", mesh))
	{
		std::cout << "FBX load ok!" << std::endl;
	}
	else
	{
		std::cout << "FBX load failed!" << std::endl;
	}

	if (loader.loadFromFile("woodbarrel.obj", mesh))
	{
		std::cout << "OBJ load ok!" << std::endl;
	}
	else
	{
		std::cout << "OBJ load failed!" << std::endl;
	}


	*/

	gtf::App* app = new MeshViewerApp();
	return app->run(argc, argv);

    return 0;
}
