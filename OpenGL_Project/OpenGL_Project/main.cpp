/***********************************************************************
 Bachelor of Software Engineering
 Media Design School
 Auckland
 New Zealand
 (c)
 2024 Media Design School
 File Name : main.cpp
 Description : main program for OpenGL Assignment2
 Author : Q Sivakorn Tuangwilai
 Mail : sivakorn.tuangwilai@mds.ac.nz
 **************************************************************************/

#include "Mesh.h"

GLFWwindow* Window = nullptr;

void InitialSetup(Camera* _camera, int _windowWidth, int _windowHeight);
void Render(Mesh* _meshArray[], int _meshCount);
void Update(Camera* _camera, float* _currentTime, float* _previousTime, float* _deltaTime, Mesh* _meshArray[], int _meshCount);

// Window
int WindowWidth = 800;
int WindowHeight = 800;

//GLuint texturePotion;
TextureLoader texture0;
TextureLoader texture1;
TextureLoader texture2;
//--------------------------------------------------

int main()
{
	// Initialize GLFW and set the version to 4.6 with core profile
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);


	// Create an GLFW control window
	Window = glfwCreateWindow(WindowWidth, WindowHeight, "First OpenGL Window", NULL, NULL);
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
	Program program;
	float CurrentTime = 0.0f;
	float PreviousTime = 0.0f;
	float DeltaTime = 0.0f;
	Camera camera(Window);
	InitialSetup(&camera, WindowWidth, WindowHeight);

	// Create mesh objects
	Mesh meshTile(QUAD_TILE);
	meshTile.setModel(glm::vec3(-170.0f, 200.0f, 0.0f), glm::vec3(250.0f, 250.0f, 1.0f), 0.0f);
	meshTile.setProgram(&program.Program_Texture);
	meshTile.setTexture(&texture0);

	Mesh meshMixed(QUAD);
	meshMixed.setModel(glm::vec3(200.0f, 200.0f, 0.0f), glm::vec3(200.0f, 200.0f, 1.0f), 0.0f);
	meshMixed.setProgram(&program.Program_TextureMix);
	meshMixed.setTexture(&texture0);
	meshMixed.setSecondTexture(&texture1);

	Mesh meshQuad(QUAD);
	meshQuad.setModel(glm::vec3(-220.0, -200.0, 0.0f), glm::vec3(160.0f, 160.0f, 1.0f), 0.0f);
	meshQuad.setProgram(&program.Program_Texture);
	meshQuad.setTexture(&texture1);

	Mesh meshFlip(QUAD_FLIP);
	meshFlip.setModel(glm::vec3(-80.0f, -200.0f, 0.0f), glm::vec3(160.0f, 160.0f, 1.0f), 0.0f);
	meshFlip.setProgram(&program.Program_Texture);
	meshFlip.setTexture(&texture1);

	Mesh meshAnim(QUAD);
	meshAnim.setModel(glm::vec3(180.0f, -180.0f, 0.0f), glm::vec3(240.0f, 240.0f, 1.0f), 0.0f);
	meshAnim.setProgram(&program.Program_SpriteSheet);
	meshAnim.setTexture(&texture2);


	// Create an array containing all the mesh objects
	Mesh* meshArray[] = { &meshTile, &meshMixed, &meshQuad, &meshFlip, &meshAnim };
	int meshCount = sizeof(meshArray) / sizeof(meshArray[0]);



	// Main Loop ***********************************************
	while (glfwWindowShouldClose(Window) == false)
	{
		// Update all objects and run the processes
		Update(&camera, &CurrentTime, &PreviousTime, &DeltaTime, meshArray, meshCount);

		// Render all objects
		Render(meshArray, meshCount);
	}
	// End of Main Loop ****************************************

	// Ensuring correct shutdown
	glfwTerminate();

	return 0;
}

// Render function to render all objects
void Render(Mesh* _meshArray[], int _meshCount)
{
	// Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int i = 0; i < _meshCount; ++i)
	{
		_meshArray[i]->Render();
	}

	// Swap the front and back buffers
	glfwSwapBuffers(Window);
}

// Initial setup function to configure OpenGL settings and objects
void InitialSetup(Camera* _camera, int _windowWidth, int _windowHeight)
{
	// Set the clear color
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// Maps the range of window size to NDC  (-1 to 1)
	glViewport(0, 0, _windowWidth, _windowHeight);

	// Cursor mode
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Normal cursor mode
	//glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide and lock cursor mode
	//glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Hide cursor mode

	// Enable Depth testing for 3D
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Face Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // Default is back face culling
	glFrontFace(GL_CCW); // Default is counter-clockwise winding order to be considered front-facing

	// Wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Uncomment to see wireframe mode

	// Calculate the projection matrix 
	// Anchor top left --------
	// _camera->SetProjectionMatrix_Orthographic(0, _windowWidth, _windowHeight, 0, 0.1f, 100.0f);
	// Anchor center --------
	_camera->SetProjectionMatrix_Orthographic(-_windowWidth / 2, _windowWidth / 2, -_windowHeight / 2, _windowHeight / 2, 0.1f, 100.0f);
	// Perspective projection
	//_camera->SetProjectionMatrix_Perspective(_windowWidth, _windowHeight, 45.0f, 0.1f, 100.0f);

	// TEXTURE SETUP ---------------------------------
	texture0.LoadTexture("Resources/Textures/PepeSad.png");
	texture1.LoadTexture("Resources/Textures/PepeCry.png");
	texture2.LoadTexture("Resources/Textures/RobotSpriteSheet2D.png");
	texture2.SetSpriteSheetParameters(8, 2);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Update function to update all objects
void Update(Camera* _camera, float* _currentTime, float* _previousTime, float* _deltaTime, Mesh* _meshArray[], int _meshCount)
{
	// Check for any events like key presses or mouse movements
	glfwPollEvents();

	// Get time
	*_currentTime = (float)glfwGetTime();
	*_deltaTime = *_currentTime - *_previousTime;
	*_previousTime = *_currentTime;

	// Update object components
	for (int i = 0; i < _meshCount; ++i)
	{
		_meshArray[i]->Update(*_currentTime, _camera->GetViewMatrix(), _camera->GetProjectionMatrix());
	}

	// Update the camera
	_camera->Update(*_currentTime);

}