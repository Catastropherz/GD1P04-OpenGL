#pragma once
#include <glew.h>

class ScreenQuad
{
private:
    GLuint VAO = 0;
    GLuint VBO = 0;

public:
    ScreenQuad();
    ~ScreenQuad();

    void Render(GLuint program, GLuint fboTextureID, int effectMode, float currentTime, float width, float height);
};