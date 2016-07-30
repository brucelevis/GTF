
#include "MeshViewerWindow.h"
#include <imgui.h>

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

	m_projectionMatrix = glm::perspective(45.0f, (float)m_windowWidth / glm::max(1.0f, (float)m_windowHeight), 1.0f, 1000.0f);
	m_viewMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, -150));
	m_modelMatrix = glm::mat4();

	gtf::GRHI->setClearColor(0.3f, 0.3f, 0.28f, 1.0f);
}

MeshViewerWindow::~MeshViewerWindow()
{

}

void MeshViewerWindow::fileDrop(int count, const char** paths)
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
			m_vaos.push_back(newVAO);
		}
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
	float infoWidth = 250.0f;
	ImGui::SetNextWindowPos(ImVec2(m_windowWidth - infoWidth - 20.0f, m_windowHeight - 200.0f), ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(ImVec2(infoWidth, 180.0f), ImGuiSetCond_Always);
	if (ImGui::Begin("Info", nullptr, flags))
	{
		ImGui::Text("Info");
		ImGui::Separator(); ImGui::Spacing();
		ImGui::Text("Drag and Drop a FBX or OBJ mesh\nfile into this window");
	}
	ImGui::End();

	//TODO: MESH TRANSFORM WITH MOUSE

	//render
	gtf::GRHI->viewport(0, 0, m_windowWidth, m_windowHeight);
	gtf::GRHI->clearColorAndDepthBuffers();

	//gtf::GRHI->setBlend(false);
	gtf::GRHI->setDepthTest(true);

	m_gfx.renderMeshProgram->active();
	m_gfx.renderMeshProgram->setUniform4x4m("uProjectionMatrix", glm::value_ptr(m_projectionMatrix));
	m_gfx.renderMeshProgram->setUniform4x4m("uModelMatrix", glm::value_ptr(m_modelMatrix));
	m_gfx.renderMeshProgram->setUniform4x4m("uViewMatrix", glm::value_ptr(m_viewMatrix));

	for (auto vao : m_vaos)
	{
		vao->render();
	}

	gtf::GRHI->setBackfaceCulling(false);
	//gtf::GRHI->setBlend(true);
}