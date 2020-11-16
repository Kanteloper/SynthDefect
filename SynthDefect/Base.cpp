#include "pch.h"
#include "Base.h"
#include <assimp/Exporter.hpp>

CBase::CBase(std::string const& filePath)
{
	path = filePath;
	LoadBase(filePath);
}

/// <summary>
/// Load grid formed mesh for defect patch
/// </summary>
/// <param name="pathName"> The path of default grid mesh OBJ file </param>
void CBase::LoadBase(std::string const& pathName)
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

CBase::~CBase()
{
}
