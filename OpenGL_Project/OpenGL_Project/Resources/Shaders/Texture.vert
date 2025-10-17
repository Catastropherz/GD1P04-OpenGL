#version 460 core

//Vertex Data Interpretation
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec2 TexCoords;
layout (location = 3) in vec3 Normal;

// Inputs
uniform mat4 ModelMat;
uniform mat4 ProjectionMat;
uniform mat4 ViewMat;

// Outputs to Fragment Shader
out vec3 FragColor;
out vec2 FragTexCoords;
out vec3 FragNormal;
out vec3 FragPos;


//Shader Functionality
void main()
{
	gl_Position = ProjectionMat * ViewMat * ModelMat * vec4(Position, 1.0);
	FragColor = Color;
	FragTexCoords = TexCoords;
	FragNormal = mat3(transpose(inverse(ModelMat))) * Normal;
	FragPos = vec3(ModelMat * vec4(Position, 1.0));
}