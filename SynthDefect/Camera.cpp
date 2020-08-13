
// Camera.cpp for calculate the corresponding Vectors and Matrics for use in OpenGL

#include "pch.h"
#include "Camera.h"

CCamera::CCamera()
{
}

/// <summary>
/// Return the Model-View Matrix calculated using the LookAt Matrix
/// </summary>
/// <returns> 4 x 4 Model-View Matrix </returns>
glm::mat4 CCamera::GetViewMatrix()
{
	// lookAt - Translate whole scene(camera + object) from the eye position to the origin(0, 0, 0)
	//          Rotate for facing to the -z axis
	return glm::lookAt(m_Position, m_ForwardAxis, m_UpAxis);
}

void CCamera::SetPosition(glm::vec3 pos)
{
	m_Position = pos;
}


/// <summary>
/// Return the constant value of Field of View
/// </summary>
/// <returns> the value of FOV </returns>
const float CCamera::GetFOV()
{
	return FOV;
}

const float CCamera::GetSensitivity()
{
	return SENSITIVITY;
}

CCamera::~CCamera()
{
}
