
// Model.cpp for 3D model data

#include "pch.h"
#include "Model.h"

#include <string>

CModel::CModel() 
{
}


CModel::CModel(LPCTSTR filePath)
{ 
    	LoadModel(filePath);
}


/// <summary>
/// Load selected object file
/// </summary>
/// <param name="pathName">: the path of selected file </param>
/// <returns> TRUE if load success, FALSE on failure </returns>
void CModel::LoadModel(LPCTSTR pathName)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(ConvertStdString(pathName),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ValidateDataStructure);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		TRACE(import.GetErrorString());
		return;
	}

  	CModel::ProcessNode(scene->mRootNode, scene);
}



/// <summary>
/// Draw the model, and thus all its meshes
/// </summary>
/// <param name="shader">: linked shaders</param>
void CModel::DrawModel(CShader& shaders)
{
	for (unsigned int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].Draw(shaders);
	}
}


/// <summary>
/// Process a node in Assimp's structure in a recursive way
/// </summary>
/// <param name="node">: A part of structure of the imported data</param>
/// <param name="scene">: The root structure of the imported data</param>
void CModel::ProcessNode(aiNode* node, const aiScene* scene)
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
/// Translate an aiMesh object of Assimp to a mesh object to be albe to render using OpenGL
/// </summary>
/// <param name="mesh">: The Mesh data of Assimp structure</param>
/// <param name="scene">: The root structure of the imported data</param>
/// <returns> Mesh that can be rendered using OpenGL</returns>
CMesh CModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
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
		m_max.x = glm::max(m_max.x, vertex.Position.x);
		m_min.x = glm::min(m_min.x, vertex.Position.x);
		m_max.y = glm::max(m_max.y, vertex.Position.y);
		m_min.y = glm::min(m_min.y, vertex.Position.y);
		m_max.z = glm::max(m_max.z, vertex.Position.z);
		m_min.z = glm::min(m_min.z, vertex.Position.z);

		// colors
		vertex.Color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);

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
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process indieces
	return CMesh(vertices, indices);
}


/// <summary>
/// Check whether the texture coordinates is existed or not in aiMesh structure
/// </summary>
/// <param name="mesh">: Meshes in loaded Model </param>
/// <returns> TRUE if existed, otherwise FALSE </returns>
BOOL CModel::IsTexCoordsExisted(aiMesh* mesh)
{
	if (mesh->mTextureCoords[0])
		return TRUE;
	else
		return FALSE;
}


/// <summary>
/// 
/// </summary>
/// <param name="mesh"></param>
/// <returns></returns>
BOOL CModel::IsNormalsExisted(aiMesh* mesh)
{
	if (mesh->mNormals)
		return TRUE;
	else
		return FALSE;
}


/// <summary>
/// 
/// </summary>
/// <param name="mesh"></param>
/// <returns></returns>
BOOL CModel::IsTangentsExisted(aiMesh* mesh)
{
	if (mesh->mTangents)
		return TRUE;
	else
		return FALSE;
}


/// <summary>
/// 
/// </summary>
/// <param name="mesh"></param>
/// <returns></returns>
BOOL CModel::IsBiTangentsExisted(aiMesh* mesh)
{
	if (mesh->mBitangents)
		return TRUE;
	else
		return FALSE;;
}


/// <summary>
///	Save the world coordinates of the position which users click in the screen
/// </summary>
/// <param name="point"> the position that users click </param>
void CModel::SetRayPoint(glm::vec3 point)
{
	m_rayPoint = point;
}


CModel::~CModel()
{
}