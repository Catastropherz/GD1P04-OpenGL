#include "Camera.h"
#include <algorithm>

Camera::Camera(GLFWwindow* _window)
{
	Window = _window;
	
	position = glm::vec3(0.0f, 15.0f, 80.0f);
	target = glm::vec3(0.0f, -15.0f, 0.0f);
	upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	lookDirection = glm::vec3(0.0f, -15.0f, -1.0f);

	// Calculate the View Matrix from the camera variables
	//center = Look Direction
	viewMatrix = glm::lookAt(position, position + lookDirection, upDirection);
	//center = TargetPos
	//viewMatrix = glm::lookAt(position, target, upDirection);
	projectionMatrix = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 1000.0f);
	//projectionMatrix = glm::ortho(-400.0f, 400.0f, -400.0f, 400.0f, 0.1f, 100.0f);
}

Camera::~Camera()
{
}

void Camera::Update(float _currentTime, bool _isOrbit)
{
	float deltaTime = _currentTime - previousTime;
	float frameTime = 1.0f / frameRate; // Calculate the time per frame based on the frame rate
	if (deltaTime > frameTime) // Adjust the frame rate as needed
	{
		// Update the previous time
		previousTime = _currentTime;

		if (_isOrbit)
		{
			orbitTime += deltaTime;

			// Orbital movement parameters
			float amplitude = 50.0f; // Amplitude of the oscillation
			float speed = 0.5f;  // Radians per second

			// Orbital movement around the origin
			position.x = amplitude * sin(speed * orbitTime);
			position.z = amplitude * cos(speed * orbitTime);
			position.y = 0.0f; // Fixed height

			// Update the look direction to always face the origin
			lookDirection = glm::normalize(-position); // Look towards the origin
		}

		
	}
	//viewMatrix = glm::lookAt(position, target, upDirection);
	viewMatrix = glm::lookAt(position, position + lookDirection, upDirection);
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

glm::vec3 Camera::GetCameraPosition() const
{
	return position;
}

glm::vec3 Camera::GetCameraForwardDirection() const
{
	return glm::normalize(lookDirection);
}

glm::vec3 Camera::GetCameraRightDirection() const
{
	return glm::normalize(glm::cross(lookDirection, upDirection));
}

void Camera::SetProjectionMatrix_Perspective(int _width, int _height, float _fov, float _nearPlane, float _farPlane)
{
	projectionMatrix = glm::perspective(glm::radians(_fov), static_cast<float>(_width) / static_cast<float>(_height), _nearPlane, _farPlane);
}

void Camera::SetProjectionMatrix_Orthographic(int _left, int _right, int _bottom, int _top)
{
	projectionMatrix = glm::ortho(static_cast<float>(_left), static_cast<float>(_right), static_cast<float>(_bottom), static_cast<float>(_top));
}

void Camera::MoveCamera(glm::vec3 _direction)
{
	position += _direction;
}

void Camera::ProcessMouseMovement(double _xpos, double _ypos)
{
	if (firstMouse)
	{
		lastMouseX = _xpos;
		lastMouseY = _ypos;
		firstMouse = false;
	}

	float xoffset = static_cast<float>(_xpos - lastMouseX);
	float yoffset = static_cast<float>(lastMouseY - _ypos); // reversed: y ranges bottom to top
	lastMouseX = _xpos;
	lastMouseY = _ypos;

	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Clamp the pitch to prevent flipping
	if (pitch < -89.0f) pitch = -89.0f;
	if (pitch > 89.0f)  pitch = 89.0f;

	// Update the camera's direction vector
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	lookDirection = glm::normalize(direction);
	target = position + lookDirection;
}

void Camera::ProcessMouseScroll(double _yoffset)
{
	fov -= static_cast<float>(_yoffset);
	fov = std::max(1.0f, std::min(90.0f, fov)); // Clamp FOV between 1 and 90 degrees

	// Update the projection matrix with the new FOV
	int width, height;
	glfwGetFramebufferSize(Window, &width, &height);
	SetProjectionMatrix_Perspective(width, height, fov, 0.1f, 1000.0f);

}

//void Camera::SetProjectionMatrix_Orthographic(int _left, int _right, int _bottom, int _top, float _nearPlane, float _farPlane)
//{
//	projectionMatrix = glm::ortho(static_cast<float>(_left), static_cast<float>(_right), static_cast<float>(_bottom), static_cast<float>(_top), _nearPlane, _farPlane);
//}


