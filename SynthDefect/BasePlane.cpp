#include "pch.h"
#include "BasePlane.h"

#include <iostream>

#define HALF_GRID_SIZE VERTEX_NUM / 2

CBasePlane::CBasePlane()
{
	m_VAO = m_VBO = m_EBO = 0;
	m_max = m_min = glm::vec3(0.0f, 0.0f, 0.0f);
	SetBasePlane();
	SetupMesh();
}


/// <summary>
/// Set base floor mesh with grid pattern 
/// </summary>
void CBasePlane::SetBasePlane()
{
	// save vertex
	for (int i = -HALF_GRID_SIZE; i < HALF_GRID_SIZE; i++)
	{
		for (int j = -HALF_GRID_SIZE; j < HALF_GRID_SIZE; j++)
		{
			Vertex p; 
			p.Position.x = (float)j / HALF_GRID_SIZE * BASE_SIZE;
			p.Position.y = 0.0f;
			p.Position.z = (float)i / HALF_GRID_SIZE * BASE_SIZE;
			FindMinMaxVertex(p.Position);
			p.Color.r = 0.5f;
			p.Color.g = 0.5f;
			p.Color.b = 0.5f;
			p.Normal.x = 0.0f;
			p.Normal.y = 1.0f;
			p.Normal.z = 0.0f;
			m_vertices.push_back(p);
		}
	}
	// save index
	for (unsigned int z = 0; z < VERTEX_NUM - 1; z++)
	{
		for (unsigned int x = 0; x < VERTEX_NUM - 1; x++)
		{
			int top_left = (z * VERTEX_NUM) + x;
			int top_right = top_left + 1;
			int bottom_left = ((z + 1) * VERTEX_NUM) + x;
			int bottom_right = bottom_left + 1;
			m_indices.push_back(top_left);
			m_indices.push_back(bottom_left);
			m_indices.push_back(top_right);
			m_indices.push_back(top_right);
			m_indices.push_back(bottom_left);
			m_indices.push_back(bottom_right);
		}
	}
}


/// <summary>
/// Initialize all the buffer objects / arrays
/// </summary>
void CBasePlane::SetupMesh()
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
	glBindVertexArray(0);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: define an array of generic vertex attribute data - %d\n", err);
}


/// <summary>
/// Search and Save the max and min value of each coordinate
/// </summary>
void CBasePlane::FindMinMaxVertex(glm::vec3 const& p)
{
	m_max.x = glm::max(m_max.x, p.x);
	m_min.x = glm::min(m_min.x, p.x);
	m_max.y = glm::max(m_max.y, p.y);
	m_min.y = glm::min(m_min.y, p.y);
	m_max.z = glm::max(m_max.z, p.z);
	m_min.z = glm::min(m_min.z, p.z);
}

/// <summary>
/// Render the base plane
/// </summary>
void CBasePlane::Draw()
{
	GLenum err;
	// bind appropriate textures
	// draw mesh
	glBindVertexArray(m_VAO);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: glBindVertexArray - %d\n", err);
	glDrawElements(GL_LINES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
	if ((err = glGetError()) != GL_NO_ERROR)
		TRACE1("ERROR: glDrawElements - %d\n", err);

	glBindVertexArray(0);
}


/// <summary>
/// Calculate and Retrieve the centroid of the base plane
/// </summary>
/// <returns> the geometry centroid of the base plane </returns>
glm::vec3 CBasePlane::GetBaseCentroid()
{
	float centerX = (m_max.x + m_min.x) / 2.0f;
	float centerY = (m_max.y + m_min.y) / 2.0f;
	float centerZ = (m_max.z + m_min.z) / 2.0f;
	return glm::vec3(centerX, centerY, centerZ);
}


CBasePlane::~CBasePlane()
{
}