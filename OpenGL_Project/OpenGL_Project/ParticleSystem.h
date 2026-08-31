#pragma once
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Camera.h"

class ParticleSystem
{
private:
    Camera* ActiveCamera = nullptr;
    GLuint Program_Render = 0;
    GLuint Program_Compute = 0;

    GLuint VAO = 0;
    GLuint VBO_PositionLife = 0;
    GLuint VBO_Velocity = 0;

    glm::vec3 EmitterOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec4 VelocityLifeChange = glm::vec4(0.0f);

    int GroupCountX = 1000;
    int WorkGroupSizeX = 128;
    int NumParticles = 0;

public:
    ParticleSystem(Camera* cam, GLuint programRender, GLuint programCompute, glm::vec3 origin);
    ~ParticleSystem();

    void Update(float deltaTime);
    void Render();
};