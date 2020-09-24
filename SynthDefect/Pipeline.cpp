#include "pch.h"
#include "Pipeline.h"

#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <random>

CPipeline::CPipeline()
{
	m_base = nullptr;
	m_model = nullptr;
	m_numOfDefects = 0;
}

/// <summary>
/// Execute five pupelines for generating point clouds with synthetic defects
/// </summary>
void CPipeline::Execute()
{
	DoPositioning();
}

/// <summary>
/// Locate the grid base mesh to the face thata selected randomly among faces user picks
/// </summary>
void CPipeline::DoPositioning()
{
	std::vector<int> random_indices;
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> dis(0, m_pickIndices.size() - 1);

	// select faces that user picks randomly
	for (int i = 0; i < m_numOfDefects; i++)
		random_indices.push_back(m_pickIndices.at(dis(gen)));

	for (int i = 0; i < random_indices.size(); i++)
	{
		// calculate the centroid of A face
		glm::vec3 face_center = GetTriangleCentroid(m_faces[random_indices.at(i)]);
		std::cout << glm::to_string(face_center) << std::endl;
	}
	// calculate the origin of base mesh



}

/// <summary>
/// Calculate the centroid of Triangles
/// </summary>
/// <param name="f"> A face among faces that selected randomly </param>
/// <returns> the centroid of the triangle(face) </returns>
glm::vec3 CPipeline::GetTriangleCentroid(aiFace const& f)
{
	glm::vec3 A = m_vertices[f.mIndices[0]].Position;
	glm::vec3 B = m_vertices[f.mIndices[1]].Position;
	glm::vec3 C = m_vertices[f.mIndices[2]].Position;
	return glm::vec3((A.x + B.x + C.x) / 3.0f, (A.y + B.y + C.y) / 3.0f, (A.z + B.z + C.z) / 3.0f);
}

CPipeline::~CPipeline()
{
}
