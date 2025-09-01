#include "Mesh.h"


GLFWwindow* Window = nullptr;

void InitialSetup(Camera* _camera, int _windowWidth, int _windowHeight);
void Update(Camera* _camera, float* _currentTime, float* _previousTime, float* _deltaTime, Mesh* _meshArray[], int _meshCount);
void ProcessInput(float _deltaTime);
void KeyInput(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
void TextInput(GLFWwindow* _window, unsigned int _codePoint);
void MouseButtonInput(GLFWwindow* _window, int _button, int _action, int _mods);
void CursorPositionInput(GLFWwindow* _window, double _xpos, double _ypos);

// Object Matrices and Components -------------------
glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
float RotationAngle = 0.0f; // Degrees

// Window
int WindowWidth = 800;
int WindowHeight = 800;

// Entry bool
bool IsTextEntryActive = false;
bool IsMousePositionActive = false;

// Colors
glm::vec3 SolidColorRed = glm::vec3(1.0f, 0.0f, 0.0f); // Red
glm::vec3 SolidColorGreen = glm::vec3(0.0f, 1.0f, 0.0f); // Green

//GLuint texturePotion;
TextureLoader texture0;
TextureLoader texture1;
TextureLoader texture2;
TextureLoader texture3;
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
	meshTile.setModel(glm::vec3(-170.0f, 200.0f, 0.0f), glm::vec3(250.0f, 250.0f, 1.0f), RotationAngle);
	meshTile.setProgram(&program.Program_Texture);
	meshTile.setTexture(&texture0);

	Mesh meshMixed(QUAD);
	meshMixed.setModel(glm::vec3(200.0f, 200.0f, 0.0f), glm::vec3(200.0f, 200.0f, 1.0f), RotationAngle);
	meshMixed.setProgram(&program.Program_TextureMix);
	meshMixed.setTexture(&texture0);
	meshMixed.setSecondTexture(&texture1);

	Mesh meshQuad(QUAD);
	meshQuad.setModel(glm::vec3(-220.0, -200.0, 0.0f), glm::vec3(160.0f, 160.0f, 1.0f), RotationAngle);
	meshQuad.setProgram(&program.Program_Texture);
	meshQuad.setTexture(&texture1);

	Mesh meshFlip(QUAD_FLIP);
	meshFlip.setModel(glm::vec3(-80.0f, -200.0f, 0.0f), glm::vec3(160.0f, 160.0f, 1.0f), RotationAngle);
	meshFlip.setProgram(&program.Program_Texture);
	meshFlip.setTexture(&texture1);

	Mesh meshAnim(QUAD);
	meshAnim.setModel(glm::vec3(180.0f, -180.0f, 0.0f), glm::vec3(240.0f, 240.0f, 1.0f), RotationAngle);
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
			// Clear buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
			// Render
			meshTile.Render();
			meshMixed.Render();
			meshQuad.Render();
			meshFlip.Render();
			meshAnim.Render();

			// Animate sprite sheet
			texture2.AnimateSpriteSheet(CurrentTime);
		
			// Swap the front and back buffers
			glfwSwapBuffers(Window);
		// End of render
	}
	// End of Main Loop ****************************************

	// Ensuring correct shutdown
	glfwTerminate();

	return 0;
}


void InitialSetup(Camera* _camera, int _windowWidth, int _windowHeight)
{
	// Set the clear color
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// Maps the range of window size to NDC  (-1 to 1)
	glViewport(0, 0, _windowWidth, _windowHeight);

	// Set the key input callback function
	glfwSetKeyCallback(Window, KeyInput);
	glfwSetMouseButtonCallback(Window, MouseButtonInput);

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
	texture3.LoadTexture("Resources/Textures/ColorPickerOld.png");

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Update(Camera* _camera, float* _currentTime, float* _previousTime, float* _deltaTime, Mesh* _meshArray[], int _meshCount)
{
	// Check for any events like key presses or mouse movements
	glfwPollEvents();

	// Get time
	*_currentTime = (float)glfwGetTime();
	*_deltaTime = *_currentTime - *_previousTime;
	*_previousTime = *_currentTime;

	// Process input
	ProcessInput(*_deltaTime);

	// Update object components
	for (int i = 0; i < _meshCount; ++i)
	{
		_meshArray[i]->Update(*_currentTime, _camera->GetViewMatrix(), _camera->GetProjectionMatrix());
	}

	// Update the camera
	_camera->Update(*_currentTime);

}

//Query GLFW key states
void ProcessInput(float _deltaTime)
{
	if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += glm::vec3(0.0f, 1.0f, 0.0f) * _deltaTime; // Move up
	}
	if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position -= glm::vec3(0.0f, 1.0f, 0.0f) * _deltaTime; // Move down
	}
	if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position -= glm::vec3(1.0f, 0.0f, 0.0f) * _deltaTime; // Move left
	}
	if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += glm::vec3(1.0f, 0.0f, 0.0f) * _deltaTime; // Move right
	}
}

// Callback function called in response to keyboard input. Processed during glfwPollEvents()
void KeyInput(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	if (_key == GLFW_KEY_ENTER && _action == GLFW_PRESS)
	{
		IsTextEntryActive = !IsTextEntryActive; // Toggle text entry mode when Enter is pressed
		if (IsTextEntryActive)
		{
			glfwSetCharCallback(_window, TextInput); // Set the text input callback
			std::cout << "Text entry mode activated." << std::endl;
		}
		else
		{
			glfwSetCharCallback(_window, nullptr); // Remove the text input callback
			std::cout << "Text entry mode deactivated." << std::endl;
		}
	}
	if (_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(_window, true); // Close the window when Escape is pressed
	}
	if (_key == GLFW_KEY_R && (_action == GLFW_REPEAT || _action == GLFW_PRESS))
	{
		RotationAngle += 10.0f; // Rotate the object by 10 degrees when R is pressed
	}
	if (_key == GLFW_KEY_T && _action == GLFW_PRESS)
	{
		Position = glm::vec3(0.0f, 0.0f, 0.0f); // Reset position when T is pressed
	}
}

// Callback function called in response to mouse button input. Processed during glfwPollEvents()
void MouseButtonInput(GLFWwindow* _window, int _button, int _action, int _mods)
{
	if (_button == GLFW_MOUSE_BUTTON_RIGHT && _action == GLFW_PRESS)
	{
		std::cout << "Mouse Button Right: Press" << std::endl;
	}
	if (_button == GLFW_MOUSE_BUTTON_RIGHT && _action == GLFW_RELEASE)
	{
		std::cout << "Mouse Button Right: Release" << std::endl;
	}
	if (_button == GLFW_MOUSE_BUTTON_MIDDLE && _action == GLFW_PRESS)
	{
		IsMousePositionActive = !IsMousePositionActive; // Toggle mouse position display
		if (IsMousePositionActive)
		{
			glfwSetCursorPosCallback(_window, CursorPositionInput); // Set the cursor position callback
			std::cout << "Mouse position tracking activated." << std::endl;
		}
		else
		{
			glfwSetCursorPosCallback(_window, nullptr); // Remove the cursor position callback
			std::cout << "Mouse position tracking deactivated." << std::endl;
		}
	}
}

// Callback function called in response to keyboard input for text printing. Processed during glfwPollEvents()
void TextInput(GLFWwindow* _window, unsigned int _codePoint)
{
	std::cout << "Text input detected: " << static_cast<unsigned char>(_codePoint) << std::endl;
}

// Callback function called in response to cursor position input. Processed during glfwPollEvents()
void CursorPositionInput(GLFWwindow* _window, double _xpos, double _ypos)
{
	std::cout << "Cursor Position: (" << _xpos << ", " << _ypos << ")" << std::endl;
}