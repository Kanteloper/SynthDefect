
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
		m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_Orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		SetCameraVectors(m_Position, m_Target);
	};

	// Attributes
private:
	glm::vec3 m_Position;
	glm::vec3 m_Target;
	glm::vec3 m_ForwardAxis;
	glm::vec3 m_UpAxis;
	glm::vec3 m_RightAxis;
	glm::vec3 m_WorldUp;
	glm::quat m_Orientation;
	float m_Zoom;
	int m_RefreshCount = 0;

	// Implementation
private:
	glm::vec3 ProjectTrackBall(glm::vec2 point);
	glm::vec3 CalculateAxis(glm::vec3 const& prev, glm::vec3 const& cur);
	float CalculateAngle(glm::vec3 prev, glm::vec3 cur);
	void RefreshQuaternion();
	void SetCameraVectors(glm::vec3 eye, glm::vec3 target);
	void UpdateCameraVectors(glm::quat trans);
public:
	void Rotate(glm::vec2 prev, glm::vec2 cur);
	glm::mat4 GetViewMatrix();
	glm::vec3 GetPosition();
	void SetZoom(float value);
	float GetZoom();
	~CCamera();
};

