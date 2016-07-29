#pragma once

#include <vector>
#include <memory>

namespace gtf
{

class StaticMeshLoader
{
public:
	enum class ELoadingAction : unsigned char
	{
		IDLE,
		LOADING_FROM_FILE,
		COMPUTING_NORMALS,
		COMPUTING_TANGENTS
	};

	bool loadFromFile(const char * path, class StaticMesh & mesh);
	void getLoadingStatus(ELoadingAction & currentAction, float & pct);
private:
	float m_loadedPct;
	ELoadingAction m_currentAction{ ELoadingAction::IDLE };
};

class StaticMesh
{
public:

	struct VertexData
	{
		float posX{ 0.0f }, posY{ 0.0f }, posZ{ 0.0f };
		float colorR{ 1.0f }, colorG{ 1.0f }, colorB{ 1.0f }, colorA{ 1.0f };
		float normX{ 0.0f }, normY{ 0.0f }, normZ{ 0.0f };
		float tanX{ 0.0f }, tanY{ 0.0f }, tanZ{ 0.0f };
		float biTanX{ 0.0f }, biTanY{ 0.0f }, biTanZ{ 0.0f };
		float tcX{ 0.0f }, tcY{ 0.0f };
	};

	class Shape
	{
	public:
		VertexData* allocateData(size_t dataSize);
		~Shape();

	public:
		unsigned int shapeId;
		VertexData* data{ nullptr };
		size_t vertexCount;
		bool haveNormals;
		bool haveTexCoords;
	};

	StaticMesh();
	~StaticMesh();

	Shape const * getShape(size_t shapeIndex) const;
	size_t getShapeCount() const { return m_shapes.size(); }
	void addShape(Shape* newShape) { m_shapes.push_back(newShape); }

private:
	std::vector<Shape*> m_shapes;
	friend class StaticMeshLoader;
};

} //namespace gtf