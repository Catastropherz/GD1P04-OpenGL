/***********************************************************************
 Bachelor of Software Engineering
 Media Design School
 Auckland
 New Zealand
 (c)
 2024 Media Design School
 File Name : Program.h
 Description : Handle shader programs setting
 Author : Q Sivakorn Tuangwilai
 Mail : sivakorn.tuangwilai@mds.ac.nz
 **************************************************************************/
#pragma once
#include "ShaderLoader.h"

class Program
{
public:
    GLuint Program_Texture = 0;
	GLuint Program_TextureInstanced = 0;
	GLuint Program_TexLight = 0;
	GLuint Program_TexLightInstanced = 0;
	GLuint Program_TexReflect = 0;
	GLuint Program_TexReflectInstanced = 0;
	GLuint Program_TextureUI = 0;

    Program();
    ~Program();

};

