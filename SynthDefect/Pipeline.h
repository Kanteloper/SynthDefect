
// Pipeline.h : Interface of Pipelice class

#pragma once

#include <vector>
#include <array>
#include "Model.h"

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

class CPipeline
{
	// Constructor
public:
	CPipeline();
	CPipeline(Properties const& props, std::vector<int> const& indices, CModel* model, CModel* base)
		: m_pickIndices(indices), m_model(model), m_base(base)
	{
		m_type = props.type;
		m_width = props.width;
		m_height = props.height;
		m_depth = props.depth;
		m_modelFaces = m_model->GetFacesFromModel();
		m_baseFaces = m_base->GetFacesFromModel();
		m_modelVertices = m_model->GetVerticesFromModel();
		m_baseVertices = m_base->GetVerticesFromModel();
	}
	
	// Attributes
private:
	int m_type;
	float m_width;
	float m_height;
	float m_depth;
	std::vector<int> m_pickIndices;
	CModel* m_model;
	CModel* m_base;
	std::vector<aiFace> m_modelFaces;
	std::vector<Vertex> m_modelVertices;
	std::vector<aiFace> m_baseFaces;
	std::vector<Vertex> m_baseVertices;

	// implements
private:
	void DoDeforming();
	std::array<glm::vec3, 36> InitControlPoints(glm::vec3 const& origin, int type);
	void DoPositioning(glm::mat4 const& pm);
	glm::mat4 CalculatePositionMatrix(glm::mat4 const& m, aiFace const& f);
	glm::vec3 CalculateTriangleCentroid(glm::vec3 const& A, glm::vec3 const& B, glm::vec3 const& C);
	void DoScaling();
	glm::mat4 CalculateScaleMatrix(glm::mat4 const& m);
	void DoModeling();
	void DoSampling();

public:
	void Execute();

	~CPipeline();
};

