
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
	void DrawModel(const CShader& shaders);
	std::vector<aiFace> GetFacesFromModel();
	std::vector<Vertex> GetVerticesFromModel();
	glm::vec3 GetModelCentroid();
	~CModel();

private:
	void LoadModel(LPCTSTR const& pathName);
	void ProcessNode(const aiNode* node, const aiScene* scene);
	CMesh ProcessMesh(const aiMesh* mesh, const aiScene* scene);
	void FindMinMaxVertex(glm::vec3 const& vertex_pos);
	BOOL IsTexCoordsExisted(const aiMesh *mesh);
	BOOL IsNormalsExisted(const aiMesh *mesh);
	BOOL IsTangentsExisted(const aiMesh *mesh);
	BOOL IsBiTangentsExisted(const aiMesh *mesh);
};

