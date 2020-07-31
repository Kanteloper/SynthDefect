
// Shader.h : Interface of Shader class

#pragma once



class CShader
{
	// Constructor
public:
	CShader();
	CShader(const char* vertexPath, const char* fragmentPath);

	// Attributes
private:
	const char* vShaderCode;
	const char* fShaderCode;

public:
	unsigned int ID;			// the program ID

	// Implement
public:
	~CShader();
};

