#include "Mesh_Terrain.h"

Mesh_Terrain::Mesh_Terrain()
{
    VAO = 0;
    VBO = 0;
    EBO = 0;
    HeightMap.clear();
    Vertices.clear();
    Indices.clear();
    programToUse = 0;
    currentTime = 0.0f;
    viewMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::mat4(1.0f);
    cameraPosition = glm::vec3(0.0f);
    texture1 = nullptr;
    texture2 = nullptr;
    texture3 = nullptr;
    texture4 = nullptr;
    lightManager = nullptr;
}

Mesh_Terrain::~Mesh_Terrain()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

bool Mesh_Terrain::LoadHeightMap(const HeightMapInfo& BuildInfo)
{
    unsigned int VertexCount = BuildInfo.Width * BuildInfo.Depth;
    std::vector<unsigned char> HeightValue(VertexCount);

    std::ifstream File;
    File.open(BuildInfo.FilePath, std::ios_base::binary);
    if (File)
    {
        File.read((char*)&HeightValue[0], (std::streamsize)HeightValue.size());
        File.close();
    }
    else
    {
        std::cout << "Error: Height map failed to load: " << BuildInfo.FilePath << std::endl;
        return false;
    }

    // Transfer raw byte data into float array for expanded height manipulation
    HeightMap.resize(VertexCount, 0.0f);
    for (unsigned int i = 0; i < VertexCount; i++)
    {
        HeightMap[i] = (float)HeightValue[i];
    }

    return true;
}

float Mesh_Terrain::Average(unsigned int Row, unsigned int Col, const HeightMapInfo& BuildInfo)
{
    float totalHeight = 0.0f;
    int count = 0;

    // Check 3x3 surrounding grid
    for (int r = -1; r <= 1; ++r)
    {
        for (int c = -1; c <= 1; ++c)
        {
            int evalRow = (int)Row + r;
            int evalCol = (int)Col + c;

            // Bounds check: Row iterates Width, Col iterates Depth
            if (evalRow >= 0 && evalRow < (int)BuildInfo.Width &&
                evalCol >= 0 && evalCol < (int)BuildInfo.Depth)
            {
                totalHeight += HeightMap[evalRow * BuildInfo.Depth + evalCol];
                count++;
            }
        }
    }

    return (count > 0) ? (totalHeight / (float)count) : 0.0f;
}

void Mesh_Terrain::SmoothHeights(const HeightMapInfo& BuildInfo)
{
    std::vector<float> SmoothedMap(HeightMap.size());

    // Iterate rows (Width) then columns (Depth)
    for (unsigned int Row = 0; Row < BuildInfo.Width; Row++)
    {
        for (unsigned int Col = 0; Col < BuildInfo.Depth; Col++)
        {
            SmoothedMap[Row * BuildInfo.Depth + Col] = Average(Row, Col, BuildInfo);
        }
    }

    HeightMap = SmoothedMap;
}

void Mesh_Terrain::BuildVertexData(const HeightMapInfo& BuildInfo)
{
    unsigned int VertexCount = BuildInfo.Width * BuildInfo.Depth;
    Vertices.resize(VertexCount);

    float HalfWidth = (BuildInfo.Width - 1) * BuildInfo.CellSpacing * 0.5f;
    float HalfDepth = (BuildInfo.Depth - 1) * BuildInfo.CellSpacing * 0.5f;

    float TexU = 1.0f / (float)(BuildInfo.Width - 1);
    float TexV = 1.0f / (float)(BuildInfo.Depth - 1);

    for (unsigned int Row = 0; Row < BuildInfo.Width; Row++)
    {
        float PosZ = HalfDepth - (Row * BuildInfo.CellSpacing);

        for (unsigned int Col = 0; Col < BuildInfo.Depth; Col++)
        {
            int Index = Row * BuildInfo.Width + Col;
            //int Index = Row * BuildInfo.Depth + Col;

            float PosX = -HalfWidth + (Col * BuildInfo.CellSpacing);
            float PosY = HeightMap[Index];

            Vertices[Index].position = glm::vec3(PosX, PosY, PosZ);
            Vertices[Index].texcoord = glm::vec2(Col * TexU, Row * TexV);
            Vertices[Index].normal = glm::vec3(0.0f, 1.0f, 0.0f);
        }

}
}

void Mesh_Terrain::GenerateNormals(const HeightMapInfo& BuildInfo)
{
    // Compute normals from surrounding vertex positions so normals always match geometry
    for (unsigned int Row = 0; Row < BuildInfo.Width; Row++)
    {
        for (unsigned int Col = 0; Col < BuildInfo.Depth; Col++)
        {
            unsigned int idx = Row * BuildInfo.Depth + Col;
            glm::vec3 pos = Vertices[idx].position;

            // sample neighbor positions (mirror edges)
            glm::vec3 left  = (Row > 0) ? Vertices[(Row - 1) * BuildInfo.Depth + Col].position : pos + glm::vec3(-BuildInfo.CellSpacing, 0.0f, 0.0f);
            glm::vec3 right = (Row + 1 < BuildInfo.Width) ? Vertices[(Row + 1) * BuildInfo.Depth + Col].position : pos + glm::vec3(BuildInfo.CellSpacing, 0.0f, 0.0f);
            glm::vec3 down  = (Col > 0) ? Vertices[Row * BuildInfo.Depth + (Col - 1)].position : pos + glm::vec3(0.0f, 0.0f, -BuildInfo.CellSpacing);
            glm::vec3 up    = (Col + 1 < BuildInfo.Depth) ? Vertices[Row * BuildInfo.Depth + (Col + 1)].position : pos + glm::vec3(0.0f, 0.0f, BuildInfo.CellSpacing);

            glm::vec3 tangentX = right - left; // direction along X
            glm::vec3 tangentZ = up - down;    // direction along Z

            glm::vec3 normal = glm::cross(tangentZ, tangentX);
            if (glm::length(normal) > 1e-6f)
                normal = glm::normalize(normal);
            else
                normal = glm::vec3(0.0f, 1.0f, 0.0f);

            Vertices[idx].normal = normal;
        }
    }
}

void Mesh_Terrain::BuildIndexData(const HeightMapInfo& BuildInfo)
{
    unsigned int FaceCount = (BuildInfo.Width - 1) * (BuildInfo.Depth - 1) * 2;
    unsigned int DrawCount = FaceCount * 3;
    Indices.resize(DrawCount);

    int Index = 0;
    for (unsigned int Row = 0; Row < (BuildInfo.Width - 1); Row++)
    {
        for (unsigned int Col = 0; Col < (BuildInfo.Depth - 1); Col++)
        {
            Indices[Index++] = Row * BuildInfo.Depth + Col;
            Indices[Index++] = Row * BuildInfo.Depth + Col + 1;
            Indices[Index++] = (Row + 1) * BuildInfo.Depth + Col;

            Indices[Index++] = (Row + 1) * BuildInfo.Depth + Col;
            Indices[Index++] = Row * BuildInfo.Depth + Col + 1;
            Indices[Index++] = (Row + 1) * BuildInfo.Depth + Col + 1;
        }
    }
}

void Mesh_Terrain::SetupMesh()
{
    // Generate buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO
    glBindVertexArray(VAO);

    // Upload Vertex Data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(VertexStandard), Vertices.data(), GL_STATIC_DRAW);

    // Upload Index Data to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(GLuint), Indices.data(), GL_STATIC_DRAW);

    // Position -> location 0, TexCoord -> location 1, Normal -> location 2
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (GLvoid*)offsetof(VertexStandard, position));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (GLvoid*)offsetof(VertexStandard, texcoord));
    glEnableVertexAttribArray(1);
    
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (GLvoid*)offsetof(VertexStandard, normal));
    glEnableVertexAttribArray(2);
    // Unbind VAO
    glBindVertexArray(0);
}

void Mesh_Terrain::setModel(glm::vec3 _position, glm::vec3 _scale, float _angleDegrees)
{
    Position = _position;
    Scale = _scale;
    RotationAngle = _angleDegrees;

    glm::mat4 TranslationMat = glm::translate(glm::mat4(1.0f), Position);
    glm::mat4 RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(RotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 ScaleMat = glm::scale(glm::mat4(1.0f), Scale);

    ModelMat = TranslationMat * RotationMat * ScaleMat;
}

void Mesh_Terrain::Update(float _currentTime, glm::mat4 _viewMat, glm::mat4 _projectionMat, Camera* _camera)
{
    currentTime = _currentTime;
    viewMatrix = _viewMat;
    projectionMatrix = _projectionMat;
    if (_camera != nullptr)
    {
        cameraPosition = _camera->GetCameraPosition();
    }
}

void Mesh_Terrain::Render()
{
    if (programToUse == 0) return;

    glUseProgram(programToUse);
    glBindVertexArray(VAO);

    // Pass Matrices and Camera to Shaders
    glUniformMatrix4fv(glGetUniformLocation(programToUse, "ModelMat"), 1, GL_FALSE, glm::value_ptr(ModelMat));
    glUniformMatrix4fv(glGetUniformLocation(programToUse, "ViewMat"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(glGetUniformLocation(programToUse, "ProjectionMat"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniform3fv(glGetUniformLocation(programToUse, "CameraPos"), 1, glm::value_ptr(cameraPosition));
    glUniform1f(glGetUniformLocation(programToUse, "CurrentTime"), currentTime);

    // Apply Lights
    if (lightManager != nullptr)
    {
        lightManager->applyLightsToShader(programToUse);
    }

    // Ensure samplers in the fragment shader are set to texture units 0..3
    GLint locGrass = glGetUniformLocation(programToUse, "TextureGrass");
    if (locGrass >= 0) glUniform1i(locGrass, 0);
    GLint locDirt = glGetUniformLocation(programToUse, "TextureDirt");
    if (locDirt >= 0) glUniform1i(locDirt, 1);
    GLint locStone = glGetUniformLocation(programToUse, "TextureStone");
    if (locStone >= 0) glUniform1i(locStone, 2);
    GLint locSnow = glGetUniformLocation(programToUse, "TextureSnow");
    if (locSnow >= 0) glUniform1i(locSnow, 3);

    // Bind Textures
    if (texture1 != nullptr)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
    }
    if (texture2 != nullptr)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
	}
    if (texture3 != nullptr)
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3->GetTextureID());
    }
    if (texture4 != nullptr)
    {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, texture4->GetTextureID());
	}

    // Draw Terrain
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}