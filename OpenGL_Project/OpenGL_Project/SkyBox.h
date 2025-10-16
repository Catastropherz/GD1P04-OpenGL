#pragma once
#include "Mesh.h"

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
	void RenderSkybox();
};

