#pragma once
#include "SkyBox.h"

struct PointLight
{
	glm::vec3 position;
	glm::vec3 color;
	float specularStrength;

};

class LightManager
{
private:
	float ambientStrength;
	glm::vec3 ambientColor;
	static const int MAX_POINT_LIGHTS = 4;
	PointLight pointLightArray[MAX_POINT_LIGHTS];
	unsigned int pointLightCount = 0;

public:
	 LightManager();
	~LightManager();
	void setAmbientLightStrength(float _strength, glm::vec3 _color);
	void addPointLight(glm::vec3 _position, glm::vec3 _color, float _specularStrength);
	void applyLightsToShader(GLuint _shaderProgram);
};

