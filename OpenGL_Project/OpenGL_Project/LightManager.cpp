#include "LightManager.h"
#include <string>

LightManager::LightManager()
{

}

LightManager::~LightManager()
{
}

void LightManager::setAmbientLightStrength(float _strength, glm::vec3 _color)
{
	ambientColor = _color;
	ambientStrength = _strength;
}

void LightManager::addPointLight(glm::vec3 _position, glm::vec3 _color, float _specularStrength)
{
	if (pointLightCount < MAX_POINT_LIGHTS)
	{
		pointLightArray[pointLightCount].position = _position;
		pointLightArray[pointLightCount].color = _color;
		pointLightArray[pointLightCount].specularStrength = _specularStrength;
		pointLightCount++;
	}
}

void LightManager::setAttenuationForPointLight(int _index, float _constant, float _linear, float _exponent)
{
	if (_index >= 0 && _index < pointLightCount)
	{
		pointLightArray[_index].attenuationConstant = _constant;
		pointLightArray[_index].attenuationLinear = _linear;
		pointLightArray[_index].attenuationExponent = _exponent;
	}
}

void LightManager::setDirectionalLight(glm::vec3 _direction, glm::vec3 _color, float _specularStrength)
{
	directionalLight.direction = _direction;
	directionalLight.color = _color;
	directionalLight.specularStrength = _specularStrength;
}

void LightManager::applyLightsToShader(GLuint _shaderProgram)
{
	glUniform3fv(glGetUniformLocation(_shaderProgram, "AmbientColor"), 1, glm::value_ptr(ambientColor));
	glUniform1f(glGetUniformLocation(_shaderProgram, "AmbientStrength"), ambientStrength);
	glUniform1i(glGetUniformLocation(_shaderProgram, "PointLightCount"), pointLightCount);
	for (unsigned int i = 0; i < pointLightCount; i++)
	{
		std::string baseName = "PointLightArray[" + std::to_string(i) + "]";
		glUniform3fv(glGetUniformLocation(_shaderProgram, (baseName + ".Position").c_str()), 1, glm::value_ptr(pointLightArray[i].position));
		glUniform3fv(glGetUniformLocation(_shaderProgram, (baseName + ".Color").c_str()), 1, glm::value_ptr(pointLightArray[i].color));
		glUniform1f(glGetUniformLocation(_shaderProgram, (baseName + ".SpecularStrength").c_str()), pointLightArray[i].specularStrength);
		glUniform1f(glGetUniformLocation(_shaderProgram, (baseName + ".AttenuationConstant").c_str()), pointLightArray[i].attenuationConstant);
		glUniform1f(glGetUniformLocation(_shaderProgram, (baseName + ".AttenuationLinear").c_str()), pointLightArray[i].attenuationLinear);
		glUniform1f(glGetUniformLocation(_shaderProgram, (baseName + ".AttenuationExponent").c_str()), pointLightArray[i].attenuationExponent);
	}

	glUniform3fv(glGetUniformLocation(_shaderProgram, "Directional.Direction"), 1, glm::value_ptr(directionalLight.direction));
	glUniform3fv(glGetUniformLocation(_shaderProgram, "Directional.Color"), 1, glm::value_ptr(directionalLight.color));
	glUniform1f(glGetUniformLocation(_shaderProgram, "Directional.SpecularStrength"), directionalLight.specularStrength);
}
