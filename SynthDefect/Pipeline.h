
// Pipeline.h : Interface of CPipelice class

#pragma once

#include <vector>
#include <array>
#include "Model.h"
#include "Base.h"
#include "CGALLib.h"

class CPipeline
{
	// Constructor
public:
	CPipeline();
	CPipeline(Properties const& props, std::vector<int> const& indices, CModel* model, CBase* base)
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
	CBase* m_base;
	std::vector<aiFace> m_modelFaces;
	std::vector<Vertex> m_modelVertices;
	std::vector<aiFace> m_baseFaces;
	std::vector<Vertex> m_baseVertices;

	// implements
private:
	glm::mat4 CalculatePositionMatrix(glm::mat4 const& m, aiFace const& f);
	glm::vec3 CalculateTriangleCentroid(glm::vec3 const& A, glm::vec3 const& B, glm::vec3 const& C);
	void DoDeforming();
	std::array<glm::vec3, 36> InitControlPoints(glm::vec3 const& origin, int type);
	std::array<std::array<float, 3>, 4> InitWeight(int type);
	void DeformGridBase(glm::vec3 const& origin, int type, std::array<glm::vec3, 36> const& cp, std::array<std::array<float, 3>, 4> const& w);
	float Bernstein(int i, int m, float t, std::array<float, 10> const& nv, int n);
	void DoPositioning(glm::mat4 const& pm);
	void DoScaling();
	glm::mat4 CalculateScaleMatrix(glm::mat4 const& m);
	void DoModeling();
	Polyhedron InitPolyhedron(std::vector<Vertex> const& vertices, std::vector<aiFace> const& facets);
	void DoSampling();

public:
	void Execute();

	~CPipeline();
};
