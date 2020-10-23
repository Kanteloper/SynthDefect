#include "pch.h"
#include "Pipeline.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/normal.hpp>
#include <iostream>
#include <random>

CPipeline::CPipeline()
{
	m_type = 0;
	m_width = 0.0f;
	m_height = 0.0f;
	m_depth = 0.0f;
	m_base = nullptr;
	m_model = nullptr;
}

/// <summary>
/// Execute five pupelines for generating point clouds with synthetic defects
/// </summary>
void CPipeline::Execute()
{
	/*for (int i = 0; i < m_baseVertices.size(); i++)
	{
		std::cout << "before: (" << m_baseVertices[i].Position.x << ", " << m_baseVertices[i].Position.y << ", " << m_baseVertices[i].Position.z << ") " << std::endl;
	}*/
	DoPositioning();
	for (int i = 0; i < m_baseVertices.size(); i++)
	{
		std::cout << "after: (" << m_baseVertices[i].Position.x << ", " << m_baseVertices[i].Position.y << ", " << m_baseVertices[i].Position.z << ") " << std::endl;
	}
	DoDeforming();
	
	// DoScaling();
}

/// <summary>
/// Locate the grid base mesh to the face that selected randomly among faces user picks
/// </summary>
void CPipeline::DoPositioning()
{
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> dis(0, (int)(m_pickIndices.size() - 1));

	// select faces that user picks randomly
	aiFace random_face = m_modelFaces[m_pickIndices.at(dis(gen))];
	glm::mat4 position_matrix = CalculatePositionMatrix(glm::mat4(1.0f), random_face);

	// multiply pisition matrix for each vertex of grid base mesh
	for (int i = 0; i < m_baseVertices.size(); i++)
	{
		m_baseVertices[i].Position = position_matrix * glm::vec4(m_baseVertices[i].Position, 1.0);
		m_base->UpdateModel(m_baseVertices);
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

	result = glm::translate(result, face_center);
	result = glm::rotate(result, -angle, axis);
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

/// <summary>
/// Deform the grid base mesh to fit the feature of each defect
/// </summary>
void CPipeline::DoDeforming()
{
	// construct lattice space
	glm::vec3 lattice_origin = m_base->GetBoundingBoxMinValue();
	std::cout << "lattice origin: " << glm::to_string(lattice_origin) << std::endl;
	glm::vec3 S_axis = glm::vec3(2.4f, 0.0f, 0.0f);
	glm::vec3 T_axis = glm::vec3(0.0f, 2.4f, 0.0f);
	glm::vec3 U_axis = glm::vec3(0.0f, 0.0f, 2.4f);
}

/// <summary>
/// Scale the grid base mesh that is deformed among values user inputs
/// </summary>
void CPipeline::DoScaling()
{
	glm::mat4 scale_matrix = CalculateScaleMatrix(glm::mat4(1.0f));
	for (int i = 0; i < m_baseFaces.size(); i++)
	{
		aiFace base_face = m_baseFaces.at(i);
		m_baseVertices[base_face.mIndices[0]].Position = scale_matrix * glm::vec4(m_baseVertices[base_face.mIndices[0]].Position, 1.0);
		m_baseVertices[base_face.mIndices[1]].Position = scale_matrix * glm::vec4(m_baseVertices[base_face.mIndices[1]].Position, 1.0);
		m_baseVertices[base_face.mIndices[2]].Position = scale_matrix * glm::vec4(m_baseVertices[base_face.mIndices[2]].Position, 1.0);
		m_base->UpdateModel(m_baseVertices);
	}
}

/// <summary>
/// Calculate Transformation Matrix for Differential Scaling
/// </summary>
/// <param name="m"> Identity matrix </param>
/// <returns> 4 x 4 Matrix for Scaling </returns>
glm::mat4 CPipeline::CalculateScaleMatrix(glm::mat4 const& m)
{
	glm::mat4 result(m);
	result = glm::scale(result, glm::vec3(m_width, m_height, m_depth));
	return result;
}


CPipeline::~CPipeline()
{
}
