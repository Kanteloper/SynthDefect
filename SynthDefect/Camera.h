
// Camera.h : interface of the Camera class

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

class CCamera
{
	// Constructor
public:
	CCamera();
	CCamera(glm::vec3 eye, glm::vec3 target) : m_Zoom(FOV)
	{
		m_Position = eye;
		m_Target = target;
		m_UpDirection = glm::vec3(0.0f, 1.0f, 0.0f);								// Assume the camera is straight up to +y axis
		SetCameraVectors(m_Position, m_Target);
	};

	// Attributes
private:
	// camera attributes
	glm::vec3 m_Position;
	glm::vec3 m_Target;
	glm::vec3 m_ForwardAxis;
	glm::vec3 m_UpAxis;
	glm::vec3 m_RightAxis;
	glm::vec3 m_UpDirection;
	float m_Zoom;
	const float radius = 1.0f;
public:


	// Implementation
private:
	glm::vec3 ProjectTrackBall(glm::vec2 point);
	void SetCameraVectors(glm::vec3 eye, glm::vec3 target);
public:
	void Rotate(glm::vec2 prev, glm::vec2 cur);
	glm::mat4 GetViewMatrix();
	glm::vec3 GetPosition();
	glm::vec3 GetForwardAxis();
	glm::vec3 GetUpAxis();
	glm::vec3 GetLeftAxis();
	void SetZoom(float value);
	float GetZoom();
	~CCamera();
};

