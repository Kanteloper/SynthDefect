
// Pipeline.h : Interface of Pipelice class

#pragma once

#include <vector>
#include "Model.h"

class CPipeline
{
	// Constructor
public:
	CPipeline();
	CPipeline(Properties const& props, std::vector<int> const& faces, CModel* model, CModel* base)
		: m_numOfDefects(props.numOfDefects), m_faces(faces), m_model(model), m_base(base) {}
	
	// Attributes
private:
	int m_numOfDefects;
	std::vector<int> m_faces;
	CModel* m_model;
	CModel* m_base;

	// implements
public:
	void Execute();
	~CPipeline();
};

