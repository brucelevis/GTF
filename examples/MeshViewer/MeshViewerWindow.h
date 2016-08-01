
#include <gtf/Window.h>
#include <gtf/RHI.h>
#include <gtf/StaticMesh.h>

#include <glm/glm.hpp>


#include <list>

#include "BuiltInGfxData.h"

class MeshViewerWindow : public gtf::Window
{
public:
	MeshViewerWindow();
	~MeshViewerWindow();
	virtual void resize(int newWidth, int newHeight);
	virtual void fileDrop(int count, const char** paths) override;
	virtual void frame(double deltaTime) override;
	
private:
	BuiltInGfxData m_gfx;
	gtf::StaticMesh m_mesh;
	gtf::StaticMeshLoader m_meshLoader;
	glm::mat4 m_modelMatrix, m_viewMatrix, m_projectionMatrix;

	//RHI
	gtf::RHIVertexAttributeList m_attrList;
	struct TexturedShape
	{
		std::string name;
		std::shared_ptr<gtf::RHIVAO> vao;
		std::shared_ptr<gtf::RHITexture2D> normalMap;
		std::shared_ptr<gtf::RHITexture2D> colorMap;
	};
	std::list< TexturedShape > m_texturedShapes;

	struct
	{
		glm::vec3 position;
		glm::vec3 viewPosition;
		glm::vec3 rotation;
		float scale;
		float scaleFactor;
		float rotAccelX{ 0.0f };
		float rotAccelY{ 0.0f };
	} m_frame;

};
