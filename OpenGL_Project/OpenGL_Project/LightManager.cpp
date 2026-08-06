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
	// Avoid signed/unsigned comparison warning (C4018) by casting pointLightCount to int
	if (_index >= 0 && _index < static_cast<int>(pointLightCount))
	{
		pointLightArray[_index].attenuationConstant = _constant;
		pointLightArray[_index].attenuationLinear = _linear;
		pointLightArray[_index].attenuationExponent = _exponent;
	}
}

void LightManager::setSpotlight(glm::vec3 _position, glm::vec3 _direction, float _cutOff, float _outerCutOff, glm::vec3 _color, float _specularStrength)
{
	spotlight.position = _position;
	spotlight.direction = _direction;
	spotlight.cutOff = glm::cos(glm::radians(_cutOff));
	spotlight.outerCutOff = glm::cos(glm::radians(_outerCutOff));
	spotlight.color = _color;
	spotlight.specularStrength = _specularStrength;
}

void LightManager::setDirectionalLight(glm::vec3 _direction, glm::vec3 _color, float _specularStrength)
{
	directionalLight.direction = _direction;
	directionalLight.color = _color;
	directionalLight.specularStrength = _specularStrength;
}

void LightManager::applyLightsToShader(GLuint _shaderProgram)
{
	// Zero color for disabling lights
	glm::vec3 zeroColor(0.0f, 0.0f, 0.0f);

	// Ambient Light
	glUniform3fv(glGetUniformLocation(_shaderProgram, "AmbientColor"), 1, glm::value_ptr(ambientColor));
	glUniform1f(glGetUniformLocation(_shaderProgram, "AmbientStrength"), ambientStrength);
	glUniform1i(glGetUniformLocation(_shaderProgram, "PointLightCount"), pointLightCount);

	// Point Lights
	for (unsigned int i = 0; i < pointLightCount; i++)
	{
		std::string baseName = "PointLightArray[" + std::to_string(i) + "]";
		glUniform3fv(glGetUniformLocation(_shaderProgram, (baseName + ".Position").c_str()), 1, glm::value_ptr(pointLightArray[i].position));
		glUniform1f(glGetUniformLocation(_shaderProgram, (baseName + ".AttenuationConstant").c_str()), pointLightArray[i].attenuationConstant);
		glUniform1f(glGetUniformLocation(_shaderProgram, (baseName + ".AttenuationLinear").c_str()), pointLightArray[i].attenuationLinear);
		glUniform1f(glGetUniformLocation(_shaderProgram, (baseName + ".AttenuationExponent").c_str()), pointLightArray[i].attenuationExponent);
	}
	if (enablePointLight)
	{
		for (unsigned int i = 0; i < pointLightCount; i++)
		{
			std::string baseName = "PointLightArray[" + std::to_string(i) + "]";
			glUniform3fv(glGetUniformLocation(_shaderProgram, (baseName + ".Color").c_str()), 1, glm::value_ptr(pointLightArray[i].color));
			glUniform1f(glGetUniformLocation(_shaderProgram, (baseName + ".SpecularStrength").c_str()), pointLightArray[i].specularStrength);
		}
	}
	else
	{
		for (unsigned int i = 0; i < pointLightCount; i++)
		{
			std::string baseName = "PointLightArray[" + std::to_string(i) + "]";
			glUniform3fv(glGetUniformLocation(_shaderProgram, (baseName + ".Color").c_str()), 1, glm::value_ptr(zeroColor));
			glUniform1f(glGetUniformLocation(_shaderProgram, (baseName + ".SpecularStrength").c_str()), 0.0f);
		}
	}

	// Directional Light
	glUniform3fv(glGetUniformLocation(_shaderProgram, "Directional.Direction"), 1, glm::value_ptr(directionalLight.direction));
	if (enableDirectionalLight)
	{
		glUniform3fv(glGetUniformLocation(_shaderProgram, "Directional.Color"), 1, glm::value_ptr(directionalLight.color));
		glUniform1f(glGetUniformLocation(_shaderProgram, "Directional.SpecularStrength"), directionalLight.specularStrength);
	}
	else
	{
		glUniform3fv(glGetUniformLocation(_shaderProgram, "Directional.Color"), 1, glm::value_ptr(zeroColor));
		glUniform1f(glGetUniformLocation(_shaderProgram, "Directional.SpecularStrength"), 0.0f);
	}

	// Spotlight
	glUniform3fv(glGetUniformLocation(_shaderProgram, "Spotlight.Position"), 1, glm::value_ptr(spotlight.position));
	glUniform3fv(glGetUniformLocation(_shaderProgram, "Spotlight.Direction"), 1, glm::value_ptr(spotlight.direction));
	glUniform1f(glGetUniformLocation(_shaderProgram, "Spotlight.CutOff"), spotlight.cutOff);
	glUniform1f(glGetUniformLocation(_shaderProgram, "Spotlight.OuterCutOff"), spotlight.outerCutOff);
	if (enableSpotlight)
	{
		glUniform3fv(glGetUniformLocation(_shaderProgram, "Spotlight.Color"), 1, glm::value_ptr(spotlight.color));
		glUniform1f(glGetUniformLocation(_shaderProgram, "Spotlight.SpecularStrength"), spotlight.specularStrength);
	}
	else
	{
		glUniform3fv(glGetUniformLocation(_shaderProgram, "Spotlight.Color"), 1, glm::value_ptr(zeroColor));
		glUniform1f(glGetUniformLocation(_shaderProgram, "Spotlight.SpecularStrength"), 0.0f);
	}
}

void LightManager::togglePointLight(bool _enable)
{
	enablePointLight = _enable;
}

void LightManager::toggleDirectionalLight(bool _enable)
{
	enableDirectionalLight = _enable;
}

void LightManager::toggleSpotlight(bool _enable)
{
	enableSpotlight = _enable;
}
