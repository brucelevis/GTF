#include "gtf/StaticMesh.h"
#include "gtf/LangUtils.h"

#include <tiny_obj_loader.h>
#include <fbxsdk.h>
#include <fbxsdk/fileio/fbxiosettings.h>
#include <mikktspace.h>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <iostream>

namespace gtf
{

bool loadFBX(const char * fbxPath, StaticMesh & mesh, float & pct)
{
	std::cout << "loadFBX : " << fbxPath << std::endl;
	
	std::string path(fbxPath);

	// Create the FBX SDK manager
	FbxManager* lSdkManager = FbxManager::Create();

	// Create an IOSettings object.
	FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// ... Configure the FbxIOSettings object ...

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Initialize the importer.
	bool lImportStatus = lImporter->Initialize(path.c_str(), -1, lSdkManager->GetIOSettings());

	if (!lImportStatus) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		return false;
	}

	// Create a new scene so it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "scene");

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	FbxGeometryConverter lGeomConverter(lSdkManager);
	lGeomConverter.Triangulate(lScene, /*replace*/true);

	// Split meshes per material, so that we only have one material per mesh (for VBO support)
	lGeomConverter.SplitMeshesPerMaterial(lScene, /*replace*/true);

	//GET VERSION
	// File format version numbers to be populated.
	int lFileMajor, lFileMinor, lFileRevision;

	// Populate the FBX file format version numbers with the import file.
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);


	// Get the root node of the scene.
	FbxNode* lRootNode = lScene->GetRootNode();

	//iterate child nodes
	int nodeCount = lRootNode->GetChildCount();
	int shapeIndex = 0;
	for (int nodei = 0; nodei < nodeCount; nodei++)
	{
		fbxsdk::FbxNode* lChildNode = lRootNode->GetChild(nodei);
		fbxsdk::FbxMesh* fbxMesh = lChildNode->GetMesh();

		if (fbxMesh)
		{
			std::cout << "mesh found" << std::endl;
			fbxMesh->GetPolygonVertices();

			printf("fbxMesh[%d].name = %s\n", shapeIndex, fbxMesh->GetName());
			printf("fbxMesh[%d].indices: %d\n", shapeIndex, fbxMesh->GetPolygonVertexCount());
			printf("fbxMesh[%d].positions: %d\n", shapeIndex, fbxMesh->GetControlPointsCount());
			printf("fbxMesh[%d].texcoords: %d\n", shapeIndex, fbxMesh->GetTextureUVCount());
			printf("fbxMesh[%d].normals: %d\n", shapeIndex, fbxMesh->GetElementNormalCount());

			if ((fbxMesh->GetPolygonVertexCount() % 3) > 0)
			{
				std::cout << "loadFBX : Incorrect number of positions" << std::endl;
				continue;
			}

			StaticMesh::Shape* newShape = new StaticMesh::Shape;
			mesh.addShape(newShape);

			unsigned int vertexCount = (unsigned int)fbxMesh->GetPolygonVertexCount();
			StaticMesh::VertexData* vertexData = newShape->allocateData(vertexCount);

			int* indexData = fbxMesh->GetPolygonVertices();

			//check for normals and texcoords
			bool haveNormals = (fbxMesh->GetElementNormalCount() > 0);
			bool haveTexCoords = (fbxMesh->GetTextureUVCount() > 0);
			bool haveColors = (fbxMesh->GetElementVertexColorCount() > 0);

			FbxLayerElementArrayTemplate<FbxVector2>* uvVertices = 0;
			fbxMesh->GetTextureUV(&uvVertices, FbxLayerElement::eTextureDiffuse);

			//store vertex data based on index list
			for (unsigned int ic = 0; ic < vertexCount / 3; ic++)
			{
				for (unsigned int vc = 0; vc < 3; vc++)
				{
					unsigned int index = indexData[(3 * ic) + vc];
					FbxVector4 vert = fbxMesh->GetControlPointAt(index);
					vertexData[3 * ic + vc].posX = (float)vert.mData[0];
					vertexData[3 * ic + vc].posY = (float)vert.mData[1];
					vertexData[3 * ic + vc].posZ = (float)vert.mData[2];

					if (haveTexCoords)
					{
						FbxVector2 uv = uvVertices->GetAt(fbxMesh->GetTextureUVIndex(ic, vc));
						vertexData[3 * ic + vc].tcX = (float)uv[0];
						vertexData[3 * ic + vc].tcY = (float)uv[1];
					}

					if (haveNormals)
					{
						FbxVector4 normal;
						fbxMesh->GetPolygonVertexNormal(ic, vc, normal);
						vertexData[3 * ic + vc].normX = (float)normal[0];
						vertexData[3 * ic + vc].normY = (float)normal[1];
						vertexData[3 * ic + vc].normZ = (float)normal[2];
					}

					if (haveColors)
					{
						fbxsdk::FbxGeometryElementVertexColor* elementColor = fbxMesh->GetElementVertexColor(index);
						auto colorsArray = elementColor->GetDirectArray();
						if (colorsArray.GetCount() > 0)
						{
							FbxColor color = colorsArray[0];
							vertexData[3 * ic + vc].colorR = (float)color.mRed;
							vertexData[3 * ic + vc].colorG = (float)color.mGreen;
							vertexData[3 * ic + vc].colorB = (float)color.mBlue;
							vertexData[3 * ic + vc].colorA = (float)color.mAlpha;
						}
					}
				}

				pct = ic / float(vertexCount);
			}

			newShape->name = fbxMesh->GetName();
			if (newShape->name.length() == 0)
			{
				char shapeNum[10];
				sprintf(shapeNum, "shape%d", shapeIndex);
				newShape->name = shapeNum;
			}
			newShape->vertexCount = vertexCount;
			newShape->haveNormals = haveNormals;
			newShape->haveTexCoords = haveTexCoords;
			newShape->shapeId = shapeIndex;

			printf("------- fbxMesh[%d] \"%s\" END ------------------\n", shapeIndex, fbxMesh->GetName());

			shapeIndex++;
		}
	}

	//delete all
	lScene->Destroy();
	lImporter->Destroy();
	lSdkManager->Destroy();

	return shapeIndex > 0;
}

bool loadOBJ(const char * objPath, StaticMesh & mesh, float & pct)
{
	std::cout << "loadOBJ : " << objPath << std::endl;

	std::string path(objPath);
	std::string basePath = path.substr(0, path.find_last_of("\\/") + 1).c_str();
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err = tinyobj::LoadObj(shapes, materials, path.c_str(), basePath.c_str());

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (shapes.size() == 0)
	{
		std::cout << "MPOBJLoader::loadFromFile : No shapes found" << std::endl;
		return false;
	}

	printf("shapes.size() = %ld\n", shapes.size());

	int shapeIndex = 0;
	for (size_t shape = 0; shape < shapes.size(); ++shape)
	{
		const tinyobj::shape_t & objShape = shapes[shapeIndex];

		printf("objShape[%d].name = %s\n", shapeIndex, objShape.name.c_str());
		printf("objShape[%d].indices: %ld\n", shapeIndex, objShape.mesh.indices.size());
		printf("objShape[%d].positions: %ld\n", shapeIndex, objShape.mesh.positions.size());
		printf("objShape[%d].texcoords: %ld\n", shapeIndex, objShape.mesh.texcoords.size());
		printf("objShape[%d].normals: %ld\n", shapeIndex, objShape.mesh.normals.size());

		//printf("Size of shape[%ld].material_ids: %ld\n", i, objShape.mesh.material_ids.size());
		//assert((objShape.mesh.indices.size() % 3) == 0);

		if ((objShape.mesh.positions.size() % 3) > 0)
		{
			std::cout << "MPOBJLoader::loadFromFile : Incorrect number of positions" << std::endl;
			continue;
		}

		StaticMesh::Shape* newShape = new StaticMesh::Shape;
		mesh.addShape(newShape);

		unsigned int vertexCount = (unsigned int)objShape.mesh.indices.size();
		StaticMesh::VertexData* vertexData = newShape->allocateData(vertexCount);
		size_t indexCount = objShape.mesh.indices.size();
		unsigned int* indexData = new unsigned int[indexCount]; //deleted each loop

																//collect index data
		for (size_t f = 0; f < indexCount / 3ul; f++)
		{
			indexData[3 * f + 0] = objShape.mesh.indices[3 * f + 0];
			indexData[3 * f + 1] = objShape.mesh.indices[3 * f + 1];
			indexData[3 * f + 2] = objShape.mesh.indices[3 * f + 2];
		}

		//check for normals and texcoords
		bool haveNormals = (objShape.mesh.normals.size()> 0);
		bool haveTexCoords = (objShape.mesh.texcoords.size() > 0);

		//store vertex data based on index list
		for (unsigned int ic = 0; ic < indexCount; ic += 3)
		{
			for (unsigned int vc = 0; vc < 3; vc++)
			{
				unsigned int index = indexData[ic + vc];

				//this boundary check prevents a crash with buggy OBJ files
				if ((3 * index + 2) < objShape.mesh.positions.size()) 
				{
					vertexData[ic + vc].posX = objShape.mesh.positions[3 * index + 0];
					vertexData[ic + vc].posY = objShape.mesh.positions[3 * index + 1];
					vertexData[ic + vc].posZ = objShape.mesh.positions[3 * index + 2];
				}

				if (haveTexCoords)
				{
					if((2 * index + 1) < objShape.mesh.texcoords.size())
					{
						vertexData[ic + vc].tcX = objShape.mesh.texcoords[2 * index + 0];
						vertexData[ic + vc].tcY = objShape.mesh.texcoords[2 * index + 1];
					}
				}

				if (haveNormals)
				{
					if((3 * index + 2) < objShape.mesh.normals.size())
					{
						vertexData[ic + vc].normX = objShape.mesh.normals[3 * index + 0];
						vertexData[ic + vc].normY = objShape.mesh.normals[3 * index + 1];
						vertexData[ic + vc].normZ = objShape.mesh.normals[3 * index + 2];
					}	
				}
			}

			pct = ic / float(vertexCount);
		}

		delete [] indexData;
		newShape->name = objShape.name;
		if (newShape->name.length() == 0)
		{
			char shapeNum[10];
			sprintf(shapeNum, "shape%d", shapeIndex);
			newShape->name = shapeNum;
		}
		newShape->vertexCount = vertexCount;
		newShape->haveNormals = haveNormals;
		newShape->haveTexCoords = haveTexCoords;
		newShape->shapeId = shapeIndex;

		printf("------- objShape[%d] \"%s\" END ------------------\n", shapeIndex, objShape.name.c_str());

		++shapeIndex;
	}


	return shapeIndex > 0;
}

void computeNormals(StaticMesh::Shape* shape, float & pct)
{
	//manually calculate normals
	for (size_t ic = 0; ic < shape->vertexCount / 3ul; ic++)
	{
		glm::vec3 points[3];
		for (int vc = 0; vc < 3; vc++)
		{
			points[vc].x = shape->data[3 * ic + vc].posX;
			points[vc].y = shape->data[3 * ic + vc].posY;
			points[vc].z = shape->data[3 * ic + vc].posZ;
		}

		glm::vec3 vecA(points[1] - points[0]);
		glm::vec3 vecB(points[2] - points[0]);

		glm::vec3 normal = glm::cross(vecA, vecB);

		for (int vc = 0; vc < 3; vc++)
		{
			glm::vec3 tempNormal = glm::vec3(shape->data[3 * ic + vc].normX,
				shape->data[3 * ic + vc].normY,
				shape->data[3 * ic + vc].normZ);
			tempNormal = tempNormal + normal;
			shape->data[3 * ic + vc].normX = tempNormal.x;
			shape->data[3 * ic + vc].normY = tempNormal.y;
			shape->data[3 * ic + vc].normZ = tempNormal.z;
		}

		pct = float(ic) / float(shape->vertexCount);
	}

	for (size_t ic = 0; ic < shape->vertexCount / 3ul; ic++)
	{
		for (int vc = 0; vc < 3; vc++)
		{
			glm::vec3 tempNormal = glm::vec3(shape->data[3 * ic + vc].normX,
				shape->data[3 * ic + vc].normY,
				shape->data[3 * ic + vc].normZ);
			tempNormal = glm::normalize(tempNormal);
			shape->data[3 * ic + vc].normX = tempNormal.x;
			shape->data[3 * ic + vc].normY = tempNormal.y;
			shape->data[3 * ic + vc].normZ = tempNormal.z;
		}

		pct = float(ic) / float(shape->vertexCount);
	}
}

void computeTangentSpace(StaticMesh::Shape* shape, float & pct)
{
	SMikkTSpaceInterface* interface = new SMikkTSpaceInterface();

	SMikkTSpaceContext* context = new SMikkTSpaceContext();

	struct TempUserData
	{
		float * loadedRatio;
		int vertexCount;
		StaticMesh::VertexData* vertexData;
	};

	TempUserData* data = new TempUserData();
	data->loadedRatio = &pct;
	data->vertexCount = shape->vertexCount;
	data->vertexData = shape->data;

	context->m_pInterface = interface;
	context->m_pUserData = data;

	interface->m_getNumFaces = [](const SMikkTSpaceContext * pContext)
	{
		TempUserData* data = (TempUserData*)pContext->m_pUserData;
		return data->vertexCount / 3;
	};

	interface->m_getNumVerticesOfFace = [](const SMikkTSpaceContext * pContext, const int iFace)
	{
		return 3;
	};

	interface->m_getPosition = [](const SMikkTSpaceContext * pContext, float fvPosOut[], const int iFace, const int iVert)
	{
		TempUserData* data = (TempUserData*)pContext->m_pUserData;
		StaticMesh::VertexData* vertexData = data->vertexData;
		StaticMesh::VertexData & vert = vertexData[iFace * 3 + iVert];
		fvPosOut[0] = vert.posX;
		fvPosOut[1] = vert.posY;
		fvPosOut[2] = vert.posZ;
	};

	interface->m_getNormal = [](const SMikkTSpaceContext * pContext, float fvNormOut[], const int iFace, const int iVert)
	{
		TempUserData* data = (TempUserData*)pContext->m_pUserData;
		StaticMesh::VertexData* vertexData = data->vertexData;
		StaticMesh::VertexData & vert = vertexData[iFace * 3 + iVert];
		fvNormOut[0] = vert.normX;
		fvNormOut[1] = vert.normY;
		fvNormOut[2] = vert.normZ;
	};

	interface->m_getTexCoord = [](const SMikkTSpaceContext * pContext, float fvTexcOut[], const int iFace, const int iVert)
	{
		TempUserData* data = (TempUserData*)pContext->m_pUserData;
		StaticMesh::VertexData* vertexData = data->vertexData;
		StaticMesh::VertexData & vert = vertexData[iFace * 3 + iVert];
		fvTexcOut[0] = vert.tcX;
		fvTexcOut[1] = vert.tcY;
	};

	interface->m_setTSpaceBasic = [](const SMikkTSpaceContext * pContext, const float fvTangent[], const float fSign, const int iFace, const int iVert)
	{
		TempUserData* data = (TempUserData*)pContext->m_pUserData;
		StaticMesh::VertexData* vertexData = data->vertexData;
		StaticMesh::VertexData & vert = vertexData[iFace * 3 + iVert];
		vert.tanX = fvTangent[0];
		vert.tanY = fvTangent[1];
		vert.tanZ = fvTangent[2];
		glm::vec3 bitangent = -fSign * glm::cross(glm::vec3(vert.normX, vert.normY, vert.normZ), glm::vec3(vert.tanX, vert.tanY, vert.tanZ));
		vert.biTanX = bitangent.x;
		vert.biTanY = bitangent.y;
		vert.biTanZ = bitangent.z;

		(*data->loadedRatio) = float(iFace * 3 + iVert) / float(data->vertexCount);
	};

	genTangSpaceDefault(context);

	delete data;
	delete context;
	delete interface;
}

bool StaticMeshLoader::loadFromFile(const char * path, StaticMesh & mesh)
{
	mesh.clear();

	char ext[6] = { 0 };
	getFileExtension(path, ext, sizeof(ext));

	m_currentAction = ELoadingAction::LOADING_FROM_FILE;
	m_loadedPct = 0.0f;

	if (strcmp(ext, ".fbx") == 0)
	{
		if (!loadFBX(path, mesh, m_loadedPct)) return false;
	}
	else if (strcmp(ext, ".obj") == 0)
	{
		if (!loadOBJ(path, mesh, m_loadedPct)) return false;
	}
	else
	{
		return false;
	}

	for (StaticMesh::Shape* shape : mesh.m_shapes)
	{
		//compute normals
		m_currentAction = ELoadingAction::COMPUTING_NORMALS;
		m_loadedPct = 0.0f;

		if (!shape->haveNormals)
			computeNormals(shape, m_loadedPct);

		//computing tangents
		m_currentAction = ELoadingAction::COMPUTING_TANGENTS;
		m_loadedPct = 0.0f;

		computeTangentSpace(shape, m_loadedPct);
	}

	return true;
}

void StaticMeshLoader::getLoadingStatus(ELoadingAction & currentAction, float & pct)
{
	currentAction = m_currentAction;
	pct = m_loadedPct;
}

StaticMesh::VertexData* StaticMesh::Shape::allocateData(size_t dataSize)
{
	delete[] data;
	data = new VertexData[dataSize];
	return data;
}

StaticMesh::Shape::~Shape() noexcept
{
	delete[] data;
}

StaticMesh::StaticMesh()
{

}

StaticMesh::~StaticMesh()
{
	clear();
}

void StaticMesh::clear()
{
	for (Shape* shape : m_shapes)
		delete shape;

	m_shapes.clear();
}

StaticMesh::Shape const * StaticMesh::getShape(size_t shapeIndex) const
{
	return (shapeIndex < m_shapes.size()) ? m_shapes[shapeIndex] : nullptr;
}



} //namespace gtf