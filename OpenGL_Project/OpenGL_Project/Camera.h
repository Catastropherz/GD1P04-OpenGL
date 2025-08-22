#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Camera
{
private:
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 upDirection;
	glm::vec3 lookDirection;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	GLFWwindow* Window = nullptr;

	float previousTime = 0.0f;
	float frameRate = 60.0f; // Frames per second
public:
	Camera(GLFWwindow* _window);
	~Camera();

	void Update(float _currentTime);
	int TriBool();
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	void SetProjectionMatrix_Perspective(int _width, int _height, float _fov = 45.0f, float _nearPlane = 0.1f, float _farPlane = 100.0f);
	void SetProjectionMatrix_Orthographic(int _left, int _right, int _bottom, int _top, float _nearPlane = 0.1f, float _farPlane = 100.0f);
};

