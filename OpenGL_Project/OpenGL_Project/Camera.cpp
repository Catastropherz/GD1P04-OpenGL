#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(1.0f, 1.0f, 3.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	lookDirection = glm::vec3(0.0f, 0.0f, -1.0f);

	// Calculate the View Matrix from the camera variables
	//center = Look Direction
	//	viewMatrix = glm::lookAt(position, position + lookDirection, upDirection);
	//center = TargetPos
	viewMatrix = glm::lookAt(position, target, upDirection);
}

Camera::~Camera()
{
}

void Camera::Update(float _deltaTime)
{
}

glm::mat4 Camera::GetViewMatrix() const
{
    return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return projectionMatrix;
}

void Camera::SetProjectionMatrix_Perspective(int _width, int _height, float _fov, float _nearPlane, float _farPlane)
{
	projectionMatrix = glm::perspective(glm::radians(_fov), static_cast<float>(_width) / static_cast<float>(_height), _nearPlane, _farPlane);
}

void Camera::SetProjectionMatrix_Orthographic(int _left, int _right, int _bottom, int _top, float _nearPlane, float _farPlane)
{
	projectionMatrix = glm::ortho(static_cast<float>(_left), static_cast<float>(_right), static_cast<float>(_bottom), static_cast<float>(_top), _nearPlane, _farPlane);
}
