
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

	//float rotation_amount = glm::dot(prev_trackball, cur_trackball);										// calculate the amount of rotation
	float rotation_amount = glm::distance(prev_trackball, cur_trackball) / (2.0f * radius);
	rotation_amount = rotation_amount > 1.0f ? 1.0f : rotation_amount < -1.0f ? -1.0f : rotation_amount;	// clamp the amount of rotation
	rotation_amount = 2.0f * glm::asin(rotation_amount);
	//rotation_amount = glm::acos(rotation_amount);

	if (m_count++ == 50)
	{
		m_count = 0;
		glm::normalize(q);
	}
	// calculate rotation matrix
	q = glm::angleAxis((float)cos(rotation_amount / 2.0f), rotation_axis);

	// transform to the coordinates that is originated LookAt point
	glm::vec3 lookat_point = m_Target + m_ForwardAxis * 0.1f;
	glm::mat4 translation_mat = glm::translate(glm::mat4(1), -lookat_point);
	glm::mat4 inverse_translation_mat = glm::inverse(translation_mat);

	UpdateCameraVectors(q);
}


/// <summary>
/// Calculate the coordinates of mouse point projected to the virtual trackball
/// </summary>
/// <param name="point">: the mouse point for transforming Normalized Device Coordinates </param>
/// <returns> the NDC of mouse point </returns>
glm::vec3 CCamera::ProjectTrackBall(glm::vec2 point)
{
	float z = 0.0f;
	double distance = sqrt(pow(point.x, 2.0) + pow(point.y, 2.0));
	// check the mouse point is inside the virtua trackball or not
	if (distance < (radius / sqrt(2.0)))	// inside the sphere(virtual trackball)
		z = (float)sqrt(pow(radius, 2.0) - pow(distance, 2.0));
	else									// outside the sphere(virtual trackball) - on hyperbola
		z = (float)pow((radius / sqrt(2.0)), 2.0) / (float)distance;
	return glm::vec3(point.x, point.y, z);
}


/// <summary>
/// Set the default Camera vectors
/// </summary>
/// <param name="eye">: the position of camera </param>
/// <param name="target">: the position of centroid of the target </param>
void CCamera::SetCameraVectors(glm::vec3 eye, glm::vec3 target)
{
	m_ForwardAxis = glm::normalize(target - eye);
	m_RightAxis = glm::normalize(glm::cross(m_ForwardAxis, m_UpDirection));
	m_UpAxis = glm::cross(m_RightAxis, m_ForwardAxis);
}


/// <summary>
/// Re-calculate the related vectors of Camera for rotation
/// </summary>
/// <param name="q"> the rotation matrix using quaternion </param>
void CCamera::UpdateCameraVectors(glm::quat trans)
{
	m_RightAxis = glm::normalize(MultiplyQuaternion(trans, m_RightAxis));
	m_ForwardAxis = glm::normalize(MultiplyQuaternion(trans, m_ForwardAxis));
	m_UpAxis = glm::normalize(MultiplyQuaternion(trans, m_UpAxis));
	m_Target = MultiplyQuaternion(trans, m_Target);

	// camera direction calibration
	m_Position = MultiplyQuaternion(trans, m_Position);
	SetCameraVectors(m_Position, m_Target);
}


glm::vec3 CCamera::MultiplyQuaternion(glm::quat q, glm::vec3 v)
{
	// v' = q * v * conjugate(q)
	return v + 2.0f * glm::cross(glm::vec3(q.x, q.y, q.z), glm::cross(glm::vec3(q.x, q.y, q.z), v) + q.w * v);
}


/// <summary>
/// 
/// </summary>
/// <param name="right"></param>
/// <param name="up"></param>
/// <param name="forward"></param>
/// <param name="center"></param>
/// <returns></returns>
glm::mat4 CCamera::LookAt(glm::vec3 const& right, glm::vec3 const& up, glm::vec3 const& forward, glm::vec3 const& center)
{
	glm::mat4 result(1);
	result[0][0] = right.x;
	result[1][0] = right.y;
	result[2][0] = right.z;
	result[3][0] = -glm::dot(right, m_Position);
	result[0][1] = up.x;
	result[1][1] = up.y;
	result[2][1] = up.z;
	result[3][1] = -glm::dot(up, m_Position);
	result[0][2] = -forward.x;
	result[1][2] = -forward.y;
	result[2][2] = -forward.z;
	result[3][2] = glm::dot(forward, m_Position);
	return result;
}




/// <summary>
/// Return the Model-View Matrix calculated using the LookAt Matrix
/// </summary>
/// <returns> 4 x 4 Model-View Matrix </returns>
glm::mat4 CCamera::GetViewMatrix()
{
	// lookAt - Translate whole scene(camera + object) from the eye position to the origin(0, 0, 0)
	//          Rotate for facing to the -z axis
	return LookAt(m_RightAxis, m_UpAxis, m_ForwardAxis, m_Target);
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
