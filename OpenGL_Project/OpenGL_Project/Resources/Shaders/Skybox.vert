#version 460 core

//Vertex Data Interpretation
layout (location = 0) in vec3 Position;

// Inputs
uniform mat4 VP; // Combined View and Projection matrix

// Outputs to Fragment Shader
out vec3 FragTexCoords;

//Shader Functionality
void main()
{
	vec4 Pos = VP * vec4(Position, 1.0);
	gl_Position = Pos.xyww;
	FragTexCoords = vec3(Position.xyz);
}