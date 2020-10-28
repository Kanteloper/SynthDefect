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

	exporter.Export(model_scene, "obj", "..\\test\\" + fileName + ".obj");
}

CBase::~CBase()
{
}
