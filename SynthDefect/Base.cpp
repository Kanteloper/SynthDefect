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

/// <summary>
/// Export mesh file to OBJ file format
/// </summary>
/// <param name="fileName"> The desired file name </param>
void CBase::ExportOBJ(std::string const& fileName)
{
	Assimp::Exporter exporter;
	Assimp::Importer importer;

	const aiScene* base_scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ValidateDataStructure);

	if (!base_scene || base_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !base_scene->mRootNode)
	{
		TRACE(importer.GetErrorString());
		return;
	}

	const aiNode* node = base_scene->mRootNode;
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		SaveNode(node->mChildren[i], base_scene);
	}
	
	exporter.Export(base_scene, "obj", "..\\test\\" + fileName + ".obj");
}

CBase::~CBase()
{
}
