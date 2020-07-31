
// Mesh.h : interface of the Mesh class
//

#pragma once

#include <gl/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>


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
	glm::vec3 Bitanget;
};


class CMesh
{
// Construction
public:
	CMesh();
	CMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) : m_vertices(vertices), m_indices(indices) 
	{
		setupMesh();
	};

// Attributes
private:
	// render data
	// VAO - Vertex Array Object, VBO - Vertex Buffer Object, EBO - Element Buffer Object
	unsigned int VAO, VBO, EBO;

public:
	// mesh data
	std::vector<Vertex>			m_vertices;
	std::vector<unsigned int>	 m_indices;

// Implementation
public:
	virtual ~CMesh();
	void setupMesh();
};

