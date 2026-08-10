#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Mesh.h"
#include "TextureLoader.h"
#include "LightManager.h"
#include "Camera.h"

struct HeightMapInfo
{
    std::string FilePath = "";
    unsigned int Width = 0;   // Columns
    unsigned int Depth = 0;   // Rows
    float CellSpacing = 1.0f; // Unit translation spacing
};

class Mesh_Terrain
{
public:
    Mesh_Terrain();
	~Mesh_Terrain();

    // Heightmap generation pipeline
    bool LoadHeightMap(const HeightMapInfo& BuildInfo);
    void SmoothHeights(const HeightMapInfo& BuildInfo);
    float Average(unsigned int Row, unsigned int Col, const HeightMapInfo& BuildInfo);
    void BuildVertexData(const HeightMapInfo& BuildInfo);
    void GenerateNormals(const HeightMapInfo& BuildInfo);
    void BuildIndexData(const HeightMapInfo& BuildInfo);
    void SetupMesh();

    // Matching Mesh.h setter functions
    void setProgram(GLuint* _program) { programToUse = *_program; }
    void setTexture1(TextureLoader* _texture) { texture1 = _texture; }
    void setTexture2(TextureLoader* _texture) { texture2 = _texture; }
    void setTexture3(TextureLoader* _texture) { texture3 = _texture; }
    void setTexture4(TextureLoader* _texture) { texture4 = _texture; }
    void setModel(glm::vec3 _position = glm::vec3(0.0f), glm::vec3 _scale = glm::vec3(1.0f), float _angleDegrees = 0.0f);
    void SetLightManager(LightManager* _lightManager) { lightManager = _lightManager; }

    // Core loops
    void Update(float _currentTime, glm::mat4 _viewMat, glm::mat4 _projectionMat, Camera* _camera);
    void Render();

private:
    std::vector<float> HeightMap;
    std::vector<VertexStandard> Vertices;
    std::vector<GLuint> Indices;

    // OpenGL Buffer handles
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;

    // Transformations
    glm::vec3 Position = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f);
    float RotationAngle = 0.0f;
    glm::mat4 ModelMat = glm::mat4(1.0f);

    // Uniform Variables
    GLuint programToUse = 0;
    float currentTime = 0.0f;
    glm::mat4 viewMatrix = glm::mat4(1.0f);
    glm::mat4 projectionMatrix = glm::mat4(1.0f);
    glm::vec3 cameraPosition = glm::vec3(0.0f);

    // Textures
    TextureLoader* texture1 = nullptr;
    TextureLoader* texture2 = nullptr;
	TextureLoader* texture3 = nullptr;
	TextureLoader* texture4 = nullptr;

    // Lighting
    LightManager* lightManager = nullptr;
};

