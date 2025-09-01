#pragma once
#include "TextureLoader.h"
#include "Camera.h"

enum MeshType
{
	TRI,
	QUAD,
	HEX,
	CUBE
};

class Mesh
{
private:
	// Vertices / Indices --------------------------
	static const GLfloat Vertices_Tri[18];
	static const GLfloat Vertices_Quad[32];
	static const GLuint Indices_Quad[6];

	static const GLfloat Vertices_Hex[42];
	static const GLuint Indices_Hex[21];
	static const GLfloat Vertices_Cube[120];
	static const GLuint Indices_Cube[36];

	GLuint VBO = 0;
	GLuint VAO = 0;
	GLuint EBO_Quad = 0;
	GLuint EBO_Hex = 0;
	GLuint EBO_Cube = 0;

	MeshType type;

	// Mesh componenets
		// Translation, Rotation, Scale
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	float RotationAngle = 0.0f; // Degrees

	// Model Matrix
	glm::mat4 TranslationMat = glm::mat4(0.0f);
	glm::mat4 RotationMat = glm::mat4(0.0f);
	glm::mat4 ScaleMat = glm::mat4(0.0f);
	glm::mat4 ModelMat = glm::mat4(0.0f);


	// Uniform variables
	GLuint programToUse = 0;
	float currentTime = 0.0f;
	glm::vec3 solidColor = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 viewMatrix = glm::mat4(0.0f);
	glm::mat4 projectionMatrix = glm::mat4(0.0f);
	
	// Texture
	TextureLoader* texture = nullptr;
	GLuint textureID = 0;
	int frameIndex = 0;
	int frameCount = 0;

public:

	Mesh(MeshType _type);
	~Mesh();

	// Set position, scale, rotation of the mesh
	void setModel(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f), float _angleDegrees = 0.0f);
	
	void setProgram(GLuint* _program);
	void setSolidColor(glm::vec3* _solidColor);
	void setTexture(TextureLoader* _texture);

	void Render();
	void Update(float _currentTime, glm::mat4 _viewMat, glm::mat4 _projectionMat);

};

