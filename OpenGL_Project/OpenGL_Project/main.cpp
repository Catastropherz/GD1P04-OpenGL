#include "TextureLoader.h"
#include "Camera.h"


GLFWwindow* Window = nullptr;

void InitialSetup(Program* _program, Camera* _camera, int _windowWidth, int _windowHeight);
void Update(Camera* _camera, float* _currentTime, float* _previousTime, float* _deltaTime);
void Render(Program* _program, Camera* _camera, float* _currentTime);
void ProcessInput(float _deltaTime);
void KeyInput(GLFWwindow* _window, int _key, int _scancode, int _action, int _mods);
void TextInput(GLFWwindow* _window, unsigned int _codePoint);
void MouseButtonInput(GLFWwindow* _window, int _button, int _action, int _mods);
void CursorPositionInput(GLFWwindow* _window, double _xpos, double _ypos);

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
GLfloat Vertices_Cube[] = {
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
GLuint Indices_Cube[] = {
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
// Object Matrices and Components -------------------
glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
//glm::vec3 Scale = glm::vec3(200.0f, 200.0f, 200.0f); // pixel scale
float RotationAngle = 0.0f; // Degrees

// Model Matrix
glm::mat4 TranslationMat;
glm::mat4 RotationMat;
glm::mat4 ScaleMat;
glm::mat4 ModelMat;

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
//--------------------------------------------------

int main()
{
	// Initialize GLFW and set the version to 4.6 with compatibility profile
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
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
	InitialSetup(&program, &camera, WindowWidth, WindowHeight);

	// Main Loop ***********************************************
	while (glfwWindowShouldClose(Window) == false)
	{
		//Update all objects and run the processes
		Update(&camera, &CurrentTime, &PreviousTime, &DeltaTime);

		// Render all objects
		Render(&program, &camera, &CurrentTime);
	}
	// End of Main Loop ****************************************

	// Ensuring correct shutdown
	glfwTerminate();

	return 0;
}

void Render(Program* _program, Camera* _camera, float* _currentTime)
{
	// Clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Program to use
	GLuint ProgramToUse = _program->Program_ClipSpace;

	// Bind the Program
	glUseProgram(ProgramToUse);
	
	// Bind the VAO for the triangle
	glBindVertexArray(_program->VAO_Tri);
	
	// Send variables to the shaders via Uniform
	glUniform1f(glGetUniformLocation(ProgramToUse, "CurrentTime"), *_currentTime);
	glUniform3fv(glGetUniformLocation(ProgramToUse, "SolidColor"), 1, glm::value_ptr(SolidColorRed));
	glUniform1i(glGetUniformLocation(ProgramToUse, "FrameIndex"), texture0.GetFrameIndex());
	glUniform1i(glGetUniformLocation(ProgramToUse, "FrameCount"), texture0.GetFrameCount());
	// Matrices
	glUniformMatrix4fv(glGetUniformLocation(ProgramToUse, "ModelMat"), 1, GL_FALSE, glm::value_ptr(ModelMat));
	glUniformMatrix4fv(glGetUniformLocation(ProgramToUse, "ViewMat"), 1, GL_FALSE, glm::value_ptr(_camera->GetViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(ProgramToUse, "ProjectionMat"), 1, GL_FALSE, glm::value_ptr(_camera->GetProjectionMatrix()));

	// Activate and bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture0.GetTextureID());
	glUniform1i(glGetUniformLocation(ProgramToUse, "Texture0"), 0);

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, texture1.GetTextureID());
	//glUniform1i(glGetUniformLocation(ProgramToUse, "Texture1"), 1);
	
	// Render
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); //Quad using EBO
	//glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0); //Hexagon using EBO
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); //Cube using EBO
	
	//Unbind the VAO and program to prevent accidental modifications
	glBindVertexArray(0);
	glUseProgram(0);

	glfwSwapBuffers(Window);
}

void InitialSetup(Program* _program, Camera* _camera, int _windowWidth, int _windowHeight)
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
	 //_camera->SetProjectionMatrix_Orthographic(-_windowWidth / 2, _windowWidth / 2, -_windowHeight / 2, _windowHeight / 2, 0.1f, 100.0f);
	// Perspective projection
	_camera->SetProjectionMatrix_Perspective(_windowWidth, _windowHeight, 45.0f, 0.1f, 100.0f);

	// Generate VAO
	glGenVertexArrays(1, &_program->VAO_Tri);
	glBindVertexArray(_program->VAO_Tri);
	
	//Generate EBO for the quad --------
	/*glGenBuffers(1, &_program->EBO_Quad);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _program->EBO_Quad);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Quad), Indices_Quad, GL_STATIC_DRAW);*/
	//Generate EBO for the hexagon --------
	/*glGenBuffers(1, &_program->EBO_Hex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _program->EBO_Hex);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Hex), Indices_Hex, GL_STATIC_DRAW);*/
	//Generate EBO for the cube --------
	glGenBuffers(1, &_program->EBO_Cube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _program->EBO_Cube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Cube), Indices_Cube, GL_STATIC_DRAW);

	//Generate VBO
	glGenBuffers(1, &_program->VBO_Tri);
	glBindBuffer(GL_ARRAY_BUFFER, _program->VBO_Tri);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Tri), Vertices_Tri, GL_STATIC_DRAW); //Triangle
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Quad), Vertices_Quad, GL_STATIC_DRAW); //Quad
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Hex), Vertices_Hex, GL_STATIC_DRAW); //Hexagon
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Cube), Vertices_Cube, GL_STATIC_DRAW); //Cube

	// Set the vertex attribute pointers
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(2);
	//glBindVertexArray(0);

	// After binding VAO and VBO for the cube
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// TEXTURE SETUP ---------------------------------
	texture0.LoadTexture("Resources/Textures/ColorPickerOld.png");
	//texture0.SetSpriteSheetParameters(7);
	//texture1.LoadTexture("Resources/Textures/Run (1).png");

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Update(Camera* _camera, float* _currentTime, float* _previousTime, float* _deltaTime)
{
	// Check for any events like key presses or mouse movements
	glfwPollEvents();

	// Get time
	*_currentTime = (float)glfwGetTime();
	*_deltaTime = *_currentTime - *_previousTime;
	*_previousTime = *_currentTime;

	// Process input
	ProcessInput(*_deltaTime);

	// Calculate the Model Matrix
	TranslationMat = glm::translate(glm::mat4(1.0f), Position);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(RotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), Scale);

	ModelMat = TranslationMat * RotationMat * ScaleMat;

	// Animate sprite sheet
	texture0.AnimateSpriteSheet(*_currentTime);

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