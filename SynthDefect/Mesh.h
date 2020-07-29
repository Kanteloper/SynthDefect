#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct Vertex {
	// position
	glm::vec3 Position;
	// surface normal
	glm::vec3 Normal;
	// texCoordinates
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitanget;
};


class Mesh
{
public:
	// mesh data
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	 indices;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

	// implementation
	void setupMesh();

private:
	// render data
	unsigned int VBO;
};

