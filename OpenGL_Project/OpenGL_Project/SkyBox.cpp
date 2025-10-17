#include "SkyBox.h"
#include <stb_image.h>

const GLfloat SkyBox::Vertices_Cube[72] = {
	// Index	// Position				
	// Front Face
	/* 00 */	-1.0f, 1.0f, 1.0f,		
	/* 01 */	-1.0f, -1.0f, 1.0f,		
	/* 02 */	1.0f, -1.0f, 1.0f,		
	/* 03 */	1.0f, 1.0f, 1.0f,		
	// Back Face
	/* 04 */	1.0f, 1.0f, -1.0f,		
	/* 05 */	1.0f, -1.0f, -1.0f,		
	/* 06 */	-1.0f, -1.0f, -1.0f,	
	/* 07 */	-1.0f, 1.0f, -1.0f,		
	// Right Face
	/* 08 */	1.0f, 1.0f, 1.0f,		
	/* 09 */	1.0f, -1.0f, 1.0f,		
	/* 10 */	1.0f, -1.0f, -1.0f,		
	/* 11 */	1.0f, 1.0f, -1.0f,		
	// Left Face
	/* 12 */	-1.0f, 1.0f, -1.0f,		
	/* 13 */	-1.0f, -1.0f, -1.0f,	
	/* 14 */	-1.0f, -1.0f, 1.0f,		
	/* 15 */	-1.0f, 1.0f, 1.0f,		
	// Top Face
	/* 16 */	-1.0f, 1.0f, -1.0f,		
	/* 17 */	-1.0f, 1.0f, 1.0f,		
	/* 18 */	1.0f, 1.0f, 1.0f,		
	/* 19 */	1.0f, 1.0f, -1.0f,		
	// Bottom Face
	/* 20 */	-1.0f, -1.0f, 1.0f,		
	/* 21 */	-1.0f, -1.0f, -1.0f,	
	/* 22 */	1.0f, -1.0f, -1.0f,		
	/* 23 */	1.0f, -1.0f, 1.0f,		
};

const GLuint SkyBox::Indices_Cube[36] = {
		8,	10,	9, // Right Face
		8,	11, 10,
		12, 14, 13, // Left Face
		12, 15, 14,
		16, 18, 17, // Top Face
		16, 19, 18,
		20, 22, 21, // Bottom Face
		20, 23, 22,
		0,	2,	1,	// Front Face
		0,	3,	2,
		4,	6,	5,	// Back Face
		4,	7,	6,
};


	//GL_TEXTURE_CUBE_MAP_POSITIVE_X // Right
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_X // Left
	//GL_TEXTURE_CUBE_MAP_POSITIVE_Y // Top
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y // Bottom
	//GL_TEXTURE_CUBE_MAP_POSITIVE_Z // Back
	//GL_TEXTURE_CUBE_MAP_NEGATIVE_Z // Front

SkyBox::SkyBox(Camera* _camera, std::string* _filePath)
{
	camera = _camera;

	// Generate VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Cube), Indices_Cube, GL_STATIC_DRAW);

	//Generate VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Bind vertices
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Cube), Vertices_Cube, GL_STATIC_DRAW); //Cube
	// Set the vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture setup
	glGenTextures(1, &textureSkybox);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkybox);
	stbi_set_flip_vertically_on_load(false);

	// Storage variables
	int ImageWidth, ImageHeight, ImageComponents;

	for (int i = 0; i < 6; i++)
	{
		// Load each image separately
		unsigned char* ImageData = stbi_load(_filePath[i].c_str(), &ImageWidth, &ImageHeight, &ImageComponents, 0);

		// Cubemap should be full color (RGB). Image might come with Alpha (RGBA)
		GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

		// Populate the cubemap texture with the image data
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
			0, LoadedComponents, ImageWidth, ImageHeight, 
			0, LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

		// Free each iamge data after creating the OpenGL texture
		stbi_image_free(ImageData);
	}

	// Set the texture parameters for the cubemap
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate the mipmaps. free the memory, and unbind the texture
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

SkyBox::~SkyBox()
{
}

void SkyBox::RenderSkybox()
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);

	glUseProgram(programToUse);

	// Bind the Skybox texture as a cube map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureSkybox);
	glUniform1i(glGetUniformLocation(programToUse, "Texture_Skybox"), 0);

	// Set up the camera matrices
	glm::mat4 camMatView = glm::mat4(glm::mat3(camera->GetViewMatrix()));
	glm::mat4 camMatProj = camera->GetProjectionMatrix();
	glUniformMatrix4fv(glGetUniformLocation(programToUse, "VP"), 1, GL_FALSE, glm::value_ptr(camMatProj * camMatView));

	// Render the skybox mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	// Unbind all objects
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}


