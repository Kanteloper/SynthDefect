#pragma once

// Model.h : interface of Model class

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <vector>
#include <string>

class Model
{
	// Constructor
public:
	Model();

	// Attributes
public:
	// model data
	std::vector<Mesh>	meshes;

	// Impelementation
public:
	~Model();
	

private:
	BOOL LoadModel(LPCTSTR pathName);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
};

