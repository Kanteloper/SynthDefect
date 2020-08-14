
// Camera.cpp for calculate the corresponding Vectors and Matrics for use in OpenGL

#include "pch.h"
#include "Camera.h"

CCamera::CCamera()
{
}


/// <summary>
/// Calculate the forward vector(direction vector) from the Camera's (updated) Euler Angles
/// </summary>
void CCamera::UpdateCameraVectors()
{
	// calculate the new Forward vector
	glm::vec3 forward;
	forward.x = cos(glm::radians(YAW)) * cos(glm::radians(PITCH));
	forward.y = sin(glm::radians(PITCH));
	forward.z = sin(glm::radians(YAW)) * cos(glm::radians(PITCH));
	m_ForwardAxis = glm::normalize(forward);
	// re-calculate the Left and Up vector
	m_LeftAxis = glm::normalize(glm::cross(m_ForwardAxis, m_UpDirection));
	m_UpAxis = glm::cross(m_LeftAxis, m_ForwardAxis);
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


/// <summary>
/// Set camera position
/// </summary>
/// <param name="pos">: camera position that want to set </param>
void CCamera::SetPosition(glm::vec3 pos)
{
	m_Position = pos;
}


/// <summary>
/// Retrieve camera position
/// </summary>
/// <returns> currently set the camera position vector </returns>
glm::vec3 CCamera::GetPosition()
{
	return m_Position;
}


/// <summary>
/// Retrieve the constant value of Field of View
/// </summary>
/// <returns> the value of FOV </returns>
const float CCamera::GetFOV()
{
	return FOV;
}


/// <summary>
/// Retrieve the constant value of Sensitivity of Camera
/// </summary>
/// <returns> the value of SENSITIVITY </returns>
const float CCamera::GetSensitivity()
{
	return SENSITIVITY;
}

CCamera::~CCamera()
{
}
