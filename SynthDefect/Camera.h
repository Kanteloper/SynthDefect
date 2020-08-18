
// Camera.h : interface of the Camera class

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default Camera Values
const float SPEED			= 2.5f;
const float SENSITIVITY		= 4.0f;
const float FOV				= 45.0f;

class CCamera
{
	// Constructor
public:
	CCamera();
	CCamera(glm::vec3 eye, glm::vec3 target) : m_MoveSensitivity(SENSITIVITY), m_Zoom(FOV)
	{
		m_Position = eye;
		m_UpDirection = glm::vec3(0.0f, 1.0f, 0.0f);								// Assume the camera is straight up to +y axis
		m_ForwardAxis = glm::normalize(eye - target);
		m_LeftAxis = glm::normalize(glm::cross(m_ForwardAxis, m_UpDirection));
		m_UpAxis = glm::cross(m_LeftAxis, m_ForwardAxis);
	};

	// Attributes
private:
	// camera attributes
	glm::vec3 m_Position;
	glm::vec3 m_ForwardAxis;
	glm::vec3 m_UpAxis;
	glm::vec3 m_LeftAxis;
	glm::vec3 m_UpDirection;
public:
	// camera options
	float m_MoveSensitivity;
	float m_Zoom;

	// Implementation
public:
	glm::mat4 GetViewMatrix();
	glm::vec3 GetPosition();
	glm::vec3 GetForwardAxis();
	glm::vec3 GetUpAxis();
	glm::vec3 GetLeftAxis();
	const float GetFOV();
	const float GetSensitivity();
	const float GetSpeed();
	~CCamera();
};

