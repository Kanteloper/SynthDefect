
// Mesh.h : interface of the Mesh class

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Shader.h"

// this structure represents 'Vertex'
// each item means an attribute of 'Vertex'
struct Vertex {
	// position
	glm::vec3 Position;
	// color
	glm::vec4 Color;
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
	CMesh(std::vector<aiFace> faces, std::vector<Vertex> vertices, std::vector<unsigned int> indices) 
		: m_faces(faces), m_vertices(vertices), m_indices(indices) 
	{
		// set the vertex buffers and its attribute pointers
		setupMesh();
	};

// Attributes
private:
	// render data
	// VAO - Vertex Array Object, VBO - Vertex Buffer Object, EBO - Element Buffer Object
	unsigned int m_VAO, m_VBO, m_EBO = 0;
	// mesh data
	std::vector<Vertex>			m_vertices;
	std::vector<unsigned int>	m_indices;
	std::vector<aiFace>			m_faces;

// Implementation
protected :
	virtual void setupMesh();
public:
	virtual ~CMesh();
	void Draw(const CShader& shaders);
	std::vector<aiFace> GetFaces();
	std::vector<Vertex> GetVertices();
};

