/***********************************************************************
 Bachelor of Software Engineering
 Media Design School
 Auckland
 New Zealand
 (c)
 2024 Media Design School
 File Name : Mesh.h
 Description : Handle mesh creation, rendering, and updating
 Author : Q Sivakorn Tuangwilai
 Mail : sivakorn.tuangwilai@mds.ac.nz
 **************************************************************************/

#pragma once
#include "LightManager.h"
#include <vector>

enum MeshType
{
	TRI,
	QUAD,
	QUAD_FLIP,
	QUAD_TILE,
	HEX,
	CUBE,
	MODEL,
};

struct VertexStandard
{
public:
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;

	VertexStandard()
	{
		position = glm::vec3(0.0f);
		texcoord = glm::vec2(0.0f);
		normal = glm::vec3(0.0f);
	};

	VertexStandard(glm::vec3 _position, glm::vec2 _texcoord, glm::vec3 _normal)
	{
		position = _position;
		texcoord = _texcoord;
		normal = _normal;

	};
};

class Mesh
{
private:
	// Vertices / Indices --------------------------
	static const GLfloat Vertices_Tri[18];
	static const GLfloat Vertices_Quad[32];
	static const GLfloat Vertices_QuadFlip[32];
	static const GLfloat Vertices_QuadTile[32];
	static const GLuint Indices_Quad[6];

	static const GLfloat Vertices_Hex[42];
	static const GLuint Indices_Hex[21];
	static const GLfloat Vertices_Cube[120];
	static const GLuint Indices_Cube[36];

	GLuint VBO = 0;
	GLuint VAO = 0;
	GLuint EBO = 0;
	GLuint VBO_Instanced = 0;

	GLuint DrawCount = 0;
	int DrawType = 0;
	int Count_Instanced = 1;

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
	glm::mat4 OrthoMat = glm::mat4(0.0f);

	// Model matrix vector for instanced rendering
	std::vector<glm::mat4> modelMatInstances;


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
	int spriteSheetRow = 0;
	int spriteSheetColumn = 0;
	TextureLoader* secondTexture = nullptr;
	GLuint skyboxTextureID = 0;
	GLuint reflectMapTexture = 0;
	bool isReflective = false;

	// Camera
	glm::vec3 cameraPosition = glm::vec3(0.0f);

	// Window parameter
	float windowWidth = 800.0f;
	float windowHeight = 800.0f;

	// Light Manager
	LightManager* lightManager = nullptr;

public:

	Mesh(MeshType _type);
	Mesh(std::string _filePath, glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f), float _angleDegrees = 0.0f, int _count = 1); // For loading model
	~Mesh();

	// Set position, scale, rotation of the mesh
	void setModel(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _scale = glm::vec3(1.0f, 1.0f, 1.0f), float _angleDegrees = 0.0f);
	
	// Set the shader program to use
	void setProgram(GLuint* _program);

	// Set solid color for the mesh
	void setSolidColor(glm::vec3* _solidColor);

	// Set texture for the mesh
	void setTexture(TextureLoader* _texture);

	// Set second texture for the mesh (for mixing)
	void setSecondTexture(TextureLoader* _texture);
	
	// Set skybox texture ID
	void SetSkybox(SkyBox* _skybox, TextureLoader* _reflectMap = nullptr);

	// Set ortho
	void SetOrtho(float _windowWidth, float _windowHeight);
	
	// Render the mesh
	void Render();

	// Update uniform variables
	void Update(float _currentTime, glm::mat4 _viewMat, glm::mat4 _projectionMat, Camera* _camera);

	// Generate model matrix instances for instanced rendering
	void GenerateModelMatInstances(int _count);

	// Move
	void Move(glm::vec3 _position);

	// Toggle Switch Texture
	void ToggleTexture();

	// Check if cursor is inside
	bool checkHover(double _xpos, double _ypos);

	// Set light manager
	void SetLightManager(LightManager* _lightManager);


};

