#include "ScreenQuad.h"
#include <gtc/type_ptr.hpp>

ScreenQuad::ScreenQuad()
{
    // Fullscreen Quad vertices (Positions + TexCoords)
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

    // Bind FBO texture to slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboTextureID);

    GLint locTexture = glGetUniformLocation(program, "screenTexture");
    if (locTexture != -1) glUniform1i(locTexture, 0);

    GLint locMode = glGetUniformLocation(program, "effectMode");
    if (locMode != -1) glUniform1i(locMode, effectMode);

    GLint locTime = glGetUniformLocation(program, "iTime");
    if (locTime != -1) glUniform1f(locTime, currentTime);

    GLint locRes = glGetUniformLocation(program, "iResolution");
    if (locRes != -1) glUniform2f(locRes, width, height);

    // Render Quad
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glUseProgram(0);
}