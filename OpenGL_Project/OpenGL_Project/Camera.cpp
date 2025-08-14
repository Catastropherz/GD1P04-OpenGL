#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Update(float _deltaTime)
{
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::mat4();
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::mat4();
}
