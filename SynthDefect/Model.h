
// Model.h : Interface of Model class

#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <vector>

class CModel
{
	// Constructor
public:
	CModel();
	CModel(LPCTSTR filePath);

	// Attributes
public:
	// model data
	std::vector<CMesh>	meshes;

	// Impelementation
public:
	~CModel();

private:
	void LoadModel(LPCTSTR pathName);
	void ProcessNode(aiNode* node, const aiScene* scene);
	CMesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	BOOL IsTexCoordsExisted(aiMesh *mesh);
};

