
// Shader.h : Interface of Shader class

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class CShader
{
	// Constructor
public:
	CShader(const char* vertexPath, const char* fragmentPath);

	// Attributes
public:
	unsigned int ID;			// the program ID

	// Implement
public:
	~CShader();

};

