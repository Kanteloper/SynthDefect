
// Shader.cpp for Shader program of OpenGL pipeline

#include "pch.h"
#include "Shader.h"
#include <gl/glew.h>
#include <gl/GLU.h>


CShader::CShader(const char* vertexPath, const char* fragmentPath = nullptr)
{
	// retrieve the vertex/fragment shader code
	m_vShaderCode = RetrieveCode(vertexPath);
	m_fShaderCode = RetrieveCode(fragmentPath);
	CompileShaderCode();
	CreateShaderProgram();
	DeleteShaders();
}


/// <summary>
/// Compile the vertex and fragment shader code
/// </summary>
void CShader::CompileShaderCode()
{
	const char* vCode = m_vShaderCode.c_str();
	const char* fCode = m_fShaderCode.c_str();
	// vertex shader
	// A shader object is used to maintain the source code strings that define a shader
	m_vertexShader = glCreateShader(GL_VERTEX_SHADER);			// create an empty shader object
	glShaderSource(m_vertexShader, 1, &vCode, NULL);	// set the source code in shader object
	glCompileShader(m_vertexShader);							// compile the shader object
	CheckCompileErrors(m_vertexShader, "VERTEX");

	// fragment shader
	m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_fragmentShader, 1, &fCode, NULL);
	glCompileShader(m_fragmentShader);
	CheckCompileErrors(m_fragmentShader, "FRAGMENT");
}


/// <summary>
/// Link the vertex and fragment shader
/// </summary>
void CShader::CreateShaderProgram()
{
	// A program object is an object to which shader objects can be attached
	ID = glCreateProgram();										// create an empty program object
	glAttachShader(ID, m_vertexShader);							// attach a shader object to a program object
	glAttachShader(ID, m_fragmentShader);
	glLinkProgram(ID);											// link a program object
	CheckCompileErrors(ID, "PROGRAM");
}


/// <summary>
/// Check shader/program compilation/linkage errors
/// </summary>
/// <param name="shader">: shader/program object with shader code</param>
/// <param name="type">: a type of shader or program</param>
void CShader::CheckCompileErrors(GLuint object, std::string type)
{
	GLint success, logSize;
	GLchar infoLog[1024];
	if (type != "PROGRAM") 
	{
		// The information log for a shader object is modified when the shader is compiled.
		glGetShaderInfoLog(object, 1024, NULL, infoLog);		// return the information log for a shader object
		glGetShaderiv(object, GL_INFO_LOG_LENGTH, &logSize);
		if (logSize != 0)
			TRACE("ERROR::SHADER_COMPILATION_ERROR of %s \n %s", type.c_str(), infoLog);
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);	// return the information log for a program object
			TRACE("ERROR::PROGRAM_LINKING_ERROR of %s \n %s", type.c_str(), infoLog);
		}
	}
}


/// <summary>
/// Delete the shader objects
/// </summary>
void CShader::DeleteShaders()
{
	glDeleteShader(m_vertexShader);
	glDeleteShader(m_fragmentShader);
}


/// <summary>
/// Activate the shader objects
/// </summary>
void CShader::Use()
{
	glUseProgram(ID);
}


/// <summary>
/// Specify the value of a uniform variable of boolean type for the current program object
/// </summary>
/// <param name="name">: the name of uniform variable whose location is to be queried </param>
/// <param name="value">: the value of uniform variable to set </param>
void CShader::SetBool(const std::string& name, BOOL value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}


/// <summary>
/// Specify the value of a uniform variable of integer type for the current program object
/// </summary>
/// <param name="name">: the name of uniform variable whose location is to be queried </param>
/// <param name="value">: the value of uniform variable to set </param>
void CShader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}


/// <summary>
/// Specify the value of a uniform variable of float type for the current program object
/// </summary>
/// <param name="name">: the name of uniform variable whose location is to be queried </param>
/// <param name="value">: the value of uniform variable to set </param>
void CShader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}


/// <summary>
/// Specify the value of a uniform variable of vec2 type for the current program object
/// </summary>
/// <param name="name">: the name of uniform variable whose location is to be queried </param>
/// <param name="value">: the value of uniform variable to set </param>
void CShader::SetVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}


/// <summary>
/// Specify the value of a uniform variable of vec2 type for the current program object
/// </summary>
/// <param name="name">: the name of uniform variable for the current program object </param>
/// <param name="x">: X coordinate </param>
/// <param name="y">: Y coordinate </param>
void CShader::SetVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}


/// <summary>
/// Specify the value of a uniform variable of vec3 type for the current program object
/// </summary>
/// <param name="name">: the name of uniform variable whose location is to be queried </param>
/// <param name="value">: the value of uniform variable to set </param>
void CShader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}


/// <summary>
/// Specify the value of a uniform variable of vec3 type for the current program object
/// </summary>
/// <param name="name">: the name of uniform variable whose location is to be queried </param>
/// <param name="x"> X coordinate </param>
/// <param name="y"> Y coordinate </param>
/// <param name="z"> Z coordiante </param>
void CShader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}


/// <summary>
/// Specify the value of a uniform variable of vec4 type for the current program object
/// </summary>
/// <param name="name">: the name of uniform variable whose location is to be queried </param>
/// <param name="value">: the value of uniform variable to set </param>
void CShader::SetVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}


/// <summary>
/// Specify the value of a uniform variable of vec4 type for the current program object
/// </summary>
/// <param name="name"></param>
/// <param name="x">: X coordinate </param>
/// <param name="y">: Y coordinate </param>
/// <param name="z">: Z coordiante </param>
/// <param name="w">: direction </param>
void CShader::SetVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}


/// <summary>
/// Specify the value of a uniform variable of mat2 type for the current program object
/// </summary>
/// <param name="name">: the name of uniform variable whose location is to be queried </param>
/// <param name="mat">: the value of uniform variable to set </param>
void CShader::SetMat2(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


/// <summary>
/// Specify the value of a uniform variable of mat3 type for the current program object
/// </summary>
/// <param name="name">: the name of uniform variable whose location is to be queried </param>
/// <param name="mat">: the value of uniform variable to set </param>
void CShader::SetMat3(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


/// <summary>
/// Specify the value of a uniform variable of mat4 type for the current program object
/// </summary>
/// <param name="name">: the name of uniform variable whose location is to be queried </param>
/// <param name="mat">: the value of uniform variable to set </param>
void CShader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}


CShader::~CShader()
{
}