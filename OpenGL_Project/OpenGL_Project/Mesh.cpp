// Library Defines
#define TINYOBJLOADER_IMPLEMENTATION

// Library Includes
#include <tiny_obj_loader.h>
#include "Mesh.h"

const GLfloat Mesh::Vertices_Tri[18] = {
	// Position			// Color
	0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	// Top Right
	-0.5f, 0.8f, 0.0f,	0.0f, 1.0f, 0.0f,	// Top Left
	0.5f, 0.8f, 0.0f,	0.0f, 0.0f, 1.0f,	// Bottom Center
};

const GLfloat Mesh::Vertices_Quad[32] = {
	// Index	// Position			// Color			// Texture Coords
	/*0*/		-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,		// Top Left
	/*1*/		-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// Btm Left
	/*2*/		0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,		// Btm Right
	/*3*/		0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 1.0f,	1.0f, 1.0f,		// Top Right
};

const GLfloat Mesh::Vertices_QuadFlip[32] = {
	// Index	// Position			// Color			// Texture Coords
	/*0*/		-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		// Top Left
	/*1*/		-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,		// Btm Left
	/*2*/		0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		// Btm Right
	/*3*/		0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 1.0f,	0.0f, 1.0f,		// Top Right
};

const GLfloat Mesh::Vertices_QuadTile[32] = {
	// Index	// Position			// Color			// Texture Coords
	/*0*/		-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	-1.0f, 1.0f,		// Top Left
	/*1*/		-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	-1.0f, -1.0f,		// Btm Left
	/*2*/		0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, -1.0f,		// Btm Right
	/*3*/		0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 1.0f,	1.0f, 1.0f,		// Top Right
};

const GLuint Mesh::Indices_Quad[6] = {
		0, 1, 2, // First Triangle (TL > BL > BR)
		0, 2, 3, // Second Triangle (TL > BR > TR)
};

const GLfloat Mesh::Vertices_Hex[42] = {
	// Index	// Position				// Color
	/*0*/		-0.5f, 0.866f, 0.0f,	1.0f, 0.0f, 1.0f,	// Top Left
	/*1*/		-1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	// Left
	/*2*/		0.0f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,	// Center
	/*3*/		-0.5f, -0.866f, 0.0f,	0.0f, 1.0f, 1.0f,	// Bottom Left
	/*4*/		0.5f, -0.866f, 0.0f,	0.0f, 1.0f, 0.0f,	// Bottom Right
	/*5*/		1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,	// Right
	/*6*/		0.5f, 0.866f, 0.0f,		1.0f, 0.0f, 0.0f,	// Top Right
};

const GLuint Mesh::Indices_Hex[21] = {
		0, 1, 2, // First Triangle (TL > L > C)
		1, 3, 2, // Second Triangle (L > BL > C)
		3, 4, 2, // Third Triangle (BL > BR > C)
		4, 5, 2, // Fourth Triangle (BR > R > C)
		5, 6, 2, // Fifth Triangle (R > TR > C)
		6, 0, 2, // Sixth Triangle (TR > TL > C)
};

const GLfloat Mesh::Vertices_Cube[120] = {
	// Index	// Position				// Tex Coords	// Position Index
	// Front Face
	/* 00 */	-0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		// 00
	/* 01 */	-0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		// 01
	/* 02 */	0.5f, -0.5f, 0.5f,		1.0f, 0.0f,		// 02
	/* 03 */	0.5f, 0.5f, 0.5f,		1.0f, 1.0f,		// 03
	// Back Face
	/* 04 */	0.5f, 0.5f, -0.5f,		0.0f, 1.0f,		// 04
	/* 05 */	0.5f, -0.5f, -0.5f,		0.0f, 0.0f,		// 05
	/* 06 */	-0.5f, -0.5f, -0.5f,	1.0f, 0.0f,		// 06
	/* 07 */	-0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		// 07
	// Right Face
	/* 08 */	0.5f, 0.5f, 0.5f,		0.0f, 1.0f,		// 03
	/* 09 */	0.5f, -0.5f, 0.5f,		0.0f, 0.0f,		// 02
	/* 10 */	0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		// 05
	/* 11 */	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		// 04
	// Left Face
	/* 12 */	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,		// 07
	/* 13 */	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		// 06
	/* 14 */	-0.5f, -0.5f, 0.5f,		1.0f, 0.0f,		// 01
	/* 15 */	-0.5f, 0.5f, 0.5f,		1.0f, 1.0f,		// 00
	// Top Face
	/* 16 */	-0.5f, 0.5f, -0.5f,		0.0f, 1.0f,		// 07
	/* 17 */	-0.5f, 0.5f, 0.5f,		0.0f, 0.0f,		// 00
	/* 18 */	0.5f, 0.5f, 0.5f,		1.0f, 0.0f,		// 03
	/* 19 */	0.5f, 0.5f, -0.5f,		1.0f, 1.0f,		// 04
	// Bottom Face
	/* 20 */	-0.5f, -0.5f, 0.5f,		0.0f, 1.0f,		// 01
	/* 21 */	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,		// 06
	/* 22 */	0.5f, -0.5f, -0.5f,		1.0f, 0.0f,		// 05
	/* 23 */	0.5f, -0.5f, 0.5f,		1.0f, 1.0f,		// 02
};

const GLuint Mesh::Indices_Cube[36] = {
		0,	1,	2,	// Front Face
		0,	2,	3,
		4,	5,	6,	// Back Face
		4,	6,	7,
		8,	9,	10, // Right Face
		8,	10, 11,
		12, 13, 14, // Left Face
		12, 14, 15,
		16, 17, 18, // Top Face
		16, 18, 19,
		20, 21, 22, // Bottom Face
		20, 22, 23,
};

Mesh::Mesh(MeshType _type)
{
	type = _type;

	//Generate VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	//Generate EBO
	switch (type)
	{
		case QUAD:
		case QUAD_FLIP:
		case QUAD_TILE:
		{
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Quad), Indices_Quad, GL_STATIC_DRAW);
			break;
		}
		case HEX:
		{
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Hex), Indices_Hex, GL_STATIC_DRAW);
			break;
		}
		case CUBE:
		{
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Cube), Indices_Cube, GL_STATIC_DRAW);
			break;
		}
	}

	//Generate VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Bind vertices based on mesh type
	switch (type)
	{
		case TRI:
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Tri), Vertices_Tri, GL_STATIC_DRAW); //Triangle
			// Set the vertex attribute pointers
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			break;
		}
		case QUAD:
		case QUAD_FLIP:
		case QUAD_TILE:
		{
			if (type == QUAD)
				glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Quad), Vertices_Quad, GL_STATIC_DRAW);
			else if (type == QUAD_FLIP)
				glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_QuadFlip), Vertices_QuadFlip, GL_STATIC_DRAW);
			else if (type == QUAD_TILE)
				glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_QuadTile), Vertices_QuadTile, GL_STATIC_DRAW);

			// Set the vertex attribute pointers
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
			break;
		}
		case HEX:
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Hex), Vertices_Hex, GL_STATIC_DRAW); //Hexagon
			// Set the vertex attribute pointers
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			break;
		}
		case CUBE:
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Cube), Vertices_Cube, GL_STATIC_DRAW); //Cube
			// Set the vertex attribute pointers
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			break;
		}
	}
}

// For loading model from file
Mesh::Mesh(std::string _filePath, glm::vec3 _position, glm::vec3 _scale, float _angleDegrees, int _count)
{
	type = MODEL;

	setModel(_position, _scale, _angleDegrees);

	std::vector<VertexStandard> Vertices;
	tinyobj::ObjReaderConfig ReaderConfig;
	tinyobj::ObjReader Reader;

	// Load model using tinyobjloader
	if (!Reader.ParseFromFile(_filePath, ReaderConfig))
	{
		if (!Reader.Error().empty())
		{
			std::cerr << "TinyObjReader: " << Reader.Error();
		}
		exit(1);
	}

	if (!Reader.Warning().empty())
	{
		std::cout << "TinyObjReader: " << Reader.Warning();
	}

	auto& Attrib = Reader.GetAttrib();
	auto& Shapes = Reader.GetShapes();

	// Loop through the shapes of the object
	for (size_t ShapeIndex = 0; ShapeIndex < Shapes.size(); ShapeIndex++)
	{
		// Loop through the faces(polygon)
		size_t ReadIndexOffset = 0;
		for (size_t FaceIndex = 0; FaceIndex < Shapes[ShapeIndex].mesh.num_face_vertices.size(); FaceIndex++)
		{
			size_t FaceVertexCount = Shapes[ShapeIndex].mesh.num_face_vertices[FaceIndex];
			// Loop through the vertices of the face
			for (size_t VertexIndex = 0; VertexIndex < FaceVertexCount; VertexIndex++)
			{
				VertexStandard Vertex{};
				tinyobj::index_t TinyObjVertex = Shapes[ShapeIndex].mesh.indices[ReadIndexOffset + VertexIndex];
				Vertex.position = glm::vec3(
					Attrib.vertices[3 * TinyObjVertex.vertex_index + 0],
					Attrib.vertices[3 * TinyObjVertex.vertex_index + 1],
					Attrib.vertices[3 * TinyObjVertex.vertex_index + 2]
				);
				if (TinyObjVertex.texcoord_index >= 0) // Check if texcoord index is valid
				{
					Vertex.texcoord = glm::vec2(
						Attrib.texcoords[2 * TinyObjVertex.texcoord_index + 0],
						Attrib.texcoords[2 * TinyObjVertex.texcoord_index + 1]
					);
				}
				if (TinyObjVertex.normal_index >= 0) // negative value no normal data
				{
					Vertex.normal = glm::vec3(
						Attrib.normals[3 * TinyObjVertex.normal_index + 0],
						Attrib.normals[3 * TinyObjVertex.normal_index + 1],
						Attrib.normals[3 * TinyObjVertex.normal_index + 2]
					);
				}
				
				Vertices.push_back(Vertex);
			}
			ReadIndexOffset += FaceVertexCount;
		}
	}

	// Generate instanced matrix
	if (_count > 1) GenerateModelMatInstances(_count); // Default to 1 instance

	// Store for use iin Rendering
	DrawType = GL_TRIANGLES;
	DrawCount = (GLuint)Vertices.size();

	// Create the Vertex Array and associated buffers
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(VertexStandard), Vertices.data(), GL_STATIC_DRAW);
	
	// Create the VertexAttributePointers for both Position and TexCoord
	// Keep in mind the new VertexStandard
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (GLvoid*)offsetof(VertexStandard, position));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (GLvoid*)offsetof(VertexStandard, texcoord));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (GLvoid*)offsetof(VertexStandard, normal));
	glEnableVertexAttribArray(3);

	// If more than 1 instance, set up instanced rendering
	if (_count > 1)
	{
		// Generate the instance VBO
		glGenBuffers(1, &VBO_Instanced);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Instanced);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * Count_Instanced, modelMatInstances.data(), GL_STATIC_DRAW);
		// Set the vertex attribute pointers for the matrix (4 vec4)
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(4);
		glEnableVertexAttribArray(5);
		glEnableVertexAttribArray(6);
		glEnableVertexAttribArray(7);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
	}
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::setModel(glm::vec3 _position, glm::vec3 _scale, float _angleDegrees)
{
	Position = _position;
	Scale = _scale;
	RotationAngle = _angleDegrees;

	// Calculate the Model Matrix
	TranslationMat = glm::translate(glm::mat4(1.0f), Position);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(RotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), Scale);

	ModelMat = TranslationMat * RotationMat * ScaleMat;

	// Push back default model matrix for instanced rendering
	modelMatInstances.push_back(ModelMat);
}

void Mesh::setProgram(GLuint* _program)
{
	programToUse = *_program;
}

void Mesh::setSolidColor(glm::vec3* _solidColor)
{
	solidColor = *_solidColor;
}

void Mesh::setTexture(TextureLoader* _texture)
{
	texture = _texture;
}

void Mesh::setSecondTexture(TextureLoader* _texture)
{
	secondTexture = _texture;
}

void Mesh::Render()
{
	// Bind the Program
	glUseProgram(programToUse);

	// Bind the VAO
	glBindVertexArray(this->VAO);

	// Send variables to the shaders via Uniform
	glUniform1f(glGetUniformLocation(programToUse, "CurrentTime"), currentTime);
	glUniform3fv(glGetUniformLocation(programToUse, "SolidColor"), 1, glm::value_ptr(solidColor));
	glUniform1i(glGetUniformLocation(programToUse, "FrameIndex"), frameIndex);
	glUniform1i(glGetUniformLocation(programToUse, "FrameCount"), frameCount);
	glUniform1i(glGetUniformLocation(programToUse, "SpriteSheetRow"), spriteSheetRow);
	glUniform1i(glGetUniformLocation(programToUse, "SpriteSheetColumn"), spriteSheetColumn);
	glUniformMatrix4fv(glGetUniformLocation(programToUse, "ModelMat"), 1, GL_FALSE, glm::value_ptr(ModelMat));
	glUniformMatrix4fv(glGetUniformLocation(programToUse, "ViewMat"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(programToUse, "ProjectionMat"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniform3fv(glGetUniformLocation(programToUse, "CameraPos"), 1, glm::value_ptr(cameraPosition));

	// Activate and bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(programToUse, "Texture0"), 0);
	if (secondTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, secondTexture->GetTextureID());
		glUniform1i(glGetUniformLocation(programToUse, "Texture1"), 1);
	}

	// Render based on mesh type
	switch (type)
	{
		case TRI:
		{
			glDrawArraysInstanced(GL_TRIANGLES, 0, 3, Count_Instanced);
			break;
		}
		case QUAD:
		case QUAD_FLIP:
		case QUAD_TILE:
		{
			glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, Count_Instanced);
			break;
		}
		case HEX:
		{
			glDrawElementsInstanced(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0, Count_Instanced);
			break;
		}
		case CUBE:
		{
			glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, Count_Instanced);
			break;
		}
		case MODEL:
		{
			glDrawArraysInstanced(DrawType, 0, DrawCount, Count_Instanced);
			//glDrawArrays(DrawType, 0, DrawCount);
			break;
		}
	}

	// Unbind the VAO and program to prevent accidental modifications
	glBindVertexArray(0);
	glUseProgram(0);
}

void Mesh::Update(float _currentTime, glm::mat4 _viewMat, glm::mat4 _projectionMat, Camera* _camera)
{
	// Calculate the Model Matrix
	TranslationMat = glm::translate(glm::mat4(1.0f), Position);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(RotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), Scale);

	ModelMat = TranslationMat * RotationMat * ScaleMat;

	// Update caamera variables
	viewMatrix = _viewMat;
	projectionMatrix = _projectionMat;
	cameraPosition = _camera->GetCameraPosition();

	// Update time variable
	currentTime = _currentTime;

	// Update texture variables
	if (texture != nullptr)
	{
		textureID = texture->GetTextureID();
		frameIndex = texture->GetFrameIndex();
		frameCount = texture->GetFrameCount();
		spriteSheetRow = texture->GetSpriteSheetRow();
		spriteSheetColumn = texture->GetSpriteSheetColumn();
	}
	else
	{
		std::cerr << "Error: Failed to load texture." << std::endl;
	}

}

// Generate model matrix instances for instanced rendering
// This function creates multiple instances of the model matrix with random translations
// Reminder : Need to change count in the shader as well
void Mesh::GenerateModelMatInstances(int _count)
{
	modelMatInstances.clear();
	Count_Instanced = _count;
	modelMatInstances.resize(Count_Instanced);

	// Generate random transformations for each instance
	for (int i = 0; i < Count_Instanced; i++)
	{
		glm::vec3 randPos = glm::vec3(
			((rand() % 4000) / 100.0f) - 20.0f, // X: -20.0 to 20.0
			0.0f, // Fixed Y position
			((rand() % 4000) / 100.0f) - 20.0f  // Z: -20.0 to 20.0
		);

		glm::vec3 randRot = glm::vec3(
			0.0f,
			((rand() % 3600) / 10.0f), // Yaw: 0 to 360 degrees
			0.0f
		);

		// Scale 0.8 to 1.2 the original size 
		glm::vec3 randScale = glm::vec3(
			((rand() % 400) / 1000.0f) + 0.8f,
			((rand() % 400) / 1000.0f) + 0.8f,
			((rand() % 400) / 1000.0f) + 0.8f
		);

		glm::mat4 transMat = glm::translate(TranslationMat, randPos); // Translate based on original translation matrix
		glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), glm::radians(randRot.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 scalMat = glm::scale(ScaleMat, randScale); // Scale based on original scale matrix
		modelMatInstances[i] = transMat * rotMat * scalMat;
	}
	modelMatInstances[0] = ModelMat; // Ensure the first instance uses the original model matrix

}