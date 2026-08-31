#version 460 core

layout (location = 0) in vec4 Position; // xyz: Position, w: Remaining Life

uniform mat4 VP; // View-Projection Matrix

out float FragLife;

void main()
{
    FragLife = Position.w; // Pass remaining life to fragment shader for fading
    gl_PointSize = 3.0f;    // Set particle point rendering size
    gl_Position = VP * vec4(Position.xyz, 1.0f);
}