#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Camera* cam, GLuint programRender, GLuint programCompute)
    : ActiveCamera(cam), Program_Render(programRender), Program_Compute(programCompute)
{
    NumParticles = ParticlesPerFirework * MaxFireworks;
    Fireworks.resize(MaxFireworks);

    for (int i = 0; i < MaxFireworks; ++i)
    {
        Fireworks[i].Active = false;
    }

    // 1. Position and Lifespan (xyz = Pos, w = Life)
    glGenBuffers(1, &VBO_PositionLife);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO_PositionLife);
    std::vector<glm::vec4> zeroBuffer(NumParticles, glm::vec4(0.0f));
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * NumParticles, zeroBuffer.data(), GL_DYNAMIC_DRAW);

    // 2. Velocity (xyz = Vel, w = MaxLife for alpha calculation)
    glGenBuffers(1, &VBO_Velocity);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO_Velocity);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::vec4) * NumParticles, zeroBuffer.data(), GL_DYNAMIC_DRAW);

    // 3. Setup VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_PositionLife);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

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

void ParticleSystem::SpawnFireworks(int count)
{
    // Fully reset buffer data on GPU
    std::vector<glm::vec4> zeroBuffer(NumParticles, glm::vec4(0.0f));

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO_PositionLife);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4) * NumParticles, zeroBuffer.data());

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, VBO_Velocity);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec4) * NumParticles, zeroBuffer.data());

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Configure firework instances
    int spawned = 0;
    for (int i = 0; i < MaxFireworks && spawned < count; ++i)
    {
        Fireworks[i].Active = true;

        // Spread spawn origins horizontally across ground
        float rx = ((rand() % 400) - 200) * 0.5f; // -100 to +100
        float rz = 120.0f + ((rand() % 200) - 100) * 0.5f;
        Fireworks[i].Origin = glm::vec3(rx, 0.0f, rz);

        // Randomized bright colors
        Fireworks[i].Color = glm::vec3(
            0.4f + (rand() % 60) / 100.0f,
            0.4f + (rand() % 60) / 100.0f,
            0.4f + (rand() % 60) / 100.0f
        );

        // Trail launch duration before spherical burst
        Fireworks[i].TrailTime = 2.0f + (rand() % 250) / 100.0f;

        spawned++;
    }
}

void ParticleSystem::Update(float deltaTime)
{
    glUseProgram(Program_Compute);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO_PositionLife);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, VBO_Velocity);

    bool anyActive = false;

    for (int i = 0; i < MaxFireworks; ++i)
    {
        if (!Fireworks[i].Active) continue;
        anyActive = true;

        glUniform1i(glGetUniformLocation(Program_Compute, "FireworkIndex"), i);
        glUniform1i(glGetUniformLocation(Program_Compute, "ParticlesPerFirework"), ParticlesPerFirework);
        glUniform1f(glGetUniformLocation(Program_Compute, "DeltaTime"), deltaTime);
        glUniform1f(glGetUniformLocation(Program_Compute, "TrailTime"), Fireworks[i].TrailTime);
        glUniform3fv(glGetUniformLocation(Program_Compute, "EmitterOrigin"), 1, glm::value_ptr(Fireworks[i].Origin));

        glUniform1i(glGetUniformLocation(Program_Compute, "SeedLife"), rand() % 10000 + 1);
        glUniform1i(glGetUniformLocation(Program_Compute, "SeedX"), rand() % 10000 + 1);
        glUniform1i(glGetUniformLocation(Program_Compute, "SeedY"), rand() % 10000 + 1);
        glUniform1i(glGetUniformLocation(Program_Compute, "SeedZ"), rand() % 10000 + 1);

        int groups = ParticlesPerFirework / WorkGroupSizeX;
        glDispatchCompute(groups, 1, 1);
    }

    if (anyActive)
    {
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
    }

    glUseProgram(0);
}

void ParticleSystem::Render()
{
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive blending for fireworks

    glUseProgram(Program_Render);

    glm::mat4 VP = ActiveCamera->GetProjectionMatrix() * ActiveCamera->GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(Program_Render, "VP"), 1, GL_FALSE, glm::value_ptr(VP));

    glBindVertexArray(VAO);

    for (int i = 0; i < MaxFireworks; ++i)
    {
        if (!Fireworks[i].Active) continue;

        glUniform3fv(glGetUniformLocation(Program_Render, "ParticleColor"), 1, glm::value_ptr(Fireworks[i].Color));

        int first = i * ParticlesPerFirework;
        glDrawArrays(GL_POINTS, first, ParticlesPerFirework);
    }

    glBindVertexArray(0);
    glUseProgram(0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}