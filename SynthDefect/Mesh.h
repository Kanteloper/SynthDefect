
// Mesh.h : interface of the Mesh class

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shader.h"


struct Vertex {
	// position
	glm::vec3 Position;
	// surface normal
	glm::vec3 Normal;
	// texture Coordinates
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 BiTangent;
};


class CMesh
{
// Construction
public:
	CMesh();
	CMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : m_vertices(vertices), m_indices(indices) 
	{
		// set the vertex buffers and its attribute pointers
		setupMesh();
	};

// Attributes
private:
	// render data
	// VAO - Vertex Array Object, VBO - Vertex Buffer Object, EBO - Element Buffer Object
	unsigned int m_VAO, m_VBO, m_EBO;

public:
	// mesh data
	std::vector<Vertex>			m_vertices;
	std::vector<unsigned int>	 m_indices;

// Implementation
private:
	void setupMesh();
public:
	virtual ~CMesh();
	void Draw(CShader& shaders);
	
};

