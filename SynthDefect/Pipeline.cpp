#include "pch.h"
#include "Pipeline.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/normal.hpp>
#include <iostream>
#include <random>

/******************* Constants *******************/

// Deformation - lattice space
const int S_ORDER = 6;
const int S_DEGREE = 3;
const float S_AXIS_SIZE = 2.4f;
const int T_ORDER = 1;
const int T_DEGREE = 1;
const float T_AXIS_SIZE = 2.4f;
const int U_ORDER = 6;
const int U_DEGREE = 3;
const float U_AXIS_SIZE = 2.4f;

// Deformation - Key control points of each defect
const float MAX_OPEN_HOLE_Y = -0.1f;
const float MIN_OPEN_HOLE_Y = -0.5f;
const float MAX_PIPE_MID_Y = -0.5f;
const float MIN_PIPE_MID_Y = -1.0f;
const float MAX_PIPE_SIDE_Y = -0.1f;
const float MIN_PIPE_SIDE_Y = -0.5f;
const float MAX_CAVED_SURFACE_MID_Y = -0.1f;
const float MIN_CAVED_SURFACE_MID_Y = -0.15f;
const float MAX_CAVED_SURFACE_SIDE_Y = -0.05f;
const float MIN_CAVED_SURFACE_SIDE_Y = -0.1f;

// Deformation - Knot vectors
const std::array<float, 10> U = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 2.5f, 2.5f, 2.5f, 2.5f };
const std::array<float, 10> W = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 2.5f, 2.5f, 2.5f, 2.5f };

// Deformation - Weights
const float MAX_OPEN_HOLE_WEIGHT = 2.0f;
const float MIN_OPEN_HOLE_WEIGHT = 0.0f;
const float MAX_PIPE_WEIGHT = 2.0f;
const float MIN_PIPE_WEIGHT = 0.3f;
const float MAX_CAVED_SURFACE_WEIGHT = 3.0f;
const float MIN_CAVED_SURFACE_WEIGHT = 1.0f;

/******************* Structures *******************/

struct SAxis {
	glm::vec3 axis = glm::vec3(S_AXIS_SIZE, 0.0f, 0.0f);
	int order = S_ORDER;
	int degree = S_DEGREE;
};
struct TAxis {
	glm::vec3 axis = glm::vec3(0.0f, T_AXIS_SIZE, 0.0f);
	int order = T_ORDER;
	int degree = T_DEGREE;
};
struct UAxis {
	glm::vec3 axis = glm::vec3(0.0f, 0.0f, U_AXIS_SIZE);
	int order = U_ORDER;
	int degree = U_DEGREE;
};

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
	// select faces that user picks randomly
	std::random_device rd;
	std::mt19937_64 gen(rd());
	std::uniform_int_distribution<int> dis(0, (int)(m_pickIndices.size() - 1));
	aiFace random_face = m_modelFaces[m_pickIndices.at(dis(gen))];

	glm::mat4 position_matrix = CalculatePositionMatrix(glm::mat4(1.0f), random_face);

	//for (int i = 0; i < m_baseVertices.size(); i++)
	//{
	//	std::cout << "before: (" << m_baseVertices[i].Position.x << ", " << m_baseVertices[i].Position.y << ", " << m_baseVertices[i].Position.z << ") " << std::endl;
	//}

	DoDeforming();

	/*for (int i = 0; i < m_baseVertices.size(); i++)
	{
		std::cout << "before positioning: (" << m_baseVertices[i].Position.x << ", " << m_baseVertices[i].Position.y << ", " << m_baseVertices[i].Position.z << ") " << std::endl;
	}*/

	DoPositioning(position_matrix);

	//for (int i = 0; i < m_baseVertices.size(); i++)
	//{
	//	std::cout << "after: (" << m_baseVertices[i].Position.x << ", " << m_baseVertices[i].Position.y << ", " << m_baseVertices[i].Position.z << ") " << std::endl;
	//}

	DoScaling();

	DoModeling();

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
	glm::vec3 origin = m_base->GetBoundingBoxMinValue();
	std::cout << "lattice origin: " << glm::to_string(origin) << std::endl;

	// Initiate control points
	std::array<glm::vec3, 36> control_points = InitControlPoints(origin, m_type);

	// Initiate Weights
	std::array<std::array<float, 3>, 4> weight = InitWeight(m_type);

	// Deform
	DeformGridBase(origin, m_type, control_points, weight);
}

/// <summary>
/// Initiate control points of grid base mesh
/// </summary>
/// <param name="origin"> The origin of lattice space </param>
/// <param name="type"> The type of defect </param>
/// <returns> Array fo 36 control points </returns>
std::array<glm::vec3, 36> CPipeline::InitControlPoints(glm::vec3 const& origin, int type)
{
	SAxis s_axis;
	TAxis t_axis;
	UAxis u_axis;
	std::array < glm::vec3, 36> cp_array;
	std::random_device rd;
	std::mt19937_64 gen(rd());

	float max_mid = 0.0f;
	float min_mid = 0.0f;
	float max_side = 0.0f;
	float min_side = 0.0f;
	switch (type)
	{
	case 1:	// Open Hole
		max_mid = MAX_OPEN_HOLE_Y;
		min_mid = MIN_OPEN_HOLE_Y;
		break;
	case 2:	// Pipe
		max_mid = MAX_PIPE_MID_Y;
		min_mid = MIN_PIPE_MID_Y;
		max_side = MAX_PIPE_SIDE_Y;
		min_side = MIN_PIPE_SIDE_Y;
		break;
	case 3:	// Caved Surface
		max_mid = MAX_CAVED_SURFACE_MID_Y;
		min_mid = MIN_CAVED_SURFACE_MID_Y;
		max_side = MAX_CAVED_SURFACE_SIDE_Y;
		min_side = MIN_CAVED_SURFACE_SIDE_Y;
		break;
	}

	std::uniform_real_distribution<float> mid_cp_y(min_mid, max_mid);
	std::uniform_real_distribution<float> side_cp_y(min_side, max_side);

	// (0, 0, 0) <= (i,j,k) <= (0, 0, 5)
	cp_array[0].x = origin.x + ((0.0f / s_axis.order) * s_axis.axis.x);
	cp_array[0].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[0].z = origin.z + ((0.0f / u_axis.order) * u_axis.axis.z);
	cp_array[1].x = origin.x + ((0.0f / s_axis.order) * s_axis.axis.x);
	cp_array[1].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[1].z = origin.z + ((1.0f / u_axis.order) * u_axis.axis.z);
	cp_array[2].x = origin.x + ((0.0f / s_axis.order) * s_axis.axis.x);
	cp_array[2].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[2].z = origin.z + ((2.0f / u_axis.order) * u_axis.axis.z);
	cp_array[3].x = origin.x + ((0.0f / s_axis.order) * s_axis.axis.x);
	cp_array[3].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[3].z = origin.z + ((3.0f / u_axis.order) * u_axis.axis.z);
	cp_array[4].x = origin.x + ((0.0f / s_axis.order) * s_axis.axis.x);
	cp_array[4].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[4].z = origin.z + ((4.0f / u_axis.order) * u_axis.axis.z);
	cp_array[5].x = origin.x + ((0.0f / s_axis.order) * s_axis.axis.x);
	cp_array[5].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[5].z = origin.z + ((5.0f / u_axis.order) * u_axis.axis.z);

	// (1, 0, 0) <= (i,j,k) <= (1, 0, 5)
	cp_array[6].x = origin.x + ((1.0f / s_axis.order) * s_axis.axis.x);
	cp_array[6].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[6].z = origin.z + ((0.0f / u_axis.order) * u_axis.axis.z);
	cp_array[7].x = origin.x + ((1.0f / s_axis.order) * s_axis.axis.x);
	cp_array[7].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[7].z = origin.z + ((1.0f / u_axis.order) * u_axis.axis.z);
	/************************************************************************/
	// key control point -> (i, j , k) = (1, 0, 2)
	cp_array[8].x = origin.x + ((1.0f / s_axis.order) * s_axis.axis.x);
	cp_array[8].y = origin.y + ((side_cp_y(gen) / t_axis.order) * t_axis.axis.y);
	cp_array[8].z = origin.z + ((2.0f / u_axis.order) * u_axis.axis.z);
	// key control point -> (i, j , k) = (1, 0, 3)
	cp_array[9].x = origin.x + ((1.0f / s_axis.order) * s_axis.axis.x);
	cp_array[9].y = origin.y + ((side_cp_y(gen) / t_axis.order) * t_axis.axis.y);
	cp_array[9].z = origin.z + ((3.0f / u_axis.order) * u_axis.axis.z);
	/************************************************************************/
	cp_array[10].x = origin.x + ((1.0f / s_axis.order) * s_axis.axis.x);
	cp_array[10].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[10].z = origin.z + ((4.0f / u_axis.order) * u_axis.axis.z);
	cp_array[11].x = origin.x + ((1.0f / s_axis.order) * s_axis.axis.x);
	cp_array[11].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[11].z = origin.z + ((5.0f / u_axis.order) * u_axis.axis.z);

	// (2, 0, 0) <= (i,j,k) <= (2, 0, 5)
	cp_array[12].x = origin.x + ((2.0f / s_axis.order) * s_axis.axis.x);
	cp_array[12].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[12].z = origin.z + ((0.0f / u_axis.order) * u_axis.axis.z);
	cp_array[13].x = origin.x + ((2.0f / s_axis.order) * s_axis.axis.x);
	cp_array[13].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[13].z = origin.z + ((1.0f / u_axis.order) * u_axis.axis.z);
	/************************************************************************/
	// key control point -> (i, j , k) = (2, 0, 2)  
	cp_array[14].x = origin.x + ((2.0f / s_axis.order) * s_axis.axis.x);
	cp_array[14].y = origin.y + ((mid_cp_y(gen) / t_axis.order) * t_axis.axis.y);
	cp_array[14].z = origin.z + ((2.0f / u_axis.order) * u_axis.axis.z);
	// key control point -> (i, j , k) = (2, 0, 3)
	cp_array[15].x = origin.x + ((2.0f / s_axis.order) * s_axis.axis.x);
	cp_array[15].y = origin.y + ((mid_cp_y(gen) / t_axis.order) * t_axis.axis.y);
	cp_array[15].z = origin.z + ((3.0f / u_axis.order) * u_axis.axis.z);
	/************************************************************************/
	cp_array[16].x = origin.x + ((2.0f / s_axis.order) * s_axis.axis.x);
	cp_array[16].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[16].z = origin.z + ((4.0f / u_axis.order) * u_axis.axis.z);
	cp_array[17].x = origin.x + ((2.0f / s_axis.order) * s_axis.axis.x);
	cp_array[17].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[17].z = origin.z + ((5.0f / u_axis.order) * u_axis.axis.z);

	// (3, 0, 0) <= (i,j,k) <= (3, 0, 5)
	cp_array[18].x = origin.x + ((3.0f / s_axis.order) * s_axis.axis.x);
	cp_array[18].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[18].z = origin.z + ((0.0f / u_axis.order) * u_axis.axis.z);
	cp_array[19].x = origin.x + ((3.0f / s_axis.order) * s_axis.axis.x);
	cp_array[19].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[19].z = origin.z + ((1.0f / u_axis.order) * u_axis.axis.z);
	/************************************************************************/
	// key control point -> (i, j , k) = (3, 0, 2)
	cp_array[20].x = origin.x + ((3.0f / s_axis.order) * s_axis.axis.x);
	cp_array[20].y = origin.y + ((mid_cp_y(gen) / t_axis.order) * t_axis.axis.y);
	cp_array[20].z = origin.z + ((2.0f / u_axis.order) * u_axis.axis.z);
	// key control point -> (i, j , k) = (3, 0, 3)
	cp_array[21].x = origin.x + ((3.0f / s_axis.order) * s_axis.axis.x);
	cp_array[21].y = origin.y + ((mid_cp_y(gen) / t_axis.order) * t_axis.axis.y);
	cp_array[21].z = origin.z + ((3.0f / u_axis.order) * u_axis.axis.z);
	/************************************************************************/
	cp_array[22].x = origin.x + ((3.0f / s_axis.order) * s_axis.axis.x);
	cp_array[22].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[22].z = origin.z + ((4.0f / u_axis.order) * u_axis.axis.z);
	cp_array[23].x = origin.x + ((3.0f / s_axis.order) * s_axis.axis.x);
	cp_array[23].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[23].z = origin.z + ((5.0f / u_axis.order) * u_axis.axis.z);

	// (4, 0, 0) <= (i,j,k) <= (4, 0, 5)
	cp_array[24].x = origin.x + ((4.0f / s_axis.order) * s_axis.axis.x);
	cp_array[24].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[24].z = origin.z + ((0.0f / u_axis.order) * u_axis.axis.z);
	cp_array[25].x = origin.x + ((4.0f / s_axis.order) * s_axis.axis.x);
	cp_array[25].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[25].z = origin.z + ((1.0f / u_axis.order) * u_axis.axis.z);
	/************************************************************************/
	// key control point -> (i, j , k) = (4, 0, 2)
	cp_array[26].x = origin.x + ((4.0f / s_axis.order) * s_axis.axis.x);
	cp_array[26].y = origin.y + ((side_cp_y(gen) / t_axis.order) * t_axis.axis.y);
	cp_array[26].z = origin.z + ((2.0f / u_axis.order) * u_axis.axis.z);
	// key control point -> (i, j , k) = (4, 0, 3)
	cp_array[27].x = origin.x + ((4.0f / s_axis.order) * s_axis.axis.x);
	cp_array[27].y = origin.y + ((side_cp_y(gen) / t_axis.order) * t_axis.axis.y);
	cp_array[27].z = origin.z + ((3.0f / u_axis.order) * u_axis.axis.z);
	/************************************************************************/
	cp_array[28].x = origin.x + ((4.0f / s_axis.order) * s_axis.axis.x);
	cp_array[28].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[28].z = origin.z + ((4.0f / u_axis.order) * u_axis.axis.z);
	cp_array[29].x = origin.x + ((4.0f / s_axis.order) * s_axis.axis.x);
	cp_array[29].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[29].z = origin.z + ((5.0f / u_axis.order) * u_axis.axis.z);

	// (5, 0, 0) <= (i,j,k) <= (5, 0, 5)
	cp_array[30].x = origin.x + ((5.0f / s_axis.order) * s_axis.axis.x);
	cp_array[30].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[30].z = origin.z + ((0.0f / u_axis.order) * u_axis.axis.z);
	cp_array[31].x = origin.x + ((5.0f / s_axis.order) * s_axis.axis.x);
	cp_array[31].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[31].z = origin.z + ((1.0f / u_axis.order) * u_axis.axis.z);
	cp_array[32].x = origin.x + ((5.0f / s_axis.order) * s_axis.axis.x);
	cp_array[32].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[32].z = origin.z + ((2.0f / u_axis.order) * u_axis.axis.z);
	cp_array[33].x = origin.x + ((5.0f / s_axis.order) * s_axis.axis.x);
	cp_array[33].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[33].z = origin.z + ((3.0f / u_axis.order) * u_axis.axis.z);
	cp_array[34].x = origin.x + ((5.0f / s_axis.order) * s_axis.axis.x);
	cp_array[34].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[34].z = origin.z + ((4.0f / u_axis.order) * u_axis.axis.z);
	cp_array[35].x = origin.x + ((5.0f / s_axis.order) * s_axis.axis.x);
	cp_array[35].y = origin.y + ((0.0f / t_axis.order) * t_axis.axis.y);
	cp_array[35].z = origin.z + ((5.0f / u_axis.order) * u_axis.axis.z);

	return cp_array;
}

/// <summary>
/// Initiate Weight value for Key control points
/// </summary>
/// <returns> Array of weight value for key control points (x, y, z) </returns>
std::array<std::array<float, 3>, 4> CPipeline::InitWeight(int type)
{
	std::array<std::array<float, 3>, 4> result;
	std::random_device rd;
	std::mt19937_64 gen(rd());

	float max_val = 0.0f;
	float min_val = 0.0f;
	switch (type)
	{
	case 1:	// Open Hole
		max_val = MAX_OPEN_HOLE_WEIGHT;
		min_val = MIN_OPEN_HOLE_WEIGHT;
		break;
	case 2:	// Pipe
		max_val = MAX_PIPE_WEIGHT;
		min_val = MIN_PIPE_WEIGHT;
		break;
	case 3:	// Caved Surface
		max_val = MAX_CAVED_SURFACE_WEIGHT;
		min_val = MIN_CAVED_SURFACE_WEIGHT;
		break;
	}

	std::uniform_real_distribution<float> cp_w(min_val, max_val);
	result = {{
		{cp_w(gen), cp_w(gen), cp_w(gen)},
		{cp_w(gen), cp_w(gen), cp_w(gen)},
		{cp_w(gen), cp_w(gen), cp_w(gen)},
		{cp_w(gen), cp_w(gen), cp_w(gen)}
	}};
	return result;
}

/// <summary>
/// Deformate the grid base mesh using NFFD
/// </summary>
/// <param name="origin"> The origin of FFD space </param>
/// <param name="type"> The type of synthetic defect </param>
/// <param name="cp"> The array of control points </param>
/// <param name="w"> The array of weight value </param>
void CPipeline::DeformGridBase(glm::vec3 const& origin, int type, std::array<glm::vec3, 36> const& cp, std::array<std::array<float, 3>, 4> const& w)
{
	SAxis s_axis;
	TAxis t_axis;
	UAxis u_axis;
	std::vector<Vertex> vertices = m_base->GetVerticesFromModel();
	for (int idx = 0; idx < vertices.size(); idx++)
	{
		// convert a vertex in world coordinates of m_base to local coordinates of lattice space
		float s = ((vertices[idx].Position.x - origin.x) / s_axis.axis.x) * (s_axis.order - s_axis.degree);
		float u = ((vertices[idx].Position.z - origin.z) / u_axis.axis.z) * (u_axis.order - u_axis.degree);

		int cur_cp = 0;
		float s_axis_bp = 0.0f;
		float u_axis_bp = 0.0f;
		glm::vec3 ffd1 = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 ffd2 = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 bottom_ffd1 = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 bottom_ffd2 = glm::vec3(0.0f, 0.0f, 0.0f);

		// change object point of grid base mesh usfing NFFD
		for (int i = 0; i < s_axis.order; i++)
		{
			ffd2 = glm::vec3(0.0f, 0.0f, 0.0f);
			bottom_ffd2 = glm::vec3(0.0f, 0.0f, 0.0f);
			for (int k = 0; k < u_axis.order; k++)
			{
				switch (type)
				{
				case 1:	// Open Hole
					if (i == 2 && k == 2)
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * w[0][0] * cp[cur_cp].x;
						ffd2.y += u_axis_bp * w[0][1] * cp[cur_cp].y;
						ffd2.z += u_axis_bp * w[0][2] * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp * w[0][0];
						bottom_ffd2.y += u_axis_bp * w[0][1];
						bottom_ffd2.z += u_axis_bp * w[0][2];
					}
					else if (i == 2 && k == 3)
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * w[1][0] * cp[cur_cp].x;
						ffd2.y += u_axis_bp * w[1][1] * cp[cur_cp].y;
						ffd2.z += u_axis_bp * w[1][2] * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp * w[1][0];
						bottom_ffd2.y += u_axis_bp * w[1][1];
						bottom_ffd2.z += u_axis_bp * w[1][2];
					}
					else if (i == 3 && k == 2)
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * w[2][0] * cp[cur_cp].x;
						ffd2.y += u_axis_bp * w[2][1] * cp[cur_cp].y;
						ffd2.z += u_axis_bp * w[2][2] * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp * w[2][0];
						bottom_ffd2.y += u_axis_bp * w[2][1];
						bottom_ffd2.z += u_axis_bp * w[2][2];
					}
					else if (i == 3 && k == 3)
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * w[3][0] * cp[cur_cp].x;
						ffd2.y += u_axis_bp * w[3][1] * cp[cur_cp].y;
						ffd2.z += u_axis_bp * w[3][2] * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp * w[3][0];
						bottom_ffd2.y += u_axis_bp * w[3][1];
						bottom_ffd2.z += u_axis_bp * w[3][2];
					}
					else
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * cp[cur_cp].x;
						ffd2.y += u_axis_bp * cp[cur_cp].y;
						ffd2.z += u_axis_bp * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp;
						bottom_ffd2.y += u_axis_bp;
						bottom_ffd2.z += u_axis_bp;
					}
					break;

				case 2:	// Pipe
					if (i == 2 && k == 2)
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * w[0][0] * cp[cur_cp].x;
						ffd2.y += u_axis_bp * w[0][1] * cp[cur_cp].y;
						ffd2.z += u_axis_bp * w[0][2] * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp * w[0][0];
						bottom_ffd2.y += u_axis_bp * w[0][1];
						bottom_ffd2.z += u_axis_bp * w[0][2];
					}
					else if (i == 2 && k == 3)
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * w[1][0] * cp[cur_cp].x;
						ffd2.y += u_axis_bp * w[1][1] * cp[cur_cp].y;
						ffd2.z += u_axis_bp * w[1][2] * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp * w[1][0];
						bottom_ffd2.y += u_axis_bp * w[1][1];
						bottom_ffd2.z += u_axis_bp * w[1][2];
					}
					else if (i == 3 && k == 2)
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * w[2][0] * cp[cur_cp].x;
						ffd2.y += u_axis_bp * w[2][1] * cp[cur_cp].y;
						ffd2.z += u_axis_bp * w[2][2] * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp * w[2][0];
						bottom_ffd2.y += u_axis_bp * w[2][1];
						bottom_ffd2.z += u_axis_bp * w[2][2];
					}
					else if (i == 3 && k == 3)
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * w[3][0] * cp[cur_cp].x;
						ffd2.y += u_axis_bp * w[3][1] * cp[cur_cp].y;
						ffd2.z += u_axis_bp * w[3][2] * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp * w[3][0];
						bottom_ffd2.y += u_axis_bp * w[3][1];
						bottom_ffd2.z += u_axis_bp * w[3][2];
					}
					else
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * cp[cur_cp].x;
						ffd2.y += u_axis_bp * cp[cur_cp].y;
						ffd2.z += u_axis_bp * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp;
						bottom_ffd2.y += u_axis_bp;
						bottom_ffd2.z += u_axis_bp;
					}
					break;

				case 3:	// Caved Surface
					if (i == 1 && k == 2)
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * w[0][0] * cp[cur_cp].x;
						ffd2.y += u_axis_bp * w[0][1] * cp[cur_cp].y;
						ffd2.z += u_axis_bp * w[0][2] * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp * w[0][0];
						bottom_ffd2.y += u_axis_bp * w[0][1];
						bottom_ffd2.z += u_axis_bp * w[0][2];
					}
					else if (i == 1 && k == 3)
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * w[1][0] * cp[cur_cp].x;
						ffd2.y += u_axis_bp * w[1][1] * cp[cur_cp].y;
						ffd2.z += u_axis_bp * w[1][2] * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp * w[1][0];
						bottom_ffd2.y += u_axis_bp * w[1][1];
						bottom_ffd2.z += u_axis_bp * w[1][2];
					}
					else if (i == 4 && k == 2)
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * w[2][0] * cp[cur_cp].x;
						ffd2.y += u_axis_bp * w[2][1] * cp[cur_cp].y;
						ffd2.z += u_axis_bp * w[2][2] * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp * w[2][0];
						bottom_ffd2.y += u_axis_bp * w[2][1];
						bottom_ffd2.z += u_axis_bp * w[2][2];
					}
					else if (i == 4 && k == 3)
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * w[3][0] * cp[cur_cp].x;
						ffd2.y += u_axis_bp * w[3][1] * cp[cur_cp].y;
						ffd2.z += u_axis_bp * w[3][2] * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp * w[3][0];
						bottom_ffd2.y += u_axis_bp * w[3][1];
						bottom_ffd2.z += u_axis_bp * w[3][2];
					}
					else
					{
						u_axis_bp = Bernstein(k, u_axis.degree, u, W, u_axis.order);
						ffd2.x += u_axis_bp * cp[cur_cp].x;
						ffd2.y += u_axis_bp * cp[cur_cp].y;
						ffd2.z += u_axis_bp * cp[cur_cp].z;
						bottom_ffd2.x += u_axis_bp;
						bottom_ffd2.y += u_axis_bp;
						bottom_ffd2.z += u_axis_bp;
					}
					break;
				}
				cur_cp++;
			}
			u_axis_bp = Bernstein(i, s_axis.degree, s, U, s_axis.order);
			ffd1.x += u_axis_bp * ffd2.x;
			ffd1.y += u_axis_bp * ffd2.y;
			ffd1.z += u_axis_bp * ffd2.z;
			bottom_ffd1.x += (u_axis_bp * bottom_ffd2.x);
			bottom_ffd1.y += (u_axis_bp * bottom_ffd2.y);
			bottom_ffd1.z += (u_axis_bp * bottom_ffd2.z);
		}
		if (bottom_ffd1 != glm::vec3(0.0f, 0.0f, 0.0f))
			vertices[idx].Position = ffd1 / bottom_ffd1;
	}

	// Update deformed points
	m_base->UpdateModel(vertices);
	m_baseVertices = m_base->GetVerticesFromModel();
}

/// <summary>
/// Calculate B-Splie basis function
/// </summary>
/// <param name="i"> Index </param>
/// <param name="m"> Degree </param>
/// <param name="t"> Point of grid base mesh in FFD box coordinate system (s, t, u) </param>
/// <param name="uv"> Unit vector </param>
/// <param name="n"> Order </param>
/// <param name="k"> Total Degree </param>
/// <returns> The value of B-Spline basis function </returns>
float CPipeline::Bernstein(int i, int m, float t, std::array<float, 10> const& nv, int n)
{
	// base condition
	if (m == 0)
	{
		if ((nv[i] <= t && t < nv[(long long)i + 1]) && (0 <= t && t < 2.5f))
			return 1.0f;
		else
			return 0.0f;
	}

	if ((nv[i] != nv[(long long)i + m]) && (nv[(long long)i + 1] != nv[(long long)i + (long long)m + 1]))
		return ((t - nv[i]) / (nv[(long long)i + (long long)m] - nv[i])) * Bernstein(i, m - 1, t, nv, n) +
		((nv[(long long)i + (long long)m + 1] - t) / (nv[(long long)i + (long long)m + 1] - nv[(long long)i + 1])) * Bernstein(i + 1, m - 1, t, nv, n);
	else
		return 0.0f;
}

/// <summary>
/// Locate the grid base mesh to the face that selected randomly among faces user picks
/// </summary>
void CPipeline::DoPositioning(glm::mat4 const& pm)
{
	//std::random_device rd;
	//std::mt19937_64 gen(rd());
	//std::uniform_int_distribution<int> dis(0, (int)(m_pickIndices.size() - 1));

	//// select faces that user picks randomly
	//aiFace random_face = m_modelFaces[m_pickIndices.at(dis(gen))];
	//glm::mat4 position_matrix = CalculatePositionMatrix(glm::mat4(1.0f), random_face);

	// multiply pisition matrix for each vertex of grid base mesh
	for (int i = 0; i < m_baseVertices.size(); i++)
	{
		m_baseVertices[i].Position = pm * glm::vec4(m_baseVertices[i].Position, 1.0);
		m_base->UpdateModel(m_baseVertices);
	}
	m_baseVertices = m_base->GetVerticesFromModel();
}

/// <summary>
/// Scale the grid base mesh that is deformed among values user inputs
/// </summary>
void CPipeline::DoScaling()
{
	glm::mat4 scale_matrix = CalculateScaleMatrix(glm::mat4(1.0f));
	for (int i = 0; i < m_baseVertices.size(); i++)
	{
		m_baseVertices[i].Position = scale_matrix * glm::vec4(m_baseVertices[i].Position, 1.0);
		m_base->UpdateModel(m_baseVertices);
	}
	m_baseVertices = m_base->GetVerticesFromModel();
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

/// <summary>
/// 
/// </summary>
void CPipeline::DoModeling()
{
}

CPipeline::~CPipeline()
{
}
