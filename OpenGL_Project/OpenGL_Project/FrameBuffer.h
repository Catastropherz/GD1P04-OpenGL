#pragma once
#include <glew.h>
#include <iostream>

class Framebuffer
{
private:
    GLuint FBO = 0;
    GLuint RenderTexture = 0;
    GLuint RBO = 0;
    int Width = 0;
    int Height = 0;

public:
    Framebuffer(int width, int height);
    ~Framebuffer();

    void Bind();
    void Unbind();

    GLuint GetTextureID() const { return RenderTexture; }
};