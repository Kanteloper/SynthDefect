
// Base.h : Interface of CBase class

#pragma once

#include "Model.h"

class CBase : public CModel
{
	// Constructor
public:
	CBase(std::string const& filePath);

	// Attributes

	// Implements
public:
	~CBase();

private:
	void LoadBase(std::string const& pathName);


};

