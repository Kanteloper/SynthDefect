
// Background.cpp for the default background Mesh

#include "pch.h"
#include "Background.h"

CBackground::CBackground()
{
}


/// <summary>
/// Initialize all the buffer objects / arrays
/// </summary>
void CBackground::SetupMesh()
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
	glBindVertexArray(0);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: define an array of generic vertex attribute data - %d\n", err);
}


/// <summary>
/// Set background mesh with the default color
/// </summary>
/// <param name="v">: vertices for background </param>
/// <returns> </returns>
void CBackground::SetDefaultBackground(glm::vec3 v)
{
	Vertex top_left, top_right;
	// top left
	top_left.Position.x = -v.x;
	top_left.Position.y = v.y;
	top_left.Position.z = v.z;
	top_left.Color.r = 0.04f;
	top_left.Color.g = 0.4f;
	top_left.Color.b = 0.6f;
	top_left.Color.a = 1.0f;
	m_vertices.push_back(top_left);
	// top right
	top_right.Position.x = v.x;
	top_right.Position.y = v.y;
	top_right.Position.z = v.z;
	top_right.Color.r = 0.04f;
	top_right.Color.g = 0.4f;
	top_right.Color.b = 0.6f;
	top_right.Color.a = 1.0f;
	m_vertices.push_back(top_right);

	Vertex bottom_left, bottom_right;
	// bottom left
	bottom_left.Position.x = -v.x;
	bottom_left.Position.y = -v.y;
	bottom_left.Position.z = v.z;
	bottom_left.Color.r = 0.0f;
	bottom_left.Color.g = 0.0f;
	bottom_left.Color.b = 0.0f;
	bottom_left.Color.a = 1.0f;
	m_vertices.push_back(bottom_left);
	// bottom right
	bottom_right.Position.x = v.x;
	bottom_right.Position.y = -v.y;
	bottom_right.Position.z = v.z;
	bottom_right.Color.r = 0.0f;
	bottom_right.Color.g = 0.0f;
	bottom_right.Color.b = 0.0f;
	bottom_right.Color.a = 1.0f;
	m_vertices.push_back(bottom_right);

	m_indices = {
		0, 3, 1,
		2, 3, 0
	};
}


/// <summary>
/// Render the default background Mesh
/// </summary>
void CBackground::Draw()
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


CBackground::~CBackground()
{
}
