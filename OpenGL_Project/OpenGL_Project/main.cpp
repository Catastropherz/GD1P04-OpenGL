/***********************************************************************
 Bachelor of Software Engineering
 Media Design School
 Auckland
 New Zealand
 (c)
 2024 Media Design School
 File Name : main.cpp
 Description : Problem_003_1
 Author : Q Sivakorn Tuangwilai
 Mail : sivakorn.tuangwilai@mds.ac.nz
 **************************************************************************/

#pragma once
#include "Mesh_Terrain.h"


GLFWwindow* Window = nullptr;

void InitialSetup(Camera* _camera, int _windowWidth, int _windowHeight);
void Render(Camera* _camera, SkyBox* _skybox , Mesh_Terrain* _terrain, Mesh* _meshArray[], int _meshCount,
	Mesh* _UIArray[] = nullptr, int _UICount = 0);
void Update(Camera* _camera, LightManager* _lightManager, float* _currentTime, float* _previousTime, float* _deltaTime, 
	Mesh_Terrain* _terrain, Mesh* _meshArray[], int _meshCount, Mesh* _UIArray[] = nullptr, int _UICount = 0);
void ProcessInput(float _deltaTime, Camera* _camera, Mesh* _meshToMove);
void KeyInput(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
void TextInput(GLFWwindow* _window, unsigned int _codePoint);
void MouseButtonInput(GLFWwindow* _window, int _button, int _action, int _mods);
void CursorPositionInput(GLFWwindow* _window, double _xpos, double _ypos);
void ScrollInput(GLFWwindow* _window, double _xoffset, double _yoffset);


// Object Matrices and Components -------------------
glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
float RotationAngle = 0.0f; // Degrees

// Camera parameters
bool enableOrbiting = false;

// Window size
int WindowWidth = 800;
int WindowHeight = 800;

// Booleans
bool isTextEntryActive = false;
bool isMousePositionActive = false;
bool enableMouse = true;
bool enableWireframe = false;
bool isMouseOverButton = false;
bool enablePointLight = true;
bool enableDirectionalLight = true;
bool enableSpotlight = true;

// For defining which mesh is a button and which mesh should switch texture
Mesh* button = nullptr;
Mesh* targetMeshForSwitchingTexture = nullptr;

// Colors
glm::vec3 SolidColorRed = glm::vec3(1.0f, 0.0f, 0.0f); // Red
glm::vec3 SolidColorGreen = glm::vec3(0.0f, 1.0f, 0.0f); // Green

//Textures
TextureLoader texture0;
TextureLoader texture0Map;
TextureLoader texture1;
TextureLoader textureButtonRed;
TextureLoader textureButtonGreen;
TextureLoader textureGround;
TextureLoader textureGrass;
TextureLoader textureSnow;
TextureLoader textureStone;

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
	glfwSetWindowUserPointer(Window, &camera);
	InitialSetup(&camera, WindowWidth, WindowHeight);

	// Load and set up the objects ---------------------------------

	// Skybox
	std::string skyboxFaces[6] =
	{
		"Resources/Textures/Skybox/right.png",
		"Resources/Textures/Skybox/left.png",
		"Resources/Textures/Skybox/top.png",
		"Resources/Textures/Skybox/bot.png",
		"Resources/Textures/Skybox/front.png",
		"Resources/Textures/Skybox/back.png"

	};
	SkyBox skybox(&camera, skyboxFaces);

	// Create Light Manager
	LightManager lightManager;
	lightManager.setAmbientLightStrength(0.03f, glm::vec3(1.0f, 1.0f, 1.0f));
	lightManager.addPointLight(glm::vec3(25.0f, 10.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);
	lightManager.setAttenuationForPointLight(0, 1.0f, 0.04f, 0.0075f);
	lightManager.addPointLight(glm::vec3(-25.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
	lightManager.setAttenuationForPointLight(1, 1.0f, 0.04f, 0.0075f);
	lightManager.setDirectionalLight(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.3f, 0.3f, 0.3f), 0.5f);

	// Create terrain
	HeightMapInfo info;
	info.FilePath = "Resources/Heightmaps/Heightmap0.raw";
	info.Width = 128;
	info.Depth = 128;
	info.CellSpacing = 1.0f;
	info.HeightScale = 1.0f;

	Mesh_Terrain terrain;
	terrain.LoadHeightMap(info);
	terrain.SmoothHeights(info);
	terrain.BuildVertexData(info);
	terrain.GenerateNormals(info);
	terrain.BuildIndexData(info);
	terrain.SetupMesh();

	// Attach Shader Program, Textures, and Lights
	terrain.setProgram(&program.Program_TextureTerrain);
	terrain.setTexture1(&textureGrass);
	terrain.setTexture2(&textureGround);
	terrain.setTexture3(&textureStone);
	terrain.setTexture4(&textureSnow);
	terrain.SetLightManager(&lightManager);
	terrain.setModel(glm::vec3(0.0f, 0.0f, 0.0f));

	// Create mesh objects
	Mesh meshBarrel("Resources/Models/SM_Prop_Barrel_01.obj");
	meshBarrel.setModel(glm::vec3(0.0f, -12.0f, 0.0f), glm::vec3(20.0f, 20.0f, 20.0f), RotationAngle);
	meshBarrel.setProgram(&program.Program_TexLight);
	meshBarrel.setTexture(&texture0);
	meshBarrel.SetLightManager(&lightManager);

	Mesh meshTree("Resources/Models/SM_Env_Statue_04.obj", 
		glm::vec3(0.0f, -12.0f, 0.0f), glm::vec3(6.0f, 6.0f, 6.0f), RotationAngle, 1000);
	meshTree.setProgram(&program.Program_TexLightInstanced);
	meshTree.setTexture(&texture0);
	meshTree.setSecondTexture(&texture1);
	meshTree.SetLightManager(&lightManager);

	Mesh meshGround(CUBE);
	meshGround.setModel(glm::vec3(0.0f, -12.0f, 0.0f), glm::vec3(2000.0f, 1.0f, 2000.0f), RotationAngle);
	meshGround.setProgram(&program.Program_TexLight);
	meshGround.setTexture(&textureGround);
	meshGround.SetLightManager(&lightManager);

	Mesh meshRedLightCube(CUBE);
	meshRedLightCube.setModel(glm::vec3(25.0f, 10.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f), RotationAngle);
	meshRedLightCube.setProgram(&program.Program_Texture);
	meshRedLightCube.setTexture(&textureButtonRed);

	Mesh meshGreenLightCube(CUBE);
	meshGreenLightCube.setModel(glm::vec3(-25.0f, 10.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f), RotationAngle);
	meshGreenLightCube.setProgram(&program.Program_Texture);
	meshGreenLightCube.setTexture(&textureButtonGreen);


	Mesh meshBanner("Resources/Models/SM_Wep_Banner_05.obj");
	meshBanner.setModel(glm::vec3(0.0f, -7.0f, 0.0f), glm::vec3(15.0f, 15.0f, 15.0f), RotationAngle);
	meshBanner.setProgram(&program.Program_TexReflect);
	meshBanner.SetSkybox(&skybox, &texture0Map);
	meshBanner.setTexture(&texture0);
	meshBanner.setSecondTexture(&texture1);
	targetMeshForSwitchingTexture = &meshBanner;

	Mesh meshButton(QUAD);
	meshButton.setModel(glm::vec3(100.0f, 700.0f, 0.0f), glm::vec3(100.0f, 100.0f, 1.0f), 0.0f);
	meshButton.setProgram(&program.Program_TextureUI);
	meshButton.SetOrtho(WindowWidth, WindowHeight);
	meshButton.setTexture(&textureButtonRed);
	meshButton.setSecondTexture(&textureButtonGreen);
	button = &meshButton;

	// Create an array containing all the mesh objects
	Mesh* meshArray[] = { &meshGround };
	Mesh* UIArray[] = { &meshButton };
	int meshCount = sizeof(meshArray) / sizeof(meshArray[0]);
	int UICount = sizeof(UIArray) / sizeof(UIArray[0]);



	// Main Loop ***********************************************
	while (glfwWindowShouldClose(Window) == false)
	{
		// Update all objects and run the processes
		Update(&camera, &lightManager, &CurrentTime, &PreviousTime, &DeltaTime, &terrain, meshArray, meshCount, UIArray, UICount);

		// Render all objects
		Render(&camera, &skybox, &terrain ,meshArray, meshCount, UIArray, UICount);
	}
	// End of Main Loop ****************************************

	// Ensuring correct shutdown
	glfwTerminate();

	return 0;
}

// Render function to render all objects
void Render(Camera* _camera, SkyBox* _skybox, Mesh_Terrain* _terrain ,Mesh* _meshArray[], int _meshCount, Mesh* _UIArray[], int _UICount)
{
	// Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render skybox first
	_skybox->RenderSkybox(); // Uncomment to see skybox

	// Render Terrain
	if (_terrain != nullptr)
	{
		_terrain->Render();
		_terrain->DumpDebug(16, 48);
	}

	//// Render mesh objects
	//if (_meshArray != nullptr)
	//{
	//	for (int i = 0; i < _meshCount; ++i)
	//	{
	//		_meshArray[i]->Render();
	//	}
	//}

	// Render UI --------
	// Temporarily Disable Depth Testing
    // UI elements should always be drawn on top, regardless of their Z position.
	glDisable(GL_DEPTH_TEST);
	 if (_UIArray != nullptr)
	 {
		 for (int i = 0; i < _UICount; ++i)
		 {
			 //_UIArray[i]->Render(); // Uncomment to see UI
		 }
	 }
	 // Re-enable depth testing
	 glEnable(GL_DEPTH_TEST);

	// Swap the front and back buffers
	glfwSwapBuffers(Window);
}

// Initial setup function to configure OpenGL settings and objects
void InitialSetup(Camera* _camera, int _windowWidth, int _windowHeight)
{
	// Seed random number generator
	srand((unsigned int)time(NULL));

	// Set the clear color
	//glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Maps the range of window size to NDC  (-1 to 1)
	glViewport(0, 0, _windowWidth, _windowHeight);

	// Enable MSAA
	glfwWindowHint(GLFW_SAMPLES, 4); // 4 samples per fragment
	glEnable(GL_MULTISAMPLE);

	// Set the key input callback function
	glfwSetKeyCallback(Window, KeyInput);
	glfwSetMouseButtonCallback(Window, MouseButtonInput);
	glfwSetCursorPosCallback(Window, CursorPositionInput);
	glfwSetScrollCallback(Window, ScrollInput);

	// Enable Depth testing for 3D
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Face Culling
	//glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK); // Default is back face culling
	glFrontFace(GL_CCW); // Default is counter-clockwise winding order to be considered front-facing

	// Wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Uncomment to see wireframe mode

	// Calculate the projection matrix 
	//// Anchor top left --------
	// _camera->SetProjectionMatrix_Orthographic(0, _windowWidth, _windowHeight, 0, 0.1f, 1000.0f);
	//// Anchor center --------
	//_camera->SetProjectionMatrix_Orthographic(-_windowWidth / 2, _windowWidth / 2, -_windowHeight / 2, _windowHeight / 2, 0.1f, 100.0f);
	//// Perspective projection
	_camera->SetProjectionMatrix_Perspective(_windowWidth, _windowHeight, 45.0f, 0.1f, 1000.0f);

	// TEXTURE SETUP ---------------------------------
	texture0.LoadTexture("Resources/Textures/Dungeons_Texture_01.png");
	texture0Map.LoadTexture("Resources/Textures/ReflectionMap_Banner.png");
	texture1.LoadTexture("Resources/Textures/Dungeons_Texture_03.png");
	textureButtonRed.LoadTexture("Resources/Textures/Red.png");
	textureButtonGreen.LoadTexture("Resources/Textures/Green.png");
	textureGround.LoadTexture("Resources/Textures/Ground.png");
	textureGrass.LoadTexture("Resources/Textures/Green.png");
	textureStone.LoadTexture("Resources/Textures/Stone.png");
	textureSnow.LoadTexture("Resources/Textures/Snow.png");

		
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Update function to update all objects
void Update(Camera* _camera, LightManager* _lightManager, float* _currentTime, float* _previousTime, float* _deltaTime, Mesh_Terrain* _terrain,
	Mesh* _meshArray[], int _meshCount, Mesh* _UIArray[], int _UICount)
{
	// Check for any events like key presses or mouse movements
	glfwPollEvents();

	// Get time
	*_currentTime = (float)glfwGetTime();
	*_deltaTime = *_currentTime - *_previousTime;
	*_previousTime = *_currentTime;

	// Process input
	ProcessInput(*_deltaTime, _camera, _meshArray[0]);

	// Cursor mode
	if (enableMouse)
	{
		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Normal cursor mode
	}
	else
	{
		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide and lock cursor mode
		//glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); // Hide cursor mode
	}

	// Wireframe mode
	if (!enableWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else
	{
		// Wireframe mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// Update terrain
	_terrain->Update(*_currentTime, _camera->GetViewMatrix(), _camera->GetProjectionMatrix(), _camera);

	// Update object components
	if (_meshArray != nullptr)
	{
		for (int i = 0; i < _meshCount; ++i)
		{
			_meshArray[i]->Update(*_currentTime, _camera->GetViewMatrix(), _camera->GetProjectionMatrix(), _camera);
		}
	}

	// Update UI
	if (_UIArray != nullptr)
	{
		for (int i = 0; i < _UICount; ++i)
		{
			_UIArray[i]->Update(*_currentTime, _camera->GetViewMatrix(), _camera->GetProjectionMatrix(), _camera);
		}
	}


	// Update the camera
	_camera->Update(*_currentTime, enableOrbiting);
	double mouseX, mouseY;
	glfwGetCursorPos(Window, &mouseX, &mouseY);
	_camera->ProcessMouseMovement(mouseX, mouseY);

	// Update light manager
	_lightManager->setSpotlight(_camera->GetCameraPosition(), _camera->GetCameraForwardDirection(),
		7.5f, 12.5f, glm::vec3(4.0f, 4.0f, 4.0f), 400.0f);
	_lightManager->togglePointLight(enablePointLight);
	_lightManager->toggleDirectionalLight(enableDirectionalLight);
	_lightManager->toggleSpotlight(enableSpotlight);




}

//Query GLFW key states
void ProcessInput(float _deltaTime, Camera* _camera , Mesh* _meshToMove)
{
	float speed = 30.0f;

	if (glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glm::vec3 direction = glm::vec3(0.0f, 1.0f, 0.0f); // Move up
		_camera->MoveCamera(direction * speed * _deltaTime);
	}
	if (glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
	{
		glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f); // Move down
		_camera->MoveCamera(direction * speed * _deltaTime);
	}
	if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::vec3 direction = _camera->GetCameraRightDirection() * -1.0f; // Move left
		_camera->MoveCamera(direction * speed * _deltaTime);
	}
	if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::vec3 direction = _camera->GetCameraRightDirection(); // Move right
		_camera->MoveCamera(direction * speed * _deltaTime);
	}
	if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::vec3 direction = _camera->GetCameraForwardDirection(); // Move forward
		_camera->MoveCamera(direction * speed * _deltaTime);
	}
	if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		glm::vec3 direction = _camera->GetCameraForwardDirection() * -1.0f; // Move backward
		_camera->MoveCamera(direction * speed * _deltaTime);
	}
}

// Callback function called in response to keyboard input. Processed during glfwPollEvents()
void KeyInput(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods)
{
	if (_key == GLFW_KEY_ENTER && _action == GLFW_PRESS)
	{
		isTextEntryActive = !isTextEntryActive; // Toggle text entry mode when Enter is pressed
		if (isTextEntryActive)
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
	if (_key == GLFW_KEY_TAB && _action == GLFW_PRESS) // Press Space to toggle camera orbiting
	{
		enableOrbiting = !enableOrbiting;
	}
	if (_key == GLFW_KEY_1 && _action == GLFW_PRESS)
	{
		enablePointLight = !enablePointLight; // Toggle point light
	}
	if (_key == GLFW_KEY_2 && _action == GLFW_PRESS)
	{
		enableDirectionalLight = !enableDirectionalLight; // Toggle directional light
	}
	if (_key == GLFW_KEY_3 && _action == GLFW_PRESS)
	{
		enableSpotlight = !enableSpotlight; // Toggle spotlight
	}
}

// Callback function called in response to mouse button input. Processed during glfwPollEvents()
void MouseButtonInput(GLFWwindow* _window, int _button, int _action, int _mods)
{
	if (_button == GLFW_MOUSE_BUTTON_LEFT && _action == GLFW_PRESS)
	{
		// Check if the mouse click occurred while hovering the UI Quad
		if (isMouseOverButton && targetMeshForSwitchingTexture)
		{
			std::cout << "Switching Banner Texture" << std::endl;

			// Toggle the 3D object's texture
			targetMeshForSwitchingTexture->ToggleTexture();

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
	if (isMousePositionActive)
	{
		std::cout << "Cursor Position: (" << _xpos << ", " << _ypos << ")" << std::endl;
	}

	// Handle texture switch on hover
	if (button->checkHover(_xpos, _ypos) && !isMouseOverButton)
	{
		// Enter hover state
		isMouseOverButton = true;
		button->ToggleTexture();
	}
	else if (!(button->checkHover(_xpos, _ypos)) && isMouseOverButton)
	{
		// Exit hover state
		isMouseOverButton = false;
		button->ToggleTexture();
	}
}

// Callback function called in response to scroll input. Processed during glfwPollEvents()
void ScrollInput(GLFWwindow* _window, double _xoffset, double _yoffset)
{
	Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(_window));
	if (camera)
	{
		camera->ProcessMouseScroll(_yoffset);
	}
}