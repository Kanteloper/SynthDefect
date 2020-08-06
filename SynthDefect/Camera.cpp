
// Camera.cpp for calculate the corresponding Vectors and Matrics for use in OpenGL

#include "pch.h"
#include "Camera.h"

CCamera::CCamera()
{
}

/// <summary>
/// Return the View Matrix calculated using the LookAt Matrix
/// </summary>
/// <returns></returns>
glm::mat4 CCamera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

CCamera::~CCamera()
{
}
