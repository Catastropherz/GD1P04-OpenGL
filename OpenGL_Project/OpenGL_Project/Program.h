#pragma once
#include "ShaderLoader.h"

class Program
{
public:
    GLuint Program_FixedTri = 0;
    GLuint Program_PositionOnly = 0;
	GLuint Program_ColorFade = 0;
	GLuint Program_WorldSpace = 0;
	GLuint Program_Assignment1 = 0;
    GLuint Program_Texture = 0;
	GLuint Program_TextureMix = 0;
	GLuint Program_SpriteSheet = 0;
	GLuint Program_ClipSpace = 0;

    Program();
    ~Program();

};

