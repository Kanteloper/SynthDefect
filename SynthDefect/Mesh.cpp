
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
	GLenum err;
	// generate the reference of a VAO, VBO and an EBO 
	glGenVertexArrays(1, &m_VAO);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: glGenVertexArrays - %d\n", err);
	glGenBuffers(1, &m_VBO);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: glGenVBOBuffers - %d\n", err);
	glGenBuffers(1, &m_EBO);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: glGenEBOBuffers - %d\n", err);

	// Vertices
	glBindVertexArray(m_VAO);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: glBindVertexArray - %d\n", err);
	// Vertex Attributes
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: glBindVBOBuffer - %d\n", err);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: glVBOBufferData - %d\n", err);
	// Vertex Array Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: glBindEBOBuffer - %d\n", err);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_DYNAMIC_DRAW);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: glEBOBufferData - %d\n", err);

	// vertex positions - x, y, z
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex color - r, g, b, a
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
	// vertex normals - nor_x, nor_y, nor_z
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coordinates - tex_x, tex_y
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	// tangent - tan_x, tan_y, tan_z
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
	// bitangent - btan_x, btan_y, btan_z
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BiTangent));
	// break the existing vertex array object binding
	glBindVertexArray(0);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: define an array of generic vertex attribute data - %d\n", err);
}


/// <summary>
/// Render the Model Mesh
/// </summary>
/// <param name="shaders">: liked Shader objects</param>
void CMesh::Draw(CShader& shaders)
{
	GLenum err;
	// bind appropriate textures
	// draw mesh
	glBindVertexArray(m_VAO);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: glBindVertexArray - %d\n", err);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: glDrawElements - %d\n", err);

	glBindVertexArray(0);
}


/// <summary>
/// Retrieve faces of a mesh
/// </summary>
/// <returns> a vector that faces of a mesh is stored </returns>
std::vector<aiFace> CMesh::GetFaces()
{
	return m_faces;
}


/// <summary>
/// Retrieve vertices of a mesh
/// </summary>
/// <returns> a vector that vertices of a mesh is stored </returns>
std::vector<Vertex> CMesh::GetVertices()
{
	return m_vertices;
}

CMesh::~CMesh()
{
}