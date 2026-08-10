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

    // Rows iterate over Depth, Columns iterate over Width
    for (unsigned int Row = 0; Row < BuildInfo.Depth; Row++)
    {
        float PosZ = HalfDepth - (Row * BuildInfo.CellSpacing);

        for (unsigned int Col = 0; Col < BuildInfo.Width; Col++)
        {
            int Index = Row * BuildInfo.Width + Col;
            float PosX = -HalfWidth + (Col * BuildInfo.CellSpacing);
            // Apply height scale to control vertical exaggeration
            float PosY = HeightMap[Index] * BuildInfo.HeightScale;

            Vertices[Index].position = glm::vec3(PosX, PosY, PosZ);
            Vertices[Index].texcoord = glm::vec2(Col * TexU, Row * TexV);
            Vertices[Index].normal = glm::vec3(0.0f, 1.0f, 0.0f);
        }

}
}

void Mesh_Terrain::GenerateNormals(const HeightMapInfo& BuildInfo)
{
    float InvCellSpacing = 1.0f / (2.0f * BuildInfo.CellSpacing);

    for (unsigned int Row = 0; Row < BuildInfo.Width; Row++)
    {
        for (unsigned int Col = 0; Col < BuildInfo.Depth; Col++)
        {
            float RowNeg = HeightMap[(Row == 0 ? Row : Row - 1) * BuildInfo.Depth + Col];
            float RowPos = HeightMap[(Row < BuildInfo.Width - 1 ? Row + 1 : Row) * BuildInfo.Depth + Col];
            float ColNeg = HeightMap[Row * BuildInfo.Depth + (Col == 0 ? Col : Col - 1)];
            float ColPos = HeightMap[Row * BuildInfo.Depth + (Col < BuildInfo.Depth - 1 ? Col +1 : Col)];

            float X = (RowNeg - RowPos);
            if (Row == 0 || Row == BuildInfo.Width - 1) 
            {
                X *= 2.0f;
            }

            float Y = (ColPos - ColNeg);
            if (Col == 0 || Col == BuildInfo.Depth - 1) 
            {
                Y *= 2.0f;
            }

            glm::vec3 TangentZ(0.0f, X * InvCellSpacing, 1.0f);
            glm::vec3 TangentX(1.0f, Y * InvCellSpacing, 0.0f);

            glm::vec3 Normal = glm::cross(TangentZ, TangentX);
            Normal = glm::normalize(Normal);

            Vertices[Row * BuildInfo.Depth + Col].normal = Normal;
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
            unsigned int v0 = Row * BuildInfo.Depth + Col;
            unsigned int v1 = Row * BuildInfo.Depth + (Col + 1);
            unsigned int v2 = (Row + 1) * BuildInfo.Depth + Col;
            unsigned int v3 = (Row + 1) * BuildInfo.Depth + (Col + 1);

            // Triangle 1: v0, v1, v2
            Indices[Index++] = v0;
            Indices[Index++] = v1;
            Indices[Index++] = v2;

            // Triangle 2: v2, v1, v3
            Indices[Index++] = v2;
            Indices[Index++] = v1;
            Indices[Index++] = v3;
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
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (void*)offsetof(VertexStandard, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (void*)offsetof(VertexStandard, texcoord));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (void*)offsetof(VertexStandard, normal));

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