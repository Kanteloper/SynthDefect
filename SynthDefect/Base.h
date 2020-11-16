
// Base.h : Interface of CBase class

#pragma once

#include "Model.h"

class CBase : public CModel
{
	// Constructor
public:
	CBase(std::string const& filePath);

	// Attributes
private:
	std::string path;

	// Implements
public:
	~CBase();

private:
	void LoadBase(std::string const& pathName);


};

