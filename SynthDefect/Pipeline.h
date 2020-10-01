
// Pipeline.h : Interface of Pipelice class

#pragma once

#include <vector>
#include "Model.h"

class CPipeline
{
	// Constructor
public:
	CPipeline();
	CPipeline(Properties const& props, std::vector<int> const& indices, CModel* model, CModel* base)
		: m_pickIndices(indices), m_model(model), m_base(base)
	{
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
	void DoPositioning();
	glm::mat4 CalculatePositionMatrix(glm::mat4 const& m, aiFace const& f);
	glm::vec3 CalculateTriangleCentroid(glm::vec3 const& A, glm::vec3 const& B, glm::vec3 const& C);
	void DoDeforming();
	void DoScaling();
	glm::mat4 CalculateScaleMatrix(glm::mat4 const& m);
	void DoModeling();
	void DoSampling();

public:
	void Execute();

	~CPipeline();
};

