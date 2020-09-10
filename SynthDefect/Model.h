
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
	glm::vec3 m_max;	// max value of coordinates
	glm::vec3 m_min;	// min value of coordinates

private:
	// model data
	std::vector<CMesh> m_meshes;

	// Impelementation
public:
	void DrawModel();
	std::vector<aiFace> GetFacesFromModel() const;
	std::vector<Vertex> GetVerticesFromModel() const;
	glm::vec3 GetModelCentroid() const;
	float GetModelScaleFactor(glm::vec3 const& cam_pos, glm::vec3 const& center) const;
	void UpdateModel(std::vector<Vertex> vertices);
	~CModel();

private:
	void LoadModel(LPCTSTR const& pathName);
	void ProcessNode(const aiNode* node, const aiScene* scene);
	CMesh ProcessMesh(const aiMesh* mesh, const aiScene* scene);
	void FindMinMaxVertex(glm::vec3 const& vertex_pos);
	BOOL IsTexCoordsExisted(const aiMesh* mesh);
	BOOL IsNormalsExisted(const aiMesh* mesh);
	BOOL IsTangentsExisted(const aiMesh* mesh);
	BOOL IsBiTangentsExisted(const aiMesh* mesh);
};

