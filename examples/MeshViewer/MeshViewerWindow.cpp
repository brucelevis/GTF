
#include "MeshViewerWindow.h"
#include <imgui.h>
#include <iostream>
#include <glm/gtx/transform.hpp>

MeshViewerWindow::MeshViewerWindow()
	: gtf::Window("MeshViewer", 800, 600)
	, m_attrList(sizeof(gtf::StaticMesh::VertexData))
{
	//attr list for gtf::StaticMesh::VertexData
	m_attrList.addAttribute(offsetof(gtf::StaticMesh::VertexData, posX), 3, gtf::ERHIValueType::RHI_FLOAT, false);
	m_attrList.addAttribute(offsetof(gtf::StaticMesh::VertexData, colorR), 4, gtf::ERHIValueType::RHI_FLOAT, false);
	m_attrList.addAttribute(offsetof(gtf::StaticMesh::VertexData, normX), 3, gtf::ERHIValueType::RHI_FLOAT, false);
	m_attrList.addAttribute(offsetof(gtf::StaticMesh::VertexData, tanX), 3, gtf::ERHIValueType::RHI_FLOAT, false);
	m_attrList.addAttribute(offsetof(gtf::StaticMesh::VertexData, biTanX), 3, gtf::ERHIValueType::RHI_FLOAT, false);
	m_attrList.addAttribute(offsetof(gtf::StaticMesh::VertexData, tcX), 2, gtf::ERHIValueType::RHI_FLOAT, false);

	m_gfx.init();

	

	gtf::GRHI->setClearColor(0.3f, 0.3f, 0.28f, 1.0f);
}

MeshViewerWindow::~MeshViewerWindow()
{

}

void MeshViewerWindow::resize(int newWidth, int newHeight)
{
	gtf::Window::resize(newWidth, newHeight);
	m_projectionMatrix = glm::perspective(45.0f, (float)m_windowWidth / glm::max(1.0f, (float)m_windowHeight), 1.0f, 1000.0f);
}

void MeshViewerWindow::fileDrop(int count, const char** paths)
{
	if (count != 1) return;

	if (m_meshLoader.loadFromFile(paths[0], m_mesh))
	{
		//reset camera
		m_frame.scale = 1.0f;
		m_frame.position = glm::vec3(0.0f);
		m_frame.viewPosition = glm::vec3(0.0f, 0.0f, -150.0f);
		m_frame.rotation = glm::vec3(0.0f);
		m_frame.scaleFactor = 1.0f;

		m_projectionMatrix = glm::perspective(45.0f, (float)m_windowWidth / glm::max(1.0f, (float)m_windowHeight), 1.0f, 1000.0f);
		m_modelMatrix = glm::mat4(1.0);
		m_viewMatrix = glm::translate(glm::mat4(1.0), m_frame.viewPosition);

		glm::vec2 topLeft(0, 0), bottomRight(0, 0);
		float midZ = 0.0f;
		glm::vec4 viewport(0, 0, m_windowWidth, m_windowHeight);
		glm::mat4 vpMatrix = m_projectionMatrix * m_viewMatrix;
		glm::mat4 ivpMatrix = glm::inverse(vpMatrix);

		m_vaos.clear();
		int shapeCount = m_mesh.getShapeCount();
		for (int s = 0; s < shapeCount; ++s)
		{
			gtf::StaticMesh::Shape const * shape = m_mesh.getShape(s);
			std::shared_ptr<gtf::RHIVAO> newVAO(gtf::GRHI->createVertexBufferObject());
			newVAO->setup(m_attrList, shape->data, shape->vertexCount);
			m_vaos.push_back(newVAO);


			//trying to calculate a default transformation to fid the viewport
			for (size_t vc = 0; vc < shape->vertexCount; vc++)
			{
				glm::vec4 projected(shape->data[vc].posX, shape->data[vc].posY, shape->data[vc].posZ, 1.0f);
				projected = vpMatrix * projected;
				projected /= projected.w;

				if (vc == 0)
				{
					topLeft.x = projected.x;
					topLeft.y = projected.y;
					bottomRight.x = projected.x;
					bottomRight.y = projected.y;
				}
				else
				{
					if (projected.x < topLeft.x)
						topLeft.x = projected.x;
					if (projected.x > bottomRight.x)
						bottomRight.x = projected.x;
					if (projected.y > topLeft.y)
						topLeft.y = projected.y;
					if (projected.y < bottomRight.y)
						bottomRight.y = projected.y;
				}

				midZ += projected.z;
			}

			midZ /= float(shape->vertexCount);
		}

		std::cout << "topLeft = (" << topLeft.x << ", " << topLeft.y << ")" << std::endl;
		std::cout << "bottomRight = (" << bottomRight.x << ", " << bottomRight.y << ")" << std::endl;

		float fixX = (bottomRight.x + topLeft.x) * 0.5f;
		float fixY = (bottomRight.y + topLeft.y) * 0.5f;

		glm::vec4 fixTranslate(fixX, -fixY, midZ, 1.0f);
		std::cout << "fixTranslate preProj = (" << fixTranslate.x << ", " << fixTranslate.y << ")" << std::endl;
		fixTranslate = ivpMatrix * fixTranslate;
		fixTranslate /= fixTranslate.w;
		fixTranslate.z = 0;
		std::cout << "fixTranslate = (" << fixTranslate.x << ", " << fixTranslate.y << ")" << std::endl;
		
		topLeft += glm::vec2(fixX, -fixY);
		bottomRight += glm::vec2(fixX, -fixY);

		float fixScale = 1.0f;
		if (glm::abs(topLeft.x) > glm::abs(bottomRight.x))
		{
			if (glm::abs(topLeft.x) > glm::abs(topLeft.y))
			{
				fixScale = 0.75 / glm::abs(topLeft.x);
			}
			else
			{
				fixScale = 0.75 / glm::abs(topLeft.y);
			}
		}
		else
		{
			if (glm::abs(bottomRight.x) > glm::abs(bottomRight.y))
			{
				fixScale = 0.75 / glm::abs(bottomRight.x);
			}
			else
			{
				fixScale = 0.75 / glm::abs(bottomRight.y);
			}
		}

		std::cout << "fixScale = " << fixScale << std::endl;
		m_frame.scale = fixScale;
		m_frame.scaleFactor = fixScale;
		m_frame.position = glm::vec3(fixTranslate);		
	}

	
}

void MeshViewerWindow::frame(double deltaTime) 
{
	//GUI
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_HorizontalScrollbar;
	float infoWidth = 280.0f;
	ImGui::SetNextWindowPos(ImVec2(m_windowWidth - infoWidth - 20.0f, m_windowHeight - 200.0f), ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(ImVec2(infoWidth, 180.0f), ImGuiSetCond_Always);
	if (ImGui::Begin("Info", nullptr, flags))
	{
		ImGui::Text("Info");
		ImGui::Separator(); ImGui::Spacing();
		ImGui::Text("Drag and Drop a FBX or OBJ mesh\nfile into this window."); ImGui::Spacing();
		ImGui::Text("Left click and drag to rotate."); ImGui::Spacing();
		ImGui::Text("Right click and drag to translate."); ImGui::Spacing();
		ImGui::Text("Use mouse wheel to scale."); ImGui::Spacing();
	}
	ImGui::End();

	//MESH TRANSFORM WITH MOUSE
	if (ImGui::IsMouseDown(0))
	{
		m_frame.rotation.y += ImGui::GetIO().MouseDelta.x * deltaTime;
		m_frame.rotation.x += ImGui::GetIO().MouseDelta.y * deltaTime;
	}
	else if (ImGui::IsMouseDown(1))
	{
		m_frame.viewPosition.x += ImGui::GetIO().MouseDelta.x * 0.2f;
		m_frame.viewPosition.y -= ImGui::GetIO().MouseDelta.y * 0.2f;
	}
	
	m_frame.scale = glm::max(0.01f, m_frame.scale + (ImGui::GetIO().MouseWheel * float(deltaTime) * m_frame.scaleFactor) * 3.0f);
		
	//update transform
	m_modelMatrix = glm::rotate(glm::mat4(), m_frame.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	m_modelMatrix = glm::rotate(m_modelMatrix, m_frame.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_frame.scale));
	m_modelMatrix = glm::translate(m_modelMatrix, m_frame.position);
	m_viewMatrix = glm::translate(glm::mat4(), glm::vec3(m_frame.viewPosition));

	//active shader program
	m_gfx.renderMeshProgram->active();
	m_gfx.renderMeshProgram->setUniform4x4m("uProjectionMatrix", glm::value_ptr(m_projectionMatrix));
	m_gfx.renderMeshProgram->setUniform4x4m("uModelMatrix", glm::value_ptr(m_modelMatrix));
	m_gfx.renderMeshProgram->setUniform4x4m("uViewMatrix", glm::value_ptr(m_viewMatrix));

	//render
	gtf::GRHI->viewport(0, 0, m_windowWidth, m_windowHeight);
	gtf::GRHI->clearColorAndDepthBuffers();
	gtf::GRHI->setDepthTest(true);

	for (auto vao : m_vaos)
	{
		vao->render();
	}
}