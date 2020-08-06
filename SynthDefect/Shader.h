
// Shader.h : Interface of Shader class

#pragma once

class CShader
{
	// Constructor
public:
	CShader() : ID(0), m_vShaderCode(""), m_fShaderCode(""), m_vertexShader(0), m_fragmentShader(0) {};
	CShader(const char* vertexPath, const char* fragmentPath);

	// Attributes
private:
	std::string m_vShaderCode;
	std::string m_fShaderCode;
	unsigned int m_vertexShader;
	unsigned int m_fragmentShader;

public:
	unsigned int ID;			// the program ID

	// Implement
private:
	void CompileShaderCode();
	void CreateShaderProgram();
	void DeleteShaders();
	void CheckCompileErrors(GLuint object, std::string type);

public:
	void Use();
public:
	~CShader();
};

