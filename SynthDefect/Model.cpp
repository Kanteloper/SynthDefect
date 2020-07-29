#include "pch.h"
#include "Model.h"

/// <summary>
/// Load selected object file
/// </summary>
/// <param name="pathName">: the path of selected file </param>
/// <returns> TRUE if load success, FALSE on failure </returns>
BOOL Model::LoadModel(LPCTSTR pathName)
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

	Model::ProcessNode(scene->mRootNode, scene);
	return TRUE;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
}

//Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
//{
//	return Mesh();
//}
