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
	glm::mat4 TranslationMat;
	glm::mat4 RotationMat;
	glm::mat4 ScaleMat;
	glm::mat4 ModelMat;


	// Uniform variables
	GLuint programToUse;
	float currentTime = 0.0f;
	glm::vec3 solidColor;
	int frameIndex = 0;
	int frameCount = 0;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	GLuint textureID;

public:

	Mesh(MeshType _type);
	~Mesh();

	// Set position, scale, rotation of the mesh
	void setModel(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f), float _angleDegrees = 0.0f);
	
	void setProgram(GLuint* _program);
	void setCurrentTime(float* _currentTime);
	void setSolidColor(glm::vec3* _solidColor);
	void setFrameIndex(int _frameIndex);
	void setFrameCount(int _frameCount);
	void setViewMatrix(glm::mat4 _viewMat);
	void setProjectionMatrix(glm::mat4 _projectionMat);
	void setTextureID(GLuint _textureID);

	void Render();
	void Update();

};

