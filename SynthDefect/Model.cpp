
// Model.cpp for loaded 3D model data

#include "pch.h"
#include "Model.h"
#include <string>
#include <assimp/Exporter.hpp>

CModel::CModel() 
{
	m_max = glm::vec3(0.0f, 0.0f, 0.0f);
	m_min = glm::vec3(0.0f, 0.0f, 0.0f);
}

CModel::CModel(LPCTSTR const& filePath)
{ 
	m_max = glm::vec3(0.0f, 0.0f, 0.0f);
	m_min = glm::vec3(0.0f, 0.0f, 0.0f);
	LoadModel(filePath);
}

CModel::CModel(std::string const& filePath)
{
	LoadBase(filePath);
}

/// <summary>
/// Load selected object file
/// </summary>
/// <param name="pathName">: The path of selected OBJ file </param>
void CModel::LoadModel(LPCTSTR const& pathName)
{
	Assimp::Importer importer;
	path = ConvertStdString(pathName);
	const aiScene* model_scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_ValidateDataStructure);

	if (!model_scene || model_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !model_scene->mRootNode)
	{
		TRACE(importer.GetErrorString());
		return;
	}
  	ProcessNode(model_scene->mRootNode, model_scene);
}


/// <summary>
/// 
/// </summary>
/// <param name="mesh"></param>
/// <param name="scene"></param>
void CModel::SaveModel()
{
	Assimp::Exporter exporter;
	Assimp::Importer importer;

	const aiScene* model_scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ValidateDataStructure);

	if (!model_scene || model_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !model_scene->mRootNode)
	{
		TRACE(importer.GetErrorString());
		return;
	}

	const aiNode* node = model_scene->mRootNode;
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		SaveNode(node->mChildren[i], model_scene);
	}

	exporter.Export(model_scene, "obj", "D:\\Project\\Research\\SynthDefect\\test\\test.obj");
}

/// <summary>
/// Load grid formed mesh for defect patch
/// </summary>
/// <param name="pathName"> The path of default grid mesh OBJ file </param>
void CModel::LoadBase(std::string const& pathName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pathName,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ValidateDataStructure);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		TRACE(importer.GetErrorString());
		return;
	}
	ProcessNode(scene->mRootNode, scene);
}

/// <summary>
/// Draw the model, and thus all its meshes
/// </summary>
/// <param name="shader">: linked shaders</param>
void CModel::DrawModel()
{
	for (unsigned int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].Draw();
	}
}

/// <summary>
/// Process a node in Assimp's structure in a recursive way
/// </summary>
/// <param name="node">: A part of structure of the imported data</param>
/// <param name="scene">: The root structure of the imported data</param>
void CModel::ProcessNode(const aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene
		// the scene contains all the data, node is just to keep stuff organized
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(ProcessMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

/// <summary>
/// 
/// </summary>
/// <param name="node"></param>
/// <param name="scene"></param>
void CModel::SaveNode(const aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene
		// the scene contains all the data, node is just to keep stuff organized
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			mesh->mVertices[i].x = GetVerticesFromModel().at(i).Position.x;
			mesh->mVertices[i].y = GetVerticesFromModel().at(i).Position.y;
			mesh->mVertices[i].z = GetVerticesFromModel().at(i).Position.z;
		}
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		SaveNode(node->mChildren[i], scene);
	}
}

/// <summary>
/// Translate an aiMesh object of Assimp to a mesh object to be albe to render using OpenGL
/// </summary>
/// <param name="mesh">: The Mesh data of Assimp structure</param>
/// <param name="scene">: The root structure of the imported data</param>
/// <returns> Mesh that can be rendered using OpenGL</returns>
CMesh CModel::ProcessMesh(const aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	std::vector<aiFace>		    faces;

	// Iterate each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vecForThreeComp;
		// positions
		vecForThreeComp.x = mesh->mVertices[i].x;
		vecForThreeComp.y = mesh->mVertices[i].y;
		vecForThreeComp.z = mesh->mVertices[i].z;
		vertex.Position = vecForThreeComp;

		// find the max, min value of x, y, z
		FindMinMaxVertex(vertex.Position);

		vertex.Color.r = MODEL_R;
		vertex.Color.g = MODEL_G;
		vertex.Color.b = MODEL_B;
		vertex.Color.a = MODEL_A;

		// normals
		if (IsNormalsExisted(mesh))
		{
			vecForThreeComp.x = mesh->mNormals[i].x;
			vecForThreeComp.y = mesh->mNormals[i].y;
			vecForThreeComp.z = mesh->mNormals[i].z;
			vertex.Normal = vecForThreeComp;
		}
		else
			vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
		
		// texture coordinates
		if (IsTexCoordsExisted(mesh))
		{
			glm::vec2 vecForTwoComp;
			// Assimp allows a model to have up to 8 different texture coordinates per vertex.
			vecForTwoComp.x = mesh->mTextureCoords[0][i].x;
			vecForTwoComp.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vecForTwoComp;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		// tangent
		if (IsTangentsExisted(mesh))
		{
			vecForThreeComp.x = mesh->mTangents[i].x;
			vecForThreeComp.y = mesh->mTangents[i].y;
			vecForThreeComp.z = mesh->mTangents[i].z;
			vertex.Tangent = vecForThreeComp;
		}
		else
			vertex.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
		
		// bitangent
		if (IsBiTangentsExisted(mesh))
		{
			vecForThreeComp.x = mesh->mBitangents[i].x;
			vecForThreeComp.y = mesh->mBitangents[i].y;
			vecForThreeComp.z = mesh->mBitangents[i].z;
			vertex.BiTangent = vecForThreeComp;
		}
		else
			vertex.BiTangent = glm::vec3(0.0f, 0.0f, 0.0f);
		vertices.push_back(vertex);
	}

	// Iterate each of the mesh's faces
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		faces.push_back(face);
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process indieces
	return CMesh(faces, vertices, indices);
}

/// <summary>
/// Search and Save the max and min value of each coordinate
/// </summary>
/// <param name="vertex_pos"> XYZ coordinates for a vertex of mesh </param>
void CModel::FindMinMaxVertex(glm::vec3 const& vertex_pos)
{
	m_max.x = glm::max(m_max.x, vertex_pos.x);
	m_min.x = glm::min(m_min.x, vertex_pos.x);
	m_max.y = glm::max(m_max.y, vertex_pos.y);
	m_min.y = glm::min(m_min.y, vertex_pos.y);
	m_max.z = glm::max(m_max.z, vertex_pos.z);
	m_min.z = glm::min(m_min.z, vertex_pos.z);
}

/// <summary>
/// Check whether the texture coordinates is existed or not in aiMesh structure
/// </summary>
/// <param name="mesh">: Meshes in loaded model </param>
/// <returns> TRUE if existed, otherwise FALSE </returns>
BOOL CModel::IsTexCoordsExisted(const aiMesh* mesh)
{
	if (mesh->mTextureCoords[0])
		return TRUE;
	else
		return FALSE;
}

/// <summary>
/// Check normal vectors are existed
/// </summary>
/// <param name="mesh">: Meshes in loaded model </param>
/// <returns> TRUE if existed, otherwise FALSE </returns>
BOOL CModel::IsNormalsExisted(const aiMesh* mesh)
{
	if (mesh->mNormals)
		return TRUE;
	else
		return FALSE;
}

/// <summary>
/// Check Tangent vectors are existed
/// </summary>
/// <param name="mesh">: Meshes in loaded model </param>
/// <returns> TRUE if existed, otherwise FALSE </returns>
BOOL CModel::IsTangentsExisted(const aiMesh* mesh)
{
	if (mesh->mTangents)
		return TRUE;
	else
		return FALSE;
}

/// <summary>
/// Check BiTangent vectors are existed
/// </summary>
/// <param name="mesh">: Meshdes in loaded model </param>
/// <returns> TRUE if existed, otherwise FALSE </returns>
BOOL CModel::IsBiTangentsExisted(const aiMesh* mesh)
{
	if (mesh->mBitangents)
		return TRUE;
	else
		return FALSE;;
}

/// <summary>
/// Retrieve faces of a model
/// </summary>
/// <returns> a vector for aiFace </returns>
std::vector<aiFace> CModel::GetFacesFromModel() const
{
	return m_meshes.at(0).GetFaces();
}

/// <summary>
/// Retrieve vertices of a model
/// </summary>
/// <returns> the vector for Vertex </returns>
std::vector<Vertex> CModel::GetVerticesFromModel() const
{
	return  m_meshes.at(0).GetVertices();
}

/// <summary>
/// Calculate and Retrieve the centroid of the loaded model
/// </summary>
/// <returns> the geometry centroid of the loaded model </returns>
glm::vec3 CModel::GetModelCentroid() const
{
	float centerX = (m_max.x + m_min.x) / 2.0f;
	float centerY = (m_max.y + m_min.y) / 2.0f;
	float centerZ = (m_max.z + m_min.z) / 2.0f;
	return glm::vec3(centerX, centerY, centerZ);
}

/// <summary>
/// Calculate the scale factor for fitting the loaded model into veiwport
/// </summary>
/// <param name="cam_pos">: the position of camera </param>
/// <param name="center">: the geometry centroid of the loaded model </param>
/// <returns> the scale factor for the loaded model </returns>
float CModel::GetModelScaleFactor(glm::vec3 const& cam_pos, glm::vec3 const& center) const
{
	float r = glm::distance(center, m_max);					// the radius of bounding sphere
	float z = glm::distance(center, cam_pos);				// the distance from model to camera
	float r_max = z * glm::sin(glm::radians(FOV) / 2.0f);	// the maximum radius of bounding sphere
	return r_max / r;
}

/// <summary>
/// Update the mesh of the loaded model
/// </summary>
void CModel::UpdateModel(std::vector<Vertex> vertices)
{
	m_meshes.at(0).Update(vertices);
}

/// <summary>
/// Retrieve minimum value of coordinates in Bounding box for model
/// </summary>
/// <returns> vector of minimum value of coordinates in Bounding box </returns>
glm::vec3 CModel::GetBoundingBoxMinValue() const
{
	glm::vec3 min = glm::vec3(1000.0f, 1000.0f, 1000.0f);
	std::vector<Vertex> v = GetVerticesFromModel();
	for (int i = 0; i < v.size(); i++)
	{
		min.x = glm::min(min.x, v[i].Position.x);
		min.y = glm::min(min.y, v[i].Position.y);
		min.z = glm::min(min.z, v[i].Position.z);
	}
	return min;
}

/// <summary>
/// Retrieve maximum value of coordinates in Bounding box for model
/// </summary>
/// <returns> vector of maximum value of coordinates in Bounding box </returns>
glm::vec3 CModel::GetBoundingBoxMaxValue() const
{
	return m_max;
}

CModel::~CModel()
{
}