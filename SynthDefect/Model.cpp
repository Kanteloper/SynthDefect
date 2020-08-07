
// Model.cpp for 3D model data

#include "pch.h"
#include "Model.h"

#include <string>

CModel::CModel() 
{
	// add default background mesh
	m_meshes.push_back(SetDefaultBackground());
}


CModel::CModel(LPCTSTR filePath) 
{ 
	// add default background mesh
	//SetDefaultBackground();
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
		aiProcess_RemoveComponent |
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
/// Set background mesh with the default color
/// </summary>
CMesh CModel::SetDefaultBackground()
{
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;

	Vertex top_left, top_right;
	// top left
	top_left.Position.x = -1.0f;
	top_left.Position.y = 1.0f;
	top_left.Position.z = 0.0f;
	vertices.push_back(top_left);
	// top right
	top_right.Position.x = 1.0f;
	top_right.Position.y = 1.0f;
	top_right.Position.z = 0.0f;
	vertices.push_back(top_right);

	Vertex bottom_left, bottom_right;
	// bottom left
	bottom_left.Position.x = 1.0f;
	bottom_left.Position.y = -1.0f;
	bottom_left.Position.z = 0.0f;
	vertices.push_back(bottom_left);
	// bottom right
	bottom_right.Position.x = 1.0f;
	bottom_right.Position.y = -1.0f;
	bottom_right.Position.z = 0.0f;
	vertices.push_back(bottom_right);

	indices = {
		0, 2, 1,
		2, 3, 1
	};

	return CMesh(vertices, indices);
}


/// <summary>
/// Draw the model, and thus all its meshes
/// </summary>
/// <param name="shader">: linked shaders</param>
void CModel::DrawModel(CShader& shaders)
{
	//TRACE1("Log: mesh size: %d\n", m_meshes.size());
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
/// Translate an aiMesh object to a mesh object to be albe to render using OpenGL
/// </summary>
/// <param name="mesh">: The Mesh data of Assimp structure</param>
/// <param name="scene">: The root structure of the imported data</param>
/// <returns> Mesh that can be rendered using OpenGL</returns>
CMesh CModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vecForThreeComp;

		// positions
		vecForThreeComp.x = mesh->mVertices[i].x;
		vecForThreeComp.y = mesh->mVertices[i].y;
		vecForThreeComp.z = mesh->mVertices[i].z;
		vertex.Position = vecForThreeComp;
		// normals
		vecForThreeComp.x = mesh->mNormals[i].x;
		vecForThreeComp.y = mesh->mNormals[i].y;
		vecForThreeComp.z = mesh->mNormals[i].z;
		vertex.Normal = vecForThreeComp;
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
		vecForThreeComp.x = mesh->mTangents[i].x;
		vecForThreeComp.y = mesh->mTangents[i].y;
		vecForThreeComp.z = mesh->mTangents[i].z;
		vertex.Tangent = vecForThreeComp;
		// bitangent
		vecForThreeComp.x = mesh->mBitangents[i].x;
		vecForThreeComp.y = mesh->mBitangents[i].y;
		vecForThreeComp.z = mesh->mBitangents[i].z;
		vertex.BiTangent = vecForThreeComp;
		vertices.push_back(vertex);
	}

	// Walk through each of the mesh's faces
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


CModel::~CModel()
{
}