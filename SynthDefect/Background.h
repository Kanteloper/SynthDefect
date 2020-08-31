#pragma once

// Background.h : interface of the Background class

#include "Mesh.h"

class CBackground : public CMesh
{
	// Constructor
public:
	CBackground();

	// Attributes
private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;

	// Implements
private:
	virtual void SetupMesh();
	void SetDefaultBackground(glm::vec3 v);

public:
	~CBackground();
	void Draw();
};

