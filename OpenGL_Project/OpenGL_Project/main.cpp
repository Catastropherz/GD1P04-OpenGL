#include "TextureLoader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


GLFWwindow* Window = nullptr;

void InitialSetup(Program* _program);
void Update(float* _currentTime);
void Render(Program* _program, float* _currentTime);

// Vertices / Indices --------------------------
GLfloat Vertices_Tri[] = {
	// Position			// Color
	0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	// Top Right
	-0.5f, 0.8f, 0.0f,	0.0f, 1.0f, 0.0f,	// Top Left
	0.5f, 0.8f, 0.0f,	0.0f, 0.0f, 1.0f,	// Bottom Center
};
GLfloat Vertices_Quad[] = {
	// Index	// Position			// Color			// Texture Coords
	/*0*/		-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f,		// Top Left
	/*1*/		-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,		// Btm Left
	/*2*/		0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f,		// Btm Right
	/*3*/		0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 1.0f,	1.0f, 1.0f,		// Top Right
};
GLuint Indices_Quad[] = {
	0, 1, 2, // First Triangle (TL > BL > BR)
	0, 2, 3, // Second Triangle (TL > BR > TR)
};

GLfloat Vertices_Hex[] = {
	// Index	// Position				// Color
	/*0*/		-0.5f, 0.866f, 0.0f,	1.0f, 0.0f, 1.0f,	// Top Left
	/*1*/		-1.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,	// Left
	/*2*/		0.0f, 0.0f, 0.0f,		1.0f, 1.0f, 1.0f,	// Center
	/*3*/		-0.5f, -0.866f, 0.0f,	0.0f, 1.0f, 1.0f,	// Bottom Left
	/*4*/		0.5f, -0.866f, 0.0f,	0.0f, 1.0f, 0.0f,	// Bottom Right
	/*5*/		1.0f, 0.0f, 0.0f,		1.0f, 1.0f, 0.0f,	// Right
	/*6*/		0.5f, 0.866f, 0.0f,		1.0f, 0.0f, 0.0f,	// Top Right
};
GLuint Indices_Hex[] = {
	0, 1, 2, // First Triangle (TL > L > C)
	1, 3, 2, // Second Triangle (L > BL > C)
	3, 4, 2, // Third Triangle (BL > BR > C)
	4, 5, 2, // Fourth Triangle (BR > R > C)
	5, 6, 2, // Fifth Triangle (R > TR > C)
	6, 0, 2, // Sixth Triangle (TR > TL > C)
};
// Object Matrices and Components -------------------
glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
float RotationAngle = 0.0f; // Degrees
glm::vec3 Scale = glm::vec3(1.5f, 1.5f, 1.0f);


glm::mat4 TranslationMat;
glm::mat4 RotationMat;
glm::mat4 ScaleMat;
glm::mat4 ModelMat;

glm::vec3 SolidColorRed = glm::vec3(1.0f, 0.0f, 0.0f); // Red
glm::vec3 SolidColorGreen = glm::vec3(0.0f, 1.0f, 0.0f); // Green

//GLuint texturePotion;
TextureLoader texture0;
TextureLoader texture1;
//--------------------------------------------------

int main()
{
	// Initialize GLFW and set the version to 4.6 with compatibility profile
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);


	// Create an GLFW control window
	Window = glfwCreateWindow(800, 800, "First OpenGL Window", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(Window);
	glfwShowWindow(Window);

	// Initialize GLEW to manage OpenGL function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		system("pause");
		glfwTerminate();
		return -1;
	}

	//Create the program
	Program program;
	float CurrentTime = 0.0f;

	// Setup the Initial elements of the program
	InitialSetup(&program);

	// Main Loop ***********************************************
	while (glfwWindowShouldClose(Window) == false)
	{
		//Update all objects and run the processes
		Update(&CurrentTime);

		// Render all objects
		Render(&program, &CurrentTime);
	}
	// End of Main Loop ****************************************

	// Ensuring correct shutdown
	glfwTerminate();

	return 0;
}

void Render(Program* _program, float* _currentTime)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Program to use
	GLuint ProgramToUse = _program->Program_SpriteSheet;

	// Bind the Program
	glUseProgram(ProgramToUse);
	
	// Bind the VAO for the triangle
	glBindVertexArray(_program->VAO_Tri);
	
	// Send variables to the shaders via Uniform
	GLint CurrentTimeLoc = glGetUniformLocation(ProgramToUse, "CurrentTime");
	glUniform1f(CurrentTimeLoc, *_currentTime);
	GLint ModelMatLoc = glGetUniformLocation(ProgramToUse, "ModelMat");
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));
	GLint SolidColorLoc = glGetUniformLocation(ProgramToUse, "SolidColor");
	glUniform3fv(SolidColorLoc, 1, glm::value_ptr(SolidColorRed));
	GLint frameIndexLoc = glGetUniformLocation(ProgramToUse, "FrameIndex");
	glUniform1i(frameIndexLoc, texture0.GetFrameIndex());
	GLint frameCountLoc = glGetUniformLocation(ProgramToUse, "FrameCount");
	glUniform1i(frameCountLoc, texture0.GetFrameCount());

	// Activate and bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0.GetTextureID());
	glUniform1i(glGetUniformLocation(ProgramToUse, "Texture0"), 0);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, texture1.GetTextureID());
	//glUniform1i(glGetUniformLocation(ProgramToUse, "Texture1"), 1);
	
	// Render
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //Quad using EBO
	//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0); //Hexagon using EBO
	
	//Unbind the VAO and program to prevent accidental modifications
	glBindVertexArray(0);
	glUseProgram(0);

	glfwSwapBuffers(Window);
}

void InitialSetup(Program* _program)
{
	// Set the clear color
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// Maps the range of window size to NDC  (-1 to 1)
	glViewport(0, 0, 800, 800);
    
	// Generate VAO
	glGenVertexArrays(1, &_program->VAO_Tri);
	glBindVertexArray(_program->VAO_Tri);
	
	//Generate EBO for the quad
	glGenBuffers(1, &_program->EBO_Quad);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _program->EBO_Quad);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Quad), Indices_Quad, GL_STATIC_DRAW);
	//Generate EBO for the hexagon
	/*glGenBuffers(1, &_program->EBO_Hex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _program->EBO_Hex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Hex), Indices_Hex, GL_STATIC_DRAW);*/

	//Generate VBO
	glGenBuffers(1, &_program->VBO_Tri);
	glBindBuffer(GL_ARRAY_BUFFER, _program->VBO_Tri);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Tri), Vertices_Tri, GL_STATIC_DRAW); //Triangle
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Quad), Vertices_Quad, GL_STATIC_DRAW); //Quad
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Hex), Vertices_Hex, GL_STATIC_DRAW); //Hexagon

	// Set the vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// TEXTURE SETUP ---------------------------------
	texture0.LoadTexture("Resources/Textures/AdventureGirl_Attack.png");
	texture0.SetSpriteSheetParameters(7);
	//texture1.LoadTexture("Resources/Textures/Run (1).png");

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Update(float* _currentTime)
{
	// Check for any events like key presses or mouse movements
	glfwPollEvents();

	// Get time
	*_currentTime = (float)glfwGetTime();

	// Calculate the Model Matrix
	TranslationMat = glm::translate(glm::mat4(1.0f), Position);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(RotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), Scale);

	ModelMat = TranslationMat * RotationMat * ScaleMat;

	// Animate sprite sheet
	texture0.AnimateSpriteSheet(*_currentTime);
}