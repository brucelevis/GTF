
#include <gtf/Window.h>
#include <gtf/RHI.h>
#include <gtf/StaticMesh.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	std::vector< std::shared_ptr<gtf::RHIVAO> > m_vaos;
	std::shared_ptr<gtf::RHITexture2D> m_normaMap{ nullptr };
	std::shared_ptr<gtf::RHITexture2D> m_colorMap{ nullptr };

	struct
	{
		glm::vec3 position;
		glm::vec3 viewPosition;
		glm::vec3 rotation;
		float scale;
		float scaleFactor;
	} m_frame;
};
