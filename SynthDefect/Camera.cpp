
// Camera.cpp for calculate the corresponding Vectors and Matrics for use in OpenGL

#include "pch.h"
#include "Camera.h"

CCamera::CCamera()
{
}


void CCamera::CalculateZoom(short zDelta)
{
	if (zDelta < 0)
	{
		if (m_Zoom >= MAX_ZOOM)
			m_Zoom = MAX_ZOOM;
		else
			m_Zoom += ZOOM_OFFSET;
	}
	else
	{
		if (m_Zoom <= MIN_ZOOM)
			m_Zoom = MIN_ZOOM;
		else
			m_Zoom -= ZOOM_OFFSET;
	}
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


/// <summary>
/// Retrieve the constant value of Field of View
/// </summary>
/// <returns> the value of FOV </returns>
const float CCamera::GetFOV()
{
	return FOV;
}

void CCamera::SetZoom(float value)
{
	m_Zoom = value;
}

float CCamera::GetZoom()
{
	return m_Zoom;
}

void CCamera::SetPitch(float value)
{
	m_Pitch = value;
}

float CCamera::GetPitch()
{
	return m_Pitch;
}

void CCamera::SetYaw(float value)
{
	m_Yaw = value;
}

float CCamera::GetYaw()
{
	return m_Yaw;
}


/// <summary>
/// Retrieve the constant value of Sensitivity of Camera
/// </summary>
/// <returns> the value of SENSITIVITY </returns>
const float CCamera::GetSensitivity()
{
	return SENSITIVITY;
}


/// <summary>
/// Retrieve the constant value of Speed of Camera
/// </summary>
/// <returns> the value of SPEED </returns>
const float CCamera::GetSpeed()
{
	return SPEED;
}

CCamera::~CCamera()
{
}
