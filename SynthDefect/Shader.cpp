#include "pch.h"
#include "Shader.h"

CShader::CShader()
{
}
CShader::~CShader()
{
}

CShader::CShader(const char* vertexPath, const char* fragmentPath = nullptr)
{
	// retrieve the vertex/fragment shader code
	std::string vString;
	std::string fString;
	vShaderCode = vString.c_str();
	fShaderCode = fString.c_str();
}
