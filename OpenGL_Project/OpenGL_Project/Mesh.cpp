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
		{
			glGenBuffers(1, &EBO_Quad);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Quad);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Quad), Indices_Quad, GL_STATIC_DRAW);
			break;
		}
		case HEX:
		{
			glGenBuffers(1, &EBO_Hex);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Hex);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Hex), Indices_Hex, GL_STATIC_DRAW);
			break;
		}
		case CUBE:
		{
			glGenBuffers(1, &EBO_Cube);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Cube);
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
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Quad), Vertices_Quad, GL_STATIC_DRAW); //Quad
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

Mesh::~Mesh()
{
}

void Mesh::setModel(glm::vec3 _position, glm::vec3 _scale, float _angleDegrees)
{
	Position = _position;
	Scale = _scale;
	RotationAngle = _angleDegrees;
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

void Mesh::Render()
{
	// Bind the Program
	glUseProgram(programToUse);

	// Bind the VAO
	glBindVertexArray(VAO);

	// Send variables to the shaders via Uniform
	glUniform1f(glGetUniformLocation(programToUse, "CurrentTime"), currentTime);
	glUniform3fv(glGetUniformLocation(programToUse, "SolidColor"), 1, glm::value_ptr(solidColor));
	glUniform1i(glGetUniformLocation(programToUse, "FrameIndex"), frameIndex);
	glUniform1i(glGetUniformLocation(programToUse, "FrameCount"), frameCount);
	glUniformMatrix4fv(glGetUniformLocation(programToUse, "ModelMat"), 1, GL_FALSE, glm::value_ptr(ModelMat));
	glUniformMatrix4fv(glGetUniformLocation(programToUse, "ViewMat"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(programToUse, "ProjectionMat"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Activate and bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(programToUse, "Texture0"), 0);

	// Render based on mesh type
	switch (type)
	{
		case TRI:
		{
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
			break;
		}
		case QUAD:
		{
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			break;
		}
		case HEX:
		{
			glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
			break;
		}
		case CUBE:
		{
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			break;
		}
	}

	// Unbind the VAO and program to prevent accidental modifications
	glBindVertexArray(0);
	glUseProgram(0);
}

void Mesh::Update(float _currentTime, glm::mat4 _viewMat, glm::mat4 _projectionMat)
{
	// Calculate the Model Matrix
	TranslationMat = glm::translate(glm::mat4(1.0f), Position);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(RotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), Scale);

	ModelMat = TranslationMat * RotationMat * ScaleMat;

	// Update caamera variables
	viewMatrix = _viewMat;
	projectionMatrix = _projectionMat;

	// Update time variable
	currentTime = _currentTime;

	// Update texture variables
	textureID = texture->GetTextureID();
	frameIndex = texture->GetFrameIndex();
	frameCount = texture->GetFrameCount();

}
