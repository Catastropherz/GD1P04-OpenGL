#version 460 core

//Vertex Data Interpretation
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec2 TexCoords;

// Inputs
uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjectionMat;
uniform int FrameIndex;
uniform int FrameCount;
uniform int SpriteSheetRow;
uniform int SpriteSheetColumn;

// Outputs to Fragment Shader
out vec3 FragColor;
out vec2 FragTexCoords;

//Shader Functionality
void main()
{
	gl_Position = ProjectionMat * ViewMat * ModelMat * vec4(Position, 1.0);
	FragColor = Color;

	//Animation logic for sprite sheet
	float frameHeight = 1.0f / float(SpriteSheetRow);
	float frameWidth = 1.0f / float(SpriteSheetColumn);
	vec2 frame = TexCoords;
	frame.x = (float(FrameIndex%(FrameCount/SpriteSheetRow)) + TexCoords.x) * frameWidth;
	frame.y = (float((FrameCount/SpriteSheetRow) - FrameIndex/(FrameCount/SpriteSheetRow) - 1) + TexCoords.y) * frameHeight;

	FragTexCoords = frame;
}