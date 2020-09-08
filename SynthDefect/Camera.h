
// Camera.h : interface of the Camera class

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CCamera
{
	// Constructor
public:
	CCamera();
	CCamera(glm::vec3 eye, glm::vec3 target);

	// Attributes
private:
	// for camera
	glm::vec3 m_position;
	glm::vec3 m_target;
	glm::vec3 m_forwardAxis;
	glm::vec3 m_upAxis;
	glm::vec3 m_rightAxis;
	glm::vec3 m_worldUp;
	float m_zoom;
	int m_refreshCount = 0;

	// for light
	glm::vec3 m_lightColor;
	glm::vec3 m_lightPosition;

	// Implementation
private:
	void SetCameraVectors(glm::vec3 const& eye, glm::vec3 const& target);
	void SetLightVectors();
public:
	glm::mat4 GetViewMatrix() const;
	glm::vec3 GetPosition() const;
	void SetZoom(float const& value);
	float GetZoom() const;
	glm::vec3 GetLightColor() const;
	glm::vec3 GetLightPosition() const;
	~CCamera();
};

