/***********************************************************************
 Bachelor of Software Engineering
 Media Design School
 Auckland
 New Zealand
 (c)
 2024 Media Design School
 File Name : ShaderLoader.h
 Description : Handle shader loading
 Author : Q Sivakorn Tuangwilai
 Mail : sivakorn.tuangwilai@mds.ac.nz
 **************************************************************************/
#pragma once

// Library Includes
#include <glew.h>
#include <glfw3.h>
#include <iostream>

class ShaderLoader
{
	
public:	
	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);

private:
	ShaderLoader(void);
	~ShaderLoader(void);
	// Create a shader
	static GLuint CreateShader(GLenum shaderType, const char* shaderName);

	// Read a shader file and return the content as a string
	static std::string ReadShaderFile(const char* filename);

	// Print error details
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);
};
