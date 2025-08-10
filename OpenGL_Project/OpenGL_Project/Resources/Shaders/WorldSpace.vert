#version 460 core

//Vertex Data Interpretation
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

// Inputs
uniform mat4 TranslationMat;
uniform mat4 RotationMat;
uniform mat4 ScaleMat;

// Outputs to Fragment Shader
out vec3 FragColor;

//Shader Functionality
void main()
{
	gl_Position = TranslationMat * RotationMat * ScaleMat * vec4(Position, 1.0f);
	FragColor = Color;
}