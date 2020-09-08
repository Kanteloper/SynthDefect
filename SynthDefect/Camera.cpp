
// Camera.cpp for calculate the corresponding Vectors and Matrics for use in OpenGL

#include "pch.h"
#include "Camera.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

CCamera::CCamera()
{
}


CCamera::CCamera(glm::vec3 eye, glm::vec3 target)
{
	m_zoom = FOV;
	m_position = eye;
	m_target = target;
	m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	SetCameraVectors(m_position, m_target);
	SetLightVectors();
}


/// <summary>
/// Set the Camera vectors
/// </summary>
/// <param name="eye">: the position of camera </param>
/// <param name="target">: the position of centroid of the target </param>
void CCamera::SetCameraVectors(glm::vec3 const& eye, glm::vec3 const& target)
{
 	m_forwardAxis = glm::normalize(eye - target);
	m_rightAxis = glm::normalize(glm::cross(m_worldUp, m_forwardAxis));
	m_upAxis = glm::cross(m_forwardAxis, m_rightAxis);
}


/// <summary>
/// Set the light vectors
/// </summary>
void CCamera::SetLightVectors()
{
	m_lightColor = glm::vec3(LIGHT_R, LIGHT_G, LIGHT_B);
	m_lightPosition = glm::vec3(LIGHT_X, LIGHT_Y, LIGHT_Z);
}



/// <summary>
/// Return the Model-View Matrix calculated using the LookAt Matrix
/// </summary>
/// <returns> 4 x 4 Model-View Matrix </returns>
glm::mat4 CCamera::GetViewMatrix() const
{
	return glm::lookAt(m_position, m_target, m_worldUp);
}


/// <summary>
/// Retrieve the position of camera
/// </summary>
/// <returns> XYZ coordinate for the camera position </returns>
glm::vec3 CCamera::GetPosition() const
{
	return m_position;
}


/// <summary>
/// Set the Field Of View for camera
/// </summary>
/// <param name="value"> the FOV which users define </param>
void CCamera::SetZoom(float const& value)
{
	m_zoom = value;
}


/// <summary>
/// Retrieve the Field of View for camera
/// </summary>
/// <returns> FOV of the camera </returns>
float CCamera::GetZoom() const
{
	return m_zoom;
}


/// <summary>
/// Retrieve the Color of light source
/// </summary>
/// <returns> the RGB value for color of the light source </returns>
glm::vec3 CCamera::GetLightColor() const
{
	return m_lightColor;
}


/// <summary>
/// Retrieve the position of light source
/// </summary>
/// <returns> XYZ coordinates for the light source position </returns>
glm::vec3 CCamera::GetLightPosition() const
{
	return m_lightPosition;
}


CCamera::~CCamera()
{
}
