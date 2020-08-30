#pragma once

// Background.h : interface of the Background class

#include "Mesh.h"

class CBackground : public CMesh
{
	// Constructor
public:
	CBackground()
	{
		SetDefaultBackground(glm::vec3(1.0f, 1.0f, 0.0f));
		SetupMesh();
	};

	// Attributes
private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	unsigned int m_VAO = 0, m_VBO = 0, m_EBO = 0;

	// Implements
private:
	virtual void SetupMesh();
	void SetDefaultBackground(glm::vec3 v);

public:
	~CBackground();
	void Draw();
};

