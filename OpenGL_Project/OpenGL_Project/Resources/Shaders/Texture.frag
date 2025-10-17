#version 460 core

//Input from vertex shader
in vec2 FragTexCoords;

// Uniform Inputs
uniform sampler2D Texture0;

// Output
out vec4 FinalColor;

void main()
{
	FinalColor = texture(Texture0, FragTexCoords);
}