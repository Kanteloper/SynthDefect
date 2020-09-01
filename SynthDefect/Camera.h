
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
	glm::quat m_orientation;
	float m_zoom;
	int m_refreshCount = 0;

	// for light
	glm::vec3 m_lightColor;
	glm::vec3 m_lightPosition;

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

