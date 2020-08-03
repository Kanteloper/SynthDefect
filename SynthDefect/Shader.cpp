#include "pch.h"
#include "Shader.h"
#include <gl/glew.h>
#include <gl/GLU.h>

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

