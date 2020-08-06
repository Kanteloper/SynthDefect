
// Shader.h : Interface of Shader class

#pragma once

#include <glm/glm.hpp>

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
	// utility uniform functions
	void SetBool(const std::string& name, BOOL value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, const glm::vec2 &value) const;
	void SetVec2(const std::string& name, float x, float y) const;
	void SetVec3(const std::string& name, const glm::vec3 &value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;
	void SetVec4(const std::string& name, const glm::vec4 &value) const;
	void SetVec4(const std::string& name, float x, float y, float z, float w) const;
	void SetMat2(const std::string& name, const glm::mat2& mat) const;
	void SetMat3(const std::string& name, const glm::mat3& mat) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;
public:
	~CShader();
};

