#version 460 core

//Input from vertex shader
in vec3 FragTexCoords;

// Uniform Inputs
uniform samplerCube Texture_Skybox;

// Output
out vec4 FinalColor;

void main()
{
	FinalColor = texture(Texture_Skybox, FragTexCoords);
}