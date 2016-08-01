
#include "MeshViewerWindow.h"
#include <imgui.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <tinyfiledialogs.h>

#include <gtf/Texture2D.h>

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
	m_projectionMatrix = glm::perspective(220.0f, (float)m_windowWidth / glm::max(1.0f, (float)m_windowHeight), 1.0f, 1000.0f);
}

void MeshViewerWindow::fileDrop(int pathCount, const char** paths)
{
	bool meshLoadedThisDrop = false;

	for (int pathIndex = 0; pathIndex < pathCount; ++pathIndex)
	{
		if (gtf::StaticMeshLoader::isMeshFile(paths[pathIndex]))
		{
			if (!meshLoadedThisDrop && m_meshLoader.loadFromFile(paths[pathIndex], m_mesh))
			{
				m_texturedShapes.clear();
	
				//reset camera
				m_frame.scale = 1.0f;
				m_frame.position = glm::vec3(0.0f);
				m_frame.viewPosition = glm::vec3(0.0f, 0.0f, -150.0f);
				m_frame.rotation = glm::vec3(0.0f);
				m_frame.scaleFactor = 1.0f;

				m_projectionMatrix = glm::perspective(220.0f, (float)m_windowWidth / glm::max(1.0f, (float)m_windowHeight), 1.0f, 1000.0f);
				m_modelMatrix = glm::mat4(1.0);
				m_viewMatrix = glm::translate(glm::mat4(1.0), m_frame.viewPosition);

				glm::vec2 topLeft(0, 0), bottomRight(0, 0);
				float midZ = 0.0f;
				glm::vec4 viewport(0, 0, m_windowWidth, m_windowHeight);
				glm::mat4 vpMatrix = m_projectionMatrix * m_viewMatrix;
				glm::mat4 ivpMatrix = glm::inverse(vpMatrix);

				
				int shapeCount = m_mesh.getShapeCount();


				for (int s = 0; s < shapeCount; ++s)
				{
					gtf::StaticMesh::Shape const * shape = m_mesh.getShape(s);
					std::shared_ptr<gtf::RHIVAO> newVAO(gtf::GRHI->createVertexBufferObject());
					newVAO->setup(m_attrList, shape->data, shape->vertexCount);
					
					//m_vaos.push_back(newVAO);
					TexturedShape texturedShape;
					texturedShape.name = shape->name;
					texturedShape.vao = newVAO;
					m_texturedShapes.push_back(texturedShape);
					
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
						fixScale = 0.75f / glm::abs(topLeft.x);
					}
					else
					{
						fixScale = 0.75f / glm::abs(topLeft.y);
					}
				}
				else
				{
					if (glm::abs(bottomRight.x) > glm::abs(bottomRight.y))
					{
						fixScale = 0.75f / glm::abs(bottomRight.x);
					}
					else
					{
						fixScale = 0.75f / glm::abs(bottomRight.y);
					}
				}

				std::cout << "fixScale = " << fixScale << std::endl;
				m_frame.scale = fixScale;
				m_frame.scaleFactor = fixScale;
				m_frame.position = glm::vec3(fixTranslate);
			}
		}
	}
}

#include <gtf/LangUtils.h>

void MeshViewerWindow::frame(double deltaTime) 
{
	static bool withNormalMap = true;
	static bool withColorMap = true;

	//GUI
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_NoMove;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoTitleBar;
	flags |= ImGuiWindowFlags_HorizontalScrollbar;
	float infoWidth = 280.0f;
	ImGui::SetNextWindowPos(ImVec2(m_windowWidth - infoWidth - 20.0f, 20.0f), ImGuiSetCond_Always);
	ImGui::SetNextWindowSize(ImVec2(infoWidth, m_windowHeight - 240.0f), ImGuiSetCond_Always);
	if (ImGui::Begin("Options", nullptr, flags))
	{
		ImGui::Text("Options");
		ImGui::Separator(); ImGui::Spacing();
		
		const char * textureFilterPatterns[3] = { "*.png", "*.tga", "*.jpg" };

		//TODO: Load Textures
		const char * normalMapBtnFormat = "Set NormalMap %s";
		const char * colorMapBtnFormat = "Set ColorMap %s";

		ImGui::Checkbox("Toggle Normal Map", &withNormalMap);
		ImGui::Checkbox("Toggle Color Map", &withColorMap);
		ImGui::Spacing();

		for (auto & texturedShape : m_texturedShapes)
		{
			char normalMapBtnText[1024];
			sprintf(normalMapBtnText, normalMapBtnFormat, texturedShape.name.c_str());
			if (ImGui::Button(normalMapBtnText))
			{
				const char * texFilePath = tinyfd_openFileDialog("Set Normal Map", "./", 3, textureFilterPatterns, NULL, 0);
				gtf::Texture2D normalMapResource;
				gtf::Texture2DLoader tex2DLoader;
				if (texFilePath && tex2DLoader.loadFromFile(texFilePath, normalMapResource))
				{
					gtf::RHITextureInfo info;
					gtf::fillRHITextureInfo(normalMapResource, info);
					texturedShape.normalMap.reset(gtf::GRHI->createTexture());
					texturedShape.normalMap->setup(info, normalMapResource.getData());
				}
			}

			char colorMapBtnText[1024];
			sprintf(colorMapBtnText, colorMapBtnFormat, texturedShape.name.c_str());
			if (ImGui::Button(colorMapBtnText))
			{
				const char * texFilePath = tinyfd_openFileDialog("Set Color Map", "./", 3, textureFilterPatterns, NULL, 0);
				gtf::Texture2D colorMapResource;
				gtf::Texture2DLoader tex2DLoader;
				if (texFilePath && tex2DLoader.loadFromFile(texFilePath, colorMapResource))
				{
					gtf::RHITextureInfo info;
					gtf::fillRHITextureInfo(colorMapResource, info);
					texturedShape.colorMap.reset(gtf::GRHI->createTexture());
					texturedShape.colorMap->setup(info, colorMapResource.getData());
				}
			}

			ImGui::Spacing();
		}
	}
	ImGui::End();

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
	if (!ImGui::GetIO().WantCaptureMouse)
	{
		if (ImGui::IsMouseDown(0))
		{
			m_frame.rotAccelX += ImGui::GetIO().MouseDelta.y * float(deltaTime) * 3.0f;
			m_frame.rotAccelY += ImGui::GetIO().MouseDelta.x * float(deltaTime) * 3.0f;
			
		}
		else if (ImGui::IsMouseDown(1))
		{
			m_frame.viewPosition.x += ImGui::GetIO().MouseDelta.x * 0.025f;
			m_frame.viewPosition.y -= ImGui::GetIO().MouseDelta.y * 0.025f;
		}

		m_frame.rotation.y += m_frame.rotAccelY * float(deltaTime);
		m_frame.rotation.x += m_frame.rotAccelX * float(deltaTime);
		m_frame.rotAccelX -= m_frame.rotAccelX * float(deltaTime) * 3.0f;
		m_frame.rotAccelY -= m_frame.rotAccelY * float(deltaTime) * 3.0f;
        
        m_frame.scale = glm::max(0.01f, m_frame.scale + (ImGui::GetIO().MouseWheel * float(deltaTime) * m_frame.scaleFactor) * 3.0f);
	}
	
	
		
	//update transform
	
	m_modelMatrix = glm::scale(glm::mat4(), glm::vec3(m_frame.scale));
	m_modelMatrix = glm::translate(m_modelMatrix, m_frame.position);
	m_viewMatrix = glm::translate(glm::mat4(), glm::vec3(m_frame.viewPosition.x, m_frame.viewPosition.y, 0.0f));
	m_viewMatrix = glm::translate(m_viewMatrix, glm::vec3(0.0f, 0.0f, m_frame.viewPosition.z));
	m_viewMatrix = glm::rotate(m_viewMatrix, m_frame.rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	m_viewMatrix = glm::rotate(m_viewMatrix, m_frame.rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));

	//active shader program
	m_gfx.renderMeshProgram->active();
	m_gfx.renderMeshProgram->setUniform4x4m("uProjectionMatrix", glm::value_ptr(m_projectionMatrix));
	m_gfx.renderMeshProgram->setUniform4x4m("uModelMatrix", glm::value_ptr(m_modelMatrix));
	m_gfx.renderMeshProgram->setUniform4x4m("uViewMatrix", glm::value_ptr(m_viewMatrix));



	//render
	gtf::GRHI->viewport(0, 0, m_windowWidth, m_windowHeight);
	gtf::GRHI->clearColorAndDepthBuffers();
	gtf::GRHI->setDepthTest(true);

	for (auto & texturedShape : m_texturedShapes)
	{
		if (texturedShape.normalMap && withNormalMap)
		{
			texturedShape.normalMap->bindAt(0);
			m_gfx.renderMeshProgram->setUniform1i("uNormalMap", 0);
			m_gfx.renderMeshProgram->setUniform1ui("uWithNormalMap", 1);
		}
		else
		{
			m_gfx.renderMeshProgram->setUniform1ui("uWithNormalMap", 0);
		}

		if (texturedShape.colorMap && withColorMap)
		{
			texturedShape.colorMap->bindAt(1);
			m_gfx.renderMeshProgram->setUniform1i("uColorMap", 1);
			m_gfx.renderMeshProgram->setUniform1ui("uWithColorMap", 1);
		}
		else
		{
			m_gfx.renderMeshProgram->setUniform1ui("uWithColorMap", 0);
		}

		texturedShape.vao->render();
	}
}