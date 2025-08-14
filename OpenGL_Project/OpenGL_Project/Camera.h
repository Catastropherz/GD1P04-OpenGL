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
public:
	Camera();
	~Camera();

	void Update(float _deltaTime);
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
};

