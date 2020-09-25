#include "pch.h"
#include "Pipeline.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/normal.hpp>
#include <cuda_runtime.h>
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
	std::uniform_int_distribution<int> dis(0, (int)(m_pickIndices.size() - 1));
	// select faces that user picks randomly
	for (int i = 0; i < m_numOfDefects; i++)
		random_indices.push_back(m_pickIndices.at(dis(gen)));

	for (int i = 0; i < random_indices.size(); i++)
	{
		aiFace random_face = m_faces[random_indices.at(i)];
		glm::mat4 position_matrix = CalculatePositionMatrix(glm::mat4(1.0f), random_face);
		std::cout << glm::to_string(position_matrix) << std::endl;

		// multiply pisition matrix for each vertex

	}
}

/// <summary>
/// Calculate the centroid of Triangles
/// </summary>
/// <param name="f"> A face among faces that selected randomly </param>
/// <returns> the centroid of the triangle(face) </returns>
glm::vec3 CPipeline::CalculateTriangleCentroid(aiFace const& f)
{
	glm::vec3 A = m_vertices[f.mIndices[0]].Position;
	glm::vec3 B = m_vertices[f.mIndices[1]].Position;
	glm::vec3 C = m_vertices[f.mIndices[2]].Position;
	return glm::vec3((A.x + B.x + C.x) / 3.0f, (A.y + B.y + C.y) / 3.0f, (A.z + B.z + C.z) / 3.0f);
}

/// <summary>
/// 
/// </summary>
/// <param name="m"></param>
/// <param name="v"></param>
/// <param name="f"></param>
/// <returns></returns>
glm::mat4 CPipeline::CalculatePositionMatrix(glm::mat4 const& m, aiFace const& f)
{
	glm::mat4 result(m);
	glm::vec3 face_center = CalculateTriangleCentroid(f);
	glm::vec3 face_normal = glm::triangleNormal(m_vertices[f.mIndices[0]].Position, m_vertices[f.mIndices[1]].Position, m_vertices[f.mIndices[2]].Position);
	std::cout << "face normal: " << glm::to_string(face_normal) << std::endl;
	// the normal of grid base mesh
	// the normla of randomly picked face


	// rotation
	result = glm::translate(result, face_center);

	return result;
}

CPipeline::~CPipeline()
{
}
