
// Camera.cpp for calculate the corresponding Vectors and Matrics for use in OpenGL

#include "pch.h"
#include "Camera.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

CCamera::CCamera()
{
}


/// <summary>
/// Calculate rotation of the camera on the virtual trackball
/// </summary>
/// <param name="prev">: the previous mouse position </param>
/// <param name="cur">: the current mouse position </param>
void CCamera::Rotate(glm::vec2 prev, glm::vec2 cur)
{
	// calculate the prev, cur coordinates on the virtual trackball
	glm::vec3 prev_trackball = ProjectTrackBall(prev);
	glm::vec3 cur_trackball = ProjectTrackBall(cur);
	/*std::cout << glm::to_string(prev) << std::endl;
	std::cout << glm::to_string(cur) << std::endl;*/

	// calculate rotation axis

}


/// <summary>
/// Calculate the coordinates of mouse point projected to the virtual trackball
/// </summary>
/// <param name="point">: the mouse point for transforming Normalized Device Coordinates </param>
/// <returns> the NDC of mouse point </returns>
glm::vec3 CCamera::ProjectTrackBall(glm::vec2 point)
{
	float z = 0.0f;
	double radius = 1.0;
	double distance = sqrt(pow(point.x, 2.0) + pow(point.y, 2.0));
	// check the mouse point is inside the virtua trackball or not
	if (distance < (radius / sqrt(2.0)))	// inside the sphere(virtual trackball)
	{
		z = (float)sqrt(pow(radius, 2.0) - pow(distance, 2.0));
	}
	else									// outside the sphere(virtual trackball) - on hyperbola
	{
		z = (float)pow((radius / sqrt(2.0)), 2.0) / distance;
	}
	return glm::vec3(point.x, point.y, z);
}

/// <summary>
/// Return the Model-View Matrix calculated using the LookAt Matrix
/// </summary>
/// <returns> 4 x 4 Model-View Matrix </returns>
glm::mat4 CCamera::GetViewMatrix()
{
	// lookAt - Translate whole scene(camera + object) from the eye position to the origin(0, 0, 0)
	//          Rotate for facing to the -z axis
	return glm::lookAt(m_Position, m_Target, m_UpDirection);
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
	return m_RightAxis;
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
