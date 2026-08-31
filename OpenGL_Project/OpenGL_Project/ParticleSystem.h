#pragma once
#include <glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "Camera.h"

struct FireworkInstance
{
    glm::vec3 Origin;
    glm::vec3 Color;
    float TrailTime; // Time spent moving upwards before exploding
    bool Active = false;
};

class ParticleSystem
{
private:
    Camera* ActiveCamera = nullptr;
    GLuint Program_Render = 0;
    GLuint Program_Compute = 0;

    GLuint VAO = 0;
    GLuint VBO_PositionLife = 0;
    GLuint VBO_Velocity = 0;

    // Buffer layout dimensions
    int WorkGroupSizeX = 128;
    int ParticlesPerFirework = 102400; // Total particles per firework
    int MaxFireworks = 4;
    int NumParticles = 0;

    std::vector<FireworkInstance> Fireworks;

public:
    ParticleSystem(Camera* cam, GLuint programRender, GLuint programCompute);
    ~ParticleSystem();

    void SpawnFireworks(int count = 4);
    void Update(float deltaTime);
    void Render();
};