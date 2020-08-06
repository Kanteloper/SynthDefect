
// Mesh.cpp for Mesh data as parts of 3D model

#include "pch.h"
#include <gl/glew.h>
#include <gl/GLU.h>
#include "Mesh.h"


CMesh::CMesh()
{
}


/// <summary>
/// Initialize all the buffer objects / arrays
/// </summary>
void CMesh::setupMesh()
{
	// generate the reference of a VAO, VBO and an EBO 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Vertices
	glBindVertexArray(VAO);
	// Vertex Attributes
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);
	// Vertex Array Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_DYNAMIC_DRAW);

	// vertex positions - x, y, z
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals - nor_x, nor_y, nor_z
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coordinates - tex_x, tex_y
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// tangent - tan_x, tan_y, tan_z
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// bitangent - btan_x, btan_y, btan_z
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BiTangent));
	// break the existing vertex array object binding
	glBindVertexArray(0);
	if (glGetError() != 0)
		TRACE("glDraw Error");
}


/// <summary>
/// Render the Mesh
/// </summary>
/// <param name="shaders">: liked Shader objects</param>
void CMesh::Draw(CShader& shaders)
{
	// bind appropriate textures
	// draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (int)m_indices.size(), GL_UNSIGNED_INT, 0);
	if (glGetError() != 0)
		TRACE("glDraw Error");

	glBindVertexArray(0);
}


CMesh::~CMesh()
{
}