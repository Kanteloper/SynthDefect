#pragma once

// Model.h : interface of Model class

#include <vector>
#include <string>

class Model
{
	// Constructor
public:
	Model(std::string const& path);

	// Attributes
public:
	// model data
	std::vector<Mesh>	meshes;

	// Impelementation
public:
	~Model();

private:
	BOOL LoadModel(LPCTSTR pathName);



};

