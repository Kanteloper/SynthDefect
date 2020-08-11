#pragma once

// Background.h : interface of the Background class

#include "Mesh.h"

class CBackground : public CMesh
{
	// Constructor
public:
	CBackground();
	CBackground(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : m_vertices(vertices), m_indices(indices)
	{
		// set the vertex buffers and its attribute pointers
		setupMesh();
	};

	// Attributes
private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	unsigned int m_VAO = 0, m_VBO = 0, m_EBO = 0;

	// Implements
private:
	virtual void setupMesh();
public:
	~CBackground();
	virtual void Draw(CShader& shaders);
};

