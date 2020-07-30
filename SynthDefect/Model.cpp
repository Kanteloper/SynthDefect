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
BOOL CModel::LoadModel(LPCTSTR pathName)
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
		return FALSE;
	}

	CModel::ProcessNode(scene->mRootNode, scene);
	return TRUE;
}

void CModel::ProcessNode(aiNode* node, const aiScene* scene)
{
}


//Mesh CModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
//{
//	return Mesh();
//}


