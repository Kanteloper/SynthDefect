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
	m_vShaderCode = RetrieveCode(vertexPath);
	m_fShaderCode = RetrieveCode(vertexPath);
	CompileShaderCode();
	CreateShaderProgram();
}


/// <summary>
/// Compile the vertex and fragment shader code
/// </summary>
void CShader::CompileShaderCode()
{

}


/// <summary>
/// Link the vertex and fragment shader
/// </summary>
void CShader::CreateShaderProgram()
{
}

