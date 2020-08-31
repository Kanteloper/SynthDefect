
// BasePlane.h : Interface of BasePlane class

#pragma once
#include "Mesh.h"
class CBasePlane : public CMesh
{
	// Constructor
public:
	CBasePlane();

	// Attributes
private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;
	glm::vec3 m_max;
	glm::vec3 m_min;

	// Implementation
private:
	void SetBasePlane();
	virtual void SetupMesh();
	void FindMinMaxVertex(glm::vec3 const& p);
public:
	~CBasePlane();
	virtual void Draw();
	glm::vec3 GetBaseCentroid();
};

