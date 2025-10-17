/***********************************************************************
 Bachelor of Software Engineering
 Media Design School
 Auckland
 New Zealand
 (c)
 2024 Media Design School
 File Name : Camera.h
 Description : Handle camera movement and view/projection matrix
 Author : Q Sivakorn Tuangwilai
 Mail : sivakorn.tuangwilai@mds.ac.nz
 **************************************************************************/

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

	bool isOrbiting = true; // Orbiting movement around the origin
	float orbitTime = 0.0f; // Storing orbit position

public:
	Camera(GLFWwindow* _window);
	~Camera();

	// Update the camera's position and orientation
	void Update(float _currentTime, bool _isOrbit);

	// Returns -1 for left, 0 for no movement, and 1 for right
	int TriBool();

	// Getter functions for the View Matrix
	glm::mat4 GetViewMatrix() const;

	// Getter function for the Projection Matrix
	glm::mat4 GetProjectionMatrix() const;

	// Getter function for the Camera Position
	glm::vec3 GetCameraPosition() const;

	// Getter function for the Camera Forward Position
	glm::vec3 GetCameraForwardDirection() const;

	// Getter function for camera right direction
	glm::vec3 GetCameraRightDirection() const;

	// Setter functions for the Projection Matrix (Perspective)
	void SetProjectionMatrix_Perspective(int _width, int _height, float _fov = 45.0f, float _nearPlane = 0.1f, float _farPlane = 100.0f);

	// Setter functions for the Projection Matrix (Orthographic)
	void SetProjectionMatrix_Orthographic(int _left, int _right, int _bottom, int _top, float _nearPlane = 0.1f, float _farPlane = 100.0f);
};

