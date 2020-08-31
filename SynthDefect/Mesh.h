
// Mesh.h : interface of the Mesh class

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "Shader.h"

struct Vertex {
	// position
	glm::vec3 Position;
	// color
	glm::vec4 Color;
	// vertex normal
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
	CMesh(std::vector<aiFace> faces, std::vector<Vertex> vertices, std::vector<unsigned int> indices);

// Attributes
private:
	// render data
	// VAO - Vertex Array Object, VBO - Vertex Buffer Object, EBO - Element Buffer Object
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;

	// mesh data
	std::vector<Vertex>			m_vertices;
	std::vector<unsigned int>	m_indices;
	std::vector<aiFace>			m_faces;

// Implementation
protected :
	virtual void SetupMesh();
public:
	~CMesh();
	virtual void Draw();
	std::vector<aiFace> GetFaces();
	std::vector<Vertex> GetVertices();
};

