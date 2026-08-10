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
    unsigned int Width = 0;   // Number of samples along X (row count)
    unsigned int Depth = 0;   // Number of samples along Z (column count)
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

    // Debug helpers
    void DumpDebug(unsigned int vertexCount, unsigned int indexCount)
    {
        unsigned int vc = (vertexCount < Vertices.size()) ? vertexCount : (unsigned int)Vertices.size();
        unsigned int ic = (indexCount < Indices.size()) ? indexCount : (unsigned int)Indices.size();

        std::cout << "Mesh_Terrain Debug Dump:\n";
        std::cout << "Total Vertices: " << Vertices.size() << "  Total Indices: " << Indices.size() << "\n";

        std::cout << "Vertices (first " << vc << "):\n";
        for (unsigned int i = 0; i < vc; ++i)
        {
            const VertexStandard& v = Vertices[i];
            std::cout << "[" << i << "] pos=(" << v.position.x << ", " << v.position.y << ", " << v.position.z << ")"
                << " tex=(" << v.texcoord.x << ", " << v.texcoord.y << ")"
                << " normal=(" << v.normal.x << ", " << v.normal.y << ", " << v.normal.z << ")\n";
        }

        std::cout << "Indices (first " << ic << "):\n";
        if (ic == 0)
        {
            std::cout << "(none)\n";
            return;
        }

        // Print indices as triangles where possible
        unsigned int printed = 0;
        for (unsigned int i = 0; i < ic; i += 3)
        {
            if (i + 2 < ic)
            {
                std::cout << "tri " << (i / 3) << ": " << Indices[i] << ", " << Indices[i + 1] << ", " << Indices[i + 2] << "\n";
                printed += 3;
            }
            else
            {
                // print remaining indices
                for (unsigned int j = i; j < ic; ++j)
                {
                    std::cout << Indices[j] << ((j + 1 < ic) ? ", " : "\n");
                    printed++;
                }
            }
            if (printed >= ic) break;
        }
    }

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

