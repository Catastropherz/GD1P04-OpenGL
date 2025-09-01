#include "Camera.h"

Camera::Camera(GLFWwindow* _window)
{
	Window = _window;
	
	position = glm::vec3(0.0f, 1.0f, 3.0f);
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	lookDirection = glm::vec3(0.0f, 0.0f, -1.0f);

	// Calculate the View Matrix from the camera variables
	//center = Look Direction
	viewMatrix = glm::lookAt(position, position + lookDirection, upDirection);
	//center = TargetPos
	//viewMatrix = glm::lookAt(position, target, upDirection);
	projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);
}

Camera::~Camera()
{
}

void Camera::Update(float _currentTime)
{
	float deltaTime = _currentTime - previousTime;
	float frameTime = 1.0f / frameRate; // Calculate the time per frame based on the frame rate
	if (deltaTime > frameTime) // Adjust the frame rate as needed
	{
		// Update the camera position
		float radius = 3.0f; // Distance from target
		float speed = 0.5f;  // Radians per second

		// Calculate new position in a circle around the target
		float angle = TriBool() * _currentTime * speed; // based on input
		position.x = target.x + radius * sin(angle);
		position.z = target.z + radius * cos(angle);
		previousTime = _currentTime;
	}
	viewMatrix = glm::lookAt(position, target, upDirection);
}

int Camera::TriBool()
{
	int ret = 0;
	if (glfwGetKey(Window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		ret -= 1; // Move left
	}
	if (glfwGetKey(Window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		ret += 1; // Move right
	}

	return ret;
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
