#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include "ShaderLoader.h"

GLFWwindow* Window = nullptr;

void InitialSetup();
void Update();
void Render();

GLuint Program_FixedTri = 0;

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

	// Initialize GLEW to manage OpenGL function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		system("pause");
		glfwTerminate();
		return -1;
	}

	// Setup the Initial elements of the program
	InitialSetup();

	// Main Loop ***********************************************
	while (glfwWindowShouldClose(Window) == false)
	{
		//Update all objects and run the processes
		Update();

		// Render all objects
		Render();
	}
	// End of Main Loop ****************************************

	// Ensuring correct shutdown
	glfwTerminate();

	return 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(Program_FixedTri);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glUseProgram(0);

	glfwSwapBuffers(Window);
}

void InitialSetup()
{
	// Set the clear color to blue
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Maps the range of window size to NDC  (-1 to 1)
	glViewport(0, 0, 800, 800);

	//Create the program
	Program_FixedTri = ShaderLoader::CreateProgram("Resources/Shaders/FixedTriangle.vert", 
													"Resources/Shaders/FixedColor.frag");
    
}

void Update()
{
	// Check for any events like key presses or mouse movements
	glfwPollEvents();
	// Check if the user has requested to close the window
	if (glfwWindowShouldClose(Window))
	{
		glfwSetWindowShouldClose(Window, true);
	}
}