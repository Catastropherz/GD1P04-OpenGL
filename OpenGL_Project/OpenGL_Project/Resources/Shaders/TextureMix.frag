#version 460 core

//Input from vertex shader
in vec3 FragColor;
in vec2 FragTexCoords;

// Uniform Inputs
uniform float CurrentTime;
uniform sampler2D Texture0;
uniform sampler2D Texture1;

// Output
out vec4 FinalColor;

void main()
{
	float blend = 0.5 * sin(CurrentTime) + 0.5;
	FinalColor = mix(texture(Texture0, FragTexCoords), texture(Texture1, FragTexCoords), blend);
}