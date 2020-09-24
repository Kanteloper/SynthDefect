
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
		: m_numOfDefects(props.numOfDefects), m_pickIndices(indices), m_model(model), m_base(base) 
	{
		m_faces = m_model->GetFacesFromModel();
		m_vertices = m_model->GetVerticesFromModel();
	}
	
	// Attributes
private:
	int m_numOfDefects;
	std::vector<int> m_pickIndices;
	CModel* m_model;
	CModel* m_base;
	std::vector<aiFace> m_faces;
	std::vector<Vertex> m_vertices;


	// implements
public:
	void Execute();
	void DoPositioning();
	glm::vec3 GetTriangleCentroid(aiFace const& f);
	void DoScaling();
	void DoDeforming();
	void DoModeling();
	void DoSampling();

	~CPipeline();
};

