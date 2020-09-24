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
/// 
/// </summary>
void CPipeline::Execute()
{
}

CPipeline::~CPipeline()
{
}
