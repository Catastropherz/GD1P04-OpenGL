#pragma once
#include "SkyBox.h"

struct PointLight
{
	glm::vec3 position;
	glm::vec3 color;
	float specularStrength;

	float attenuationConstant = 1.0f;
	float attenuationLinear = 0.09f;
	float attenuationExponent = 0.032f;
};

struct DirectionalLight
{
	glm::vec3 direction;
	glm::vec3 color;
	float specularStrength;
};

class LightManager
{
private:
	float ambientStrength = 0.2f;;
	glm::vec3 ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);;
	static const int MAX_POINT_LIGHTS = 4;
	PointLight pointLightArray[MAX_POINT_LIGHTS];
	unsigned int pointLightCount = 0;
	DirectionalLight directionalLight;

public:
	 LightManager();
	~LightManager();

	// Set ambient light properties
	void setAmbientLightStrength(float _strength, glm::vec3 _color);

	// Add a point light (position, color, specular strength)
	void addPointLight(glm::vec3 _position, glm::vec3 _color, float _specularStrength);
	
	// Set attenuation for a specific point light (index, constant, linear, exponent)
	void setAttenuationForPointLight(int _index, float _constant, float _linear, float _exponent);
	
	// Set directional light
	void setDirectionalLight(glm::vec3 _direction, glm::vec3 _color, float _specularStrength);
	
	// Apply light properties to the shader program
	void applyLightsToShader(GLuint _shaderProgram);
};

