
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
void CCamera::Rotate(glm::vec2 const& prev, glm::vec2 const& cur)
{
	// calculate the prev, cur coordinates using virtual trackball algorithm
	glm::vec3 prev_trackball = ProjectTrackBall(prev);
	glm::vec3 cur_trackball = ProjectTrackBall(cur);

	// calculate the axis and angle of rotation for camera
	glm::vec3 rotation_axis = CalculateAxis(prev_trackball, cur_trackball);
	float rotation_angle = CalculateAngle(prev_trackball, cur_trackball);

	// calculate rotation orientation
	RefreshQuaternion();
	glm::quat change = glm::angleAxis(-rotation_angle * SENSITIVITY, rotation_axis);
	glm::quat transform = m_Orientation * change;
	m_Orientation = change;
	Update(transform);
}


/// <summary>
/// Calculate the coordinates of mouse point projected to the virtual trackball
/// </summary>
/// <param name="point">: the mouse point for transforming Normalized Device Coordinates </param>
/// <returns> the NDC of mouse point </returns>
glm::vec3 CCamera::ProjectTrackBall(glm::vec2 const& point)
{
	double z = 0.0f;
	double distance = sqrt(pow(point.x, 2.0) + pow(point.y, 2.0));
	// check the mouse point is inside the virtua trackball or not
	if (distance < (TRACKBALL_RADIUS / sqrt(2.0)))	// inside the sphere(virtual trackball)
		z = sqrt(pow(TRACKBALL_RADIUS, 2.0) - pow(distance, 2.0));
	else									// outside the sphere(virtual trackball) - on hyperbola
		z = pow((TRACKBALL_RADIUS / sqrt(2.0)), 2.0) / distance;
	return glm::normalize(glm::vec3(point.x, point.y, (float)z));
}


/// <summary>
/// Calculate rotation axis of the camera
/// </summary>
/// <param name="prev">: the previous mouse position on virtual trackball </param>
/// <param name="cur">: the current mouse position on virtual trackball </param>
/// <returns> the rotation axis of the camera </returns>
glm::vec3 CCamera::CalculateAxis(glm::vec3 const& prev, glm::vec3 const& cur)
{
	return glm::normalize(glm::cross(prev, cur));
}


/// <summary>
/// Calculate rotation angle of the camera
/// </summary>
/// <param name="prev">: the previous mouse position on virtual trackball </param>
/// <param name="cur">: the current mouse position on virtual trackball </param>
/// <returns> the rotation angle of the camera </returns>
float CCamera::CalculateAngle(glm::vec3 const& prev, glm::vec3 const& cur)
{
	return glm::acos(glm::dot(prev, cur));
}


/// <summary>
/// Refresh the origin orientation for quaternion rotation
/// </summary>
void CCamera::RefreshQuaternion()
{
	if (m_RefreshCount++ == REFRESH)
	{
		m_RefreshCount = 0;
		m_Orientation = glm::normalize(m_Orientation);
	}
}


/// <summary>
/// Set the Camera vectors
/// </summary>
/// <param name="eye">: the position of camera </param>
/// <param name="target">: the position of centroid of the target </param>
void CCamera::SetCameraVectors(glm::vec3 const& eye, glm::vec3 const& target)
{
 	m_ForwardAxis = glm::normalize(target - eye);
	m_RightAxis = glm::normalize(glm::cross(m_ForwardAxis, m_WorldUp));
	m_UpAxis = glm::cross(m_RightAxis, m_ForwardAxis);
}


/// <summary>
/// Set the light vectors
/// </summary>
void CCamera::SetLightVectors()
{
	m_LightColor = glm::vec3(LIGHT_R, LIGHT_G, LIGHT_B);
	m_LightPosition = glm::vec3(LIGHT_X, LIGHT_Y, LIGHT_Z);
}


/// <summary>
/// Re-calculate the related vectors of Camera and light for rotation
/// </summary>
/// <param name="q"> the rotation matrix using quaternion </param>
void CCamera::Update(glm::quat const& trans)
{
	m_Position = trans * m_Position;
	m_WorldUp = trans * m_WorldUp;
	m_Target = trans * m_Target;
	m_LightPosition = trans * m_LightPosition;
	SetCameraVectors(m_Position, m_Target);
}


/// <summary>
/// Return the Model-View Matrix calculated using the LookAt Matrix
/// </summary>
/// <returns> 4 x 4 Model-View Matrix </returns>
glm::mat4 CCamera::GetViewMatrix()
{
	return glm::lookAt(m_Position, m_Target, m_WorldUp);
}


/// <summary>
/// Retrieve the position of camera
/// </summary>
/// <returns> XYZ coordinate for the camera position </returns>
glm::vec3 CCamera::GetPosition()
{
	return m_Position;
}


/// <summary>
/// Set the Field Of View for camera
/// </summary>
/// <param name="value"> the FOV which users define </param>
void CCamera::SetZoom(float const& value)
{
	m_Zoom = value;
}


/// <summary>
/// Retrieve the Field of View for camera
/// </summary>
/// <returns> FOV of the camera </returns>
float CCamera::GetZoom()
{
	return m_Zoom;
}


/// <summary>
/// Retrieve the Color of light source
/// </summary>
/// <returns> the RGB value for color of the light source </returns>
glm::vec3 CCamera::GetLightColor()
{
	return m_LightColor;
}


/// <summary>
/// Retrieve the position of light source
/// </summary>
/// <returns> XYZ coordinates for the light source position </returns>
glm::vec3 CCamera::GetLightPosition()
{
	return m_LightPosition;
}


CCamera::~CCamera()
{
}
