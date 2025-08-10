#version 460 core

//Vertex Data Interpretation
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;

// Inputs
uniform mat4 TranslationMat;
uniform mat4 RotationMat;
uniform mat4 ScaleMat;
uniform float CurrentTime;

// Outputs to Fragment Shader
out vec3 FragColor;

//Shader Functionality
void main()
{
	// Oscillating between 50% scale and 150% scale
	mat4 ScaleVar = ScaleMat * mat4(
		1.0f + 0.5f * sin(CurrentTime), 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f + 0.5f * sin(CurrentTime), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f + 0.5f * sin(CurrentTime), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	
	// Apply transformations to the vertex position
	gl_Position = TranslationMat * RotationMat * ScaleVar * vec4(Position, 1.0f);
	FragColor = Color;
}