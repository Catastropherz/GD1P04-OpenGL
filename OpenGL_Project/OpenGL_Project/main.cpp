#include "Program.h"
#include "ShaderLoader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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
	/*0*/		-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	-2.0f, 2.0f,		// Top Left
	/*1*/		-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	-2.0f, -2.0f,		// Btm Left
	/*2*/		0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	2.0f, -2.0f,		// Btm Right
	/*3*/		0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 1.0f,	2.0f, 2.0f,		// Top Right
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
glm::mat4 TranslationMat;
glm::vec3 PositionSecondHex = glm::vec3(0.5f, 0.5f, 0.0f);
glm::mat4 TranslationMatSecondHex;
float RotationAngle = 0.0f; // Degrees
glm::mat4 RotationMat;
glm::vec3 Scale = glm::vec3(1.5f, 1.5f, 1.0f);
glm::mat4 ScaleMat;
glm::vec3 SolidColorRed = glm::vec3(1.0f, 0.0f, 0.0f); // Red
glm::vec3 SolidColorGreen = glm::vec3(0.0f, 1.0f, 0.0f); // Green

GLuint texturePotion;
//--------------------------------------------------

int main()
{
	// Initialize GLFW and set the version to 4.6 with compatibility profile
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//Create the program
	Program program;
	float CurrentTime = 0.0f;

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
	GLuint ProgramToUse = _program->Program_Texture;

	// Bind the Program
	glUseProgram(ProgramToUse);
	
	// Bind the VAO for the triangle
	glBindVertexArray(_program->VAO_Tri);
	
	// Send variables to the shaders via Uniform
	GLint CurrentTimeLoc = glGetUniformLocation(ProgramToUse, "CurrentTime");
	glUniform1f(CurrentTimeLoc, *_currentTime);
	GLint TranslationMatLoc = glGetUniformLocation(ProgramToUse, "TranslationMat");
	glUniformMatrix4fv(TranslationMatLoc, 1, GL_FALSE, glm::value_ptr(TranslationMat));
	GLint RotationMatLoc = glGetUniformLocation(ProgramToUse, "RotationMat");
	glUniformMatrix4fv(RotationMatLoc, 1, GL_FALSE, glm::value_ptr(RotationMat));
	GLint ScaleMatLoc = glGetUniformLocation(ProgramToUse, "ScaleMat");
	glUniformMatrix4fv(ScaleMatLoc, 1, GL_FALSE, glm::value_ptr(ScaleMat));
	GLint SolidColorLoc = glGetUniformLocation(ProgramToUse, "SolidColor");
	glUniform3fv(SolidColorLoc, 1, glm::value_ptr(SolidColorRed));

	// Activate and bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePotion);
	glUniform1i(glGetUniformLocation(_program->Program_Texture, "Texture0"), 0);
	
	// Render
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //Quad using EBO
	//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0); //Hexagon using EBO

	//// Second Hexagon -----------------------------
	//glUniformMatrix4fv(TranslationMatLoc, 1, GL_FALSE, glm::value_ptr(TranslationMatSecondHex));
	//glUniform3fv(SolidColorLoc, 1, glm::value_ptr(SolidColorGreen));
	//// Render the second hexagon
	//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0); //Hexagon using EBO
	
	//Unbind the VAO and program to prevent accidental modifications
	glBindVertexArray(0);
	glUseProgram(0);

	glfwSwapBuffers(Window);
}

void InitialSetup(Program* _program)
{
	// Set the clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //black

	// Maps the range of window size to NDC  (-1 to 1)
	glViewport(0, 0, 800, 800);

	//// Load the shaders and create the shader program
	_program->Program_FixedTri = ShaderLoader::CreateProgram("Resources/Shaders/FixedTriangle.vert", 
															"Resources/Shaders/FixedColor.frag");
	_program->Program_PositionOnly = ShaderLoader::CreateProgram("Resources/Shaders/PositionOnly.vert", 
																"Resources/Shaders/VertexColor.frag");
	_program->Program_ColorFade = ShaderLoader::CreateProgram("Resources/Shaders/PositionOnly.vert",
																"Resources/Shaders/VertexColorFade.frag");
	_program->Program_WorldSpace = ShaderLoader::CreateProgram("Resources/Shaders/WorldSpace.vert",
																"Resources/Shaders/VertexColorFade.frag");
	_program->Program_Assignment1 = ShaderLoader::CreateProgram("Resources/Shaders/Assignment1_WorldSpace.vert",
																"Resources/Shaders/Assignment1_Color.frag");
	_program->Program_Texture = ShaderLoader::CreateProgram("Resources/Shaders/Texture.vert",
															"Resources/Shaders/Texture.frag");
    
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
	// Load the image data
	int imageWidth, imageHeight, imageComponents;
	stbi_set_flip_vertically_on_load(true); // Flip the image vertically if needed
	unsigned char* imageData = stbi_load(	"Resources/Textures/Jump_Attack__000.png",
											&imageWidth, &imageHeight, &imageComponents, 0);
	if (!imageData) {
		std::cerr << "Failed to load texture image!" << std::endl;
	}
	// Create and bind a texture
	glGenTextures(1, &texturePotion);
	glBindTexture(GL_TEXTURE_2D, texturePotion);
	// Check how many components the image has (RGBA or RGB)
	GLint loadedComponents = (imageComponents == 4) ? GL_RGBA : GL_RGB;
	// Populate the texture with the image data
	glTexImage2D(	GL_TEXTURE_2D, 0, loadedComponents, imageWidth, imageHeight, 0,
					loadedComponents, GL_UNSIGNED_BYTE, imageData);
	// Setting the filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// Generate the mipmaps, free the memory, and unbind the texture
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imageData);
	glBindTexture(GL_TEXTURE_2D, 0);
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
	// Second hexagon
	TranslationMatSecondHex = glm::translate(glm::mat4(1.0f), PositionSecondHex);
}