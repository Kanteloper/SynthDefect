
// Camera.h : interface of the Camera class

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default Camera Values
const float SPEED			= 2.5f;
const float SENSITIVITY		= 0.1f;
const float FOV				= 45.0f;

class CCamera
{
	// Constructor
public:
	CCamera();
	CCamera(glm::vec3 pos, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MoveSpeed(SPEED), MoveSensitivity(SENSITIVITY), Zoom(FOV) 
	{
		Position = pos;
		WorldUp = up;
		Right = glm::normalize(glm::cross(Front, WorldUp));
		Up = glm::normalize(glm::cross(Right, Front));
	};

	// Attributes
public:
	// camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// camera options
	float MoveSpeed;
	float MoveSensitivity;
	float Zoom;

	// Implementation
public:
	glm::mat4 GetViewMatrix();
	~CCamera();
};

