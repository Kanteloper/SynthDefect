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
		aiFace random_face = m_modelFaces[random_indices.at(i)];
		glm::mat4 position_matrix = CalculatePositionMatrix(glm::mat4(1.0f), random_face);
		std::cout << glm::to_string(position_matrix) << std::endl;

		// multiply pisition matrix for each vertex using cuda
	}
}

/// <summary>
/// Calculate Transformation Matrix for Positioining
/// </summary>
/// <param name="m"> Identity matrix </param>
/// <param name="f"> the face that randomly select </param>
/// <returns> 4 x 4 Matrix for Positioning </returns>
glm::mat4 CPipeline::CalculatePositionMatrix(glm::mat4 const& m, aiFace const& f)
{
	glm::mat4 result(m);
	glm::vec3 face_A = m_modelVertices[f.mIndices[0]].Position;
	glm::vec3 face_B = m_modelVertices[f.mIndices[1]].Position;
	glm::vec3 face_C = m_modelVertices[f.mIndices[2]].Position;

	aiFace base_face = m_baseFaces[0];											// pick one face between any faces
	glm::vec3 base_A = m_baseVertices[base_face.mIndices[0]].Position;
	glm::vec3 base_B = m_baseVertices[base_face.mIndices[1]].Position;
	glm::vec3 base_C = m_baseVertices[base_face.mIndices[2]].Position;

	glm::vec3 face_center = CalculateTriangleCentroid(face_A, face_B, face_C);
	glm::vec3 face_normal = glm::triangleNormal(face_A, face_B, face_C);
	glm::vec3 base_normal = glm::triangleNormal(base_A, base_B, base_C);

	float angle = glm::acos(glm::dot(face_normal, base_normal));
	glm::vec3 axis = glm::normalize(glm::cross(face_normal, base_normal));

	// Transformation for Positioning : Translation -> Rotation
	result = glm::translate(result, face_center);
	result = glm::rotate(result, angle, axis);
	return result;
}

/// <summary>
/// Calculate the centroid of Triangles
/// </summary>
/// <param name="A"> first point of a face </param>
/// <param name="B"> second point of a face </param>
/// <param name="C"> third point of a face </param>
/// <returns> the centroid of the triangle(face) </returns>
glm::vec3 CPipeline::CalculateTriangleCentroid(glm::vec3 const& A, glm::vec3 const& B, glm::vec3 const& C)
{
	return glm::vec3((A.x + B.x + C.x) / 3.0f, (A.y + B.y + C.y) / 3.0f, (A.z + B.z + C.z) / 3.0f);
}


CPipeline::~CPipeline()
{
}
