/***********************************************************************
 Bachelor of Software Engineering
 Media Design School
 Auckland
 New Zealand
 (c)
 2024 Media Design School
 File Name : SkyBox.h
 Description : Handle skybox creation
 Author : Q Sivakorn Tuangwilai
 Mail : sivakorn.tuangwilai@mds.ac.nz
 **************************************************************************/

#pragma once
#include "TextureLoader.h"
#include "Camera.h"

class SkyBox
{
private:
	Camera* camera = nullptr;

	static const GLfloat Vertices_Cube[72];
	static const GLuint Indices_Cube[36];

	// Generate TextureID for cubemap
	GLuint textureSkybox = 0;

	// Shader program
	GLuint programToUse = ShaderLoader::CreateProgram(	"Resources/Shaders/Skybox.vert",
														"Resources/Shaders/Skybox.frag"	);

	// VAO, VBO, EBO
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;

public:

	SkyBox(Camera* _camera, std::string* _filePath);
	~SkyBox();

	// Render the skybox
	void RenderSkybox();

	// Getter function
	GLuint GetTextureID() const { return textureSkybox; }

};

