#include "ScreenQuad.h"
#include <gtc/type_ptr.hpp>

ScreenQuad::ScreenQuad()
{
    // Fullscreen NDC Quad vertices (Positions + TexCoords)
    float quadVertices[] = {
        // Positions   // TexCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

ScreenQuad::~ScreenQuad()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void ScreenQuad::Render(GLuint program, GLuint fboTextureID, int effectMode, float currentTime, float width, float height)
{
    glUseProgram(program);

    // Bind FBO texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboTextureID);
    glUniform1i(glGetUniformLocation(program, "screenTexture"), 0);

    // Uniforms for ShaderToy post-processing effects
    glUniform1i(glGetUniformLocation(program, "effectMode"), effectMode);
    glUniform1f(glGetUniformLocation(program, "iTime"), currentTime);
    glUniform2f(glGetUniformLocation(program, "iResolution"), width, height);

    // Render Quad
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glUseProgram(0);
}