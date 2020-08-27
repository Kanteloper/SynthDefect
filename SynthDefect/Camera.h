
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
		SetLightVectors();
	};

	// Attributes
private:
	// for camera
	glm::vec3 m_Position;
	glm::vec3 m_Target;
	glm::vec3 m_ForwardAxis;
	glm::vec3 m_UpAxis;
	glm::vec3 m_RightAxis;
	glm::vec3 m_WorldUp;
	glm::quat m_Orientation;
	float m_Zoom;
	int m_RefreshCount = 0;

	// for light
	glm::vec3 m_LightColor;
	glm::vec3 m_LightPosition;

	// Implementation
private:
	glm::vec3 ProjectTrackBall(glm::vec2 const& point);
	glm::vec3 CalculateAxis(glm::vec3 const& prev, glm::vec3 const& cur);
	float CalculateAngle(glm::vec3 const& prev, glm::vec3 const& cur);
	void RefreshQuaternion();
	void SetCameraVectors(glm::vec3 const& eye, glm::vec3 const& target);
	void SetLightVectors();
	void Update(glm::quat const& trans);
public:
	void Rotate(glm::vec2 const& prev, glm::vec2 const& cur);
	glm::mat4 GetViewMatrix();
	glm::vec3 GetPosition();
	void SetZoom(float const& value);
	float GetZoom();
	glm::vec3 GetLightColor();
	glm::vec3 GetLightPosition();
	~CCamera();
};

