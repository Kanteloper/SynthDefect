
// Model.h : Interface of Model class

#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <vector>
#include "Mesh.h"
#include "Shader.h"

class CModel
{
	// Constructor
public:
	CModel::CModel();
	CModel::CModel(LPCTSTR filePath);

	// Attributes
public:
	glm::vec3 m_max = glm::vec3(0.0f, 0.0f, 0.0f);				// max value of coordinates
	glm::vec3 m_min = glm::vec3(0.0f, 0.0f, 0.0f);				// min value of coordinates

private:
	// model data
	std::vector<CMesh> m_meshes;
	glm::vec3 m_rayPoint;
	// Impelementation
public:
	void DrawModel(CShader& shaders);
	std::vector<aiFace> GetFacesFromModel();
	std::vector<Vertex> GetVerticesFromModel();
	~CModel();

private:
	void LoadModel(LPCTSTR pathName);
	void ProcessNode(aiNode* node, const aiScene* scene);
	CMesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	BOOL IsTexCoordsExisted(aiMesh *mesh);
	BOOL IsNormalsExisted(aiMesh *mesh);
	BOOL IsTangentsExisted(aiMesh *mesh);
	BOOL IsBiTangentsExisted(aiMesh *mesh);
};

