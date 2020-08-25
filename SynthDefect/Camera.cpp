
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

	// calculate rotation axis
	prev_trackball = glm::normalize(prev_trackball);
	cur_trackball = glm::normalize(cur_trackball);
	glm::vec3 rotation_axis = glm::cross(prev_trackball, cur_trackball);
	rotation_axis = glm::normalize(rotation_axis);
	std::cout << "rot_axis: " << glm::to_string(rotation_axis) << std::endl;

	float rotation_amount = glm::dot(prev_trackball, cur_trackball);										// calculate the amount of rotation
	rotation_amount = rotation_amount > 1.0f ? 1.0f : rotation_amount < -1.0f ? -1.0f : rotation_amount;	// clamp the amount of rotation
	rotation_amount = glm::acos(rotation_amount);
	
	// calculate rotation matrix
	glm::mat4 rot_mat = glm::rotate(glm::mat4(1), -rotation_amount, rotation_axis);

	UpdateCameraVectors(rot_mat);
}



/// <summary>
/// Calculate the coordinates of mouse point projected to the virtual trackball
/// </summary>
/// <param name="point">: the mouse point for transforming Normalized Device Coordinates </param>
/// <returns> the NDC of mouse point </returns>
glm::vec3 CCamera::ProjectTrackBall(glm::vec2 point)
{
	double z = 0.0f;
	double distance = sqrt(pow(point.x, 2.0) + pow(point.y, 2.0));
	// check the mouse point is inside the virtua trackball or not
	if (distance < (radius / sqrt(2.0)))	// inside the sphere(virtual trackball)
		z = sqrt(pow(radius, 2.0) - pow(distance, 2.0));
	else									// outside the sphere(virtual trackball) - on hyperbola
		z = pow((radius / sqrt(2.0)), 2.0) / distance;
	return glm::vec3(point.x, point.y, (float)z);
}


/// <summary>
/// Set the default Camera vectors
/// </summary>
/// <param name="eye">: the position of camera </param>
/// <param name="target">: the position of centroid of the target </param>
void CCamera::SetCameraVectors(glm::vec3 eye, glm::vec3 target)
{
	std::cout << "eye: " << glm::to_string(eye) << std::endl;
	std::cout << "target: " << glm::to_string(target) << std::endl;
	m_ForwardAxis = glm::normalize(target - eye);
	std::cout << "forward: " << glm::to_string(m_ForwardAxis) << std::endl;
	m_RightAxis = glm::normalize(glm::cross(m_ForwardAxis, m_UpDirection));
	//std::cout << "right: " << glm::to_string(m_RightAxis) << std::endl;
	m_UpAxis = glm::cross(m_RightAxis, m_ForwardAxis);
	//std::cout << "up: " << glm::to_string(m_UpAxis) << std::endl;
}


/// <summary>
/// 
/// </summary>
/// <param name="trans"></param>
void CCamera::UpdateCameraVectors(glm::mat4 trans)
{

	m_Target = trans * glm::vec4(m_Target, 1.0);
	m_Position = trans * glm::vec4(m_Position, 1.0);
	m_RightAxis = glm::normalize(trans * glm::vec4(m_RightAxis, 1.0));
	m_UpDirection = glm::normalize(trans * glm::vec4(m_UpDirection, 1.0));
	SetCameraVectors(m_Position, m_Target);
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
