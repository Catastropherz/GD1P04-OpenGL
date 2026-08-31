#version 460 core

// Fullscreen Quad Inputs
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;

out vec2 FragTexCoords;

void main()
{
    FragTexCoords = TexCoords;
    gl_Position = vec4(Position, 1.0f);
}