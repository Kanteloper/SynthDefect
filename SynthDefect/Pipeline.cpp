#include "pch.h"
#include "Pipeline.h"

#include <iostream>

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
/// 
/// </summary>
void CPipeline::DoPositioning()
{

}

CPipeline::~CPipeline()
{
}
