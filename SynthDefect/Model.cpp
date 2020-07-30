// Model.cpp for 3D model data

#include "pch.h"
#include <string>


CModel::CModel()
{
}

CModel::CModel(LPCTSTR filePath)
{
	LoadModel(filePath);
}

CModel::~CModel()
{
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

	ProcessNode(scene->mRootNode, scene);
}


/// <summary>
/// Process a node in Assimp's data structure
/// </summary>
/// <param name="node">: A node in Assimp's data structure(Tree)</param>
/// <param name="scene">: The root structure of the imported data</param>
void CModel::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	// then do the same for each of child node
}


/// <summary>
/// 
/// </summary>
/// <param name="mesh"></param>
/// <param name="scene"></param>
/// <returns></returns>
CMesh CModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	return CMesh();
}