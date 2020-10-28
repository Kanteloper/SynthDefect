
// Model.h : Interface of CModel class

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
	CModel::CModel(LPCTSTR const& filePath);

	// Attributes
private:
	std::vector<CMesh> m_meshes;	// model data
	glm::vec3 m_max;				// max value of coordinates
	glm::vec3 m_min;				// min value of coordinates
	std::string path;

	// Impelementation
public:
	void DrawModel();
	virtual void ExportOBJ(std::string const& fileName);
	std::vector<aiFace> GetFacesFromModel() const;
	std::vector<Vertex> GetVerticesFromModel() const;
	glm::vec3 GetModelCentroid() const;
	float GetModelScaleFactor(glm::vec3 const& cam_pos, glm::vec3 const& center) const;
	void UpdateModel(std::vector<Vertex> vertices);
	glm::vec3 GetBoundingBoxMinValue() const;
	glm::vec3 GetBoundingBoxMaxValue() const;
	~CModel();

protected:
	void ProcessNode(const aiNode* node, const aiScene* scene);
	void SaveNode(const aiNode* node, const aiScene* scene);

private:
	void LoadModel(LPCTSTR const& pathName);
	CMesh ProcessMesh(const aiMesh* mesh, const aiScene* scene);
	void FindMinMaxVertex(glm::vec3 const& vertex_pos);
	BOOL IsTexCoordsExisted(const aiMesh* mesh);
	BOOL IsNormalsExisted(const aiMesh* mesh);
	BOOL IsTangentsExisted(const aiMesh* mesh);
	BOOL IsBiTangentsExisted(const aiMesh* mesh);
};

