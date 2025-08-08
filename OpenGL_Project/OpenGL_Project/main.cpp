#include "Program.h"
#include "ShaderLoader.h"

GLFWwindow* Window = nullptr;

void InitialSetup(Program* _program);
void Update();
void Render(Program* _program);

// Vertices / Indices
GLfloat Vertices_Tri[] = {
	// position
	0.0f, 0.0f, 0.0f,
	-0.5f, 0.8f, 0.0f,
	0.5f, 0.8f, 0.0f,
};

int main()
{
	// Initialize GLFW and set the version to 4.6 with compatibility profile
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	//Create the program
	Program program;

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
	InitialSetup(&program);

	// Main Loop ***********************************************
	while (glfwWindowShouldClose(Window) == false)
	{
		//Update all objects and run the processes
		Update();

		// Render all objects
		Render(&program);
	}
	// End of Main Loop ****************************************

	// Ensuring correct shutdown
	glfwTerminate();

	return 0;
}

void Render(Program* _program)
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Fixed triangle
	//glUseProgram(_program->Program_FixedTri);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	//glUseProgram(0);

	//Position only triangle
	glUseProgram(_program->Program_PositionOnly);
	glBindVertexArray(_program->VAO_Tri);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	glUseProgram(0);

	glfwSwapBuffers(Window);
}

void InitialSetup(Program* _program)
{
	// Set the clear color to blue
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Maps the range of window size to NDC  (-1 to 1)
	glViewport(0, 0, 800, 800);

	// Load the shaders and create the shader program
	_program->Program_FixedTri = ShaderLoader::CreateProgram("Resources/Shaders/FixedTriangle.vert", 
															"Resources/Shaders/FixedColor.frag");
	_program->Program_PositionOnly = ShaderLoader::CreateProgram("Resources/Shaders/PositionOnly.vert", 
																"Resources/Shaders/FixedColor.frag");
    
	// Generate VAO for the triangle
	glGenVertexArrays(1, &_program->VAO_Tri);
	glBindVertexArray(_program->VAO_Tri);
	
	//Generate VBO for a triangle
	glGenBuffers(1, &_program->VBO_Tri);
	glBindBuffer(GL_ARRAY_BUFFER, _program->VBO_Tri);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Tri), Vertices_Tri, GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


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