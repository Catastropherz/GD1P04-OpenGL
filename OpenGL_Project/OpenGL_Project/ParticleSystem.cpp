#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Camera* cam, GLuint programRender, GLuint programCompute, glm::vec3 origin)
    : ActiveCamera(cam), Program_Render(programRender), Program_Compute(programCompute), EmitterOrigin(origin)
{
    GroupCountX = 1000;
    WorkGroupSizeX = 128;
    NumParticles = WorkGroupSizeX * GroupCountX;

    // Store Position and Lifespan information (SSBO Binding 0)
    glGenBuffers(1, &VBO_PositionLife);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO_PositionLife);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * NumParticles, NULL, GL_DYNAMIC_DRAW);

    // Store Velocity information (SSBO Binding 1)
    glGenBuffers(1, &VBO_Velocity);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO_Velocity);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * NumParticles, NULL, GL_DYNAMIC_DRAW);

    // Setup VAO & VBO link
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_PositionLife);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // Unbinding
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO_PositionLife);
    glDeleteBuffers(1, &VBO_Velocity);
}

void ParticleSystem::Update(float deltaTime)
{
    // Compute change in velocity (gravity) and lifetime delta
    glm::vec3 gravity = glm::vec3(0.0f, -9.8f, 0.0f) * deltaTime;
    VelocityLifeChange = glm::vec4(gravity, deltaTime);

    // Bind Compute Program & Pass Uniforms
    glUseProgram(Program_Compute);

    glUniform4fv(glGetUniformLocation(Program_Compute, "VelocityLifeChange"), 1, glm::value_ptr(VelocityLifeChange));
    glUniform3fv(glGetUniformLocation(Program_Compute, "EmitterOrigin"), 1, glm::value_ptr(EmitterOrigin));

    // Seeds for random numbers on GPU
    glUniform1i(glGetUniformLocation(Program_Compute, "SeedLife"), rand() % 10000);
    glUniform1i(glGetUniformLocation(Program_Compute, "SeedX"), rand() % 10000);
    glUniform1i(glGetUniformLocation(Program_Compute, "SeedY"), rand() % 10000);
    glUniform1i(glGetUniformLocation(Program_Compute, "SeedZ"), rand() % 10000);

    // Bind SSBO storage buffers
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO_PositionLife);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, VBO_Velocity);

    // Dispatch Compute Shader
    glDispatchCompute(GroupCountX, 1, 1);

    // Wait for compute shader completion before reading buffers in render pass
    glMemoryBarrier(GL_ALL_BARRIER_BITS);

    glUseProgram(0);
}

void ParticleSystem::Render()
{
    // Enable point size customization in vertex shader
    glEnable(GL_PROGRAM_POINT_SIZE);

    glUseProgram(Program_Render);

    // Pass VP matrix to render shader
    glm::mat4 VP = ActiveCamera->GetProjectionMatrix() * ActiveCamera->GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(Program_Render, "VP"), 1, GL_FALSE, glm::value_ptr(VP));

    // Draw Particles
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, NumParticles);

    // Cleanup & Unbind
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}