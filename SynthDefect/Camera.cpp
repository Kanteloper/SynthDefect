
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


/// <summary>
/// Retrieve camera position
/// </summary>
/// <returns> currently set the camera position vector </returns>
glm::vec3 CCamera::GetPosition()
{
	return m_Position;
}


glm::vec3 CCamera::GetForwardAxis()
{
	return m_ForwardAxis;
}


glm::vec3 CCamera::GetUpAxis()
{
	return m_UpAxis;
}


glm::vec3 CCamera::GetLeftAxis()
{
	return m_LeftAxis;
}


void CCamera::SetZoom(float value)
{
	m_Zoom = value;
}


float CCamera::GetZoom()
{
	return m_Zoom;
}


CCamera::~CCamera()
{
}
