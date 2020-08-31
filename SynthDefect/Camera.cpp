
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
	m_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	SetCameraVectors(m_position, m_target);
	SetLightVectors();
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
	glm::quat transform = m_orientation * change;
	m_orientation = change;
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
	if (m_refreshCount++ == REFRESH)
	{
		m_refreshCount = 0;
		m_orientation = glm::normalize(m_orientation);
	}
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
/// Re-calculate the related vectors of Camera and light for rotation
/// </summary>
/// <param name="q"> the rotation matrix using quaternion </param>
void CCamera::Update(glm::quat const& trans)
{
	m_position = trans * m_position;
	m_worldUp = trans * m_worldUp;
	m_target = trans * m_target;
	m_lightPosition = trans * m_lightPosition;
	SetCameraVectors(m_position, m_target);
}


/// <summary>
/// Return the Model-View Matrix calculated using the LookAt Matrix
/// </summary>
/// <returns> 4 x 4 Model-View Matrix </returns>
glm::mat4 CCamera::GetViewMatrix()
{
	return glm::lookAt(m_position, m_target, m_worldUp);
}


/// <summary>
/// Retrieve the position of camera
/// </summary>
/// <returns> XYZ coordinate for the camera position </returns>
glm::vec3 CCamera::GetPosition()
{
	return m_position;
}


/// <summary>
/// Set the postion of camera
/// </summary>
/// <param name="pos">: the coordinate to be set for the camera's position </param>
void CCamera::SetPosition(glm::vec3 const& pos)
{
	m_position = pos;
	SetCameraVectors(m_position, m_target);
}


/// <summary>
/// Set the target of camera which looks at
/// </summary>
/// <param name="pos">the coordinate to be set for camera's target </param>
void CCamera::SetTarget(glm::vec3 const& target)
{
	m_target = target;
	SetCameraVectors(m_position, m_target);
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
float CCamera::GetZoom()
{
	return m_zoom;
}


/// <summary>
/// Retrieve the Color of light source
/// </summary>
/// <returns> the RGB value for color of the light source </returns>
glm::vec3 CCamera::GetLightColor()
{
	return m_lightColor;
}


/// <summary>
/// Retrieve the position of light source
/// </summary>
/// <returns> XYZ coordinates for the light source position </returns>
glm::vec3 CCamera::GetLightPosition()
{
	return m_lightPosition;
}


CCamera::~CCamera()
{
}
