#version 460 core

layout (location = 0) in vec4 Position; // xyz: Pos, w: Life

uniform mat4 VP;

out float FragLife;

void main()
{
    FragLife = Position.w;
    gl_PointSize = (Position.w > 0.0f) ? 4.0f : 0.0f; // Hide dead particles
    gl_Position = VP * vec4(Position.xyz, 1.0f);
}