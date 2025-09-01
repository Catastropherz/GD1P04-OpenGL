#include "Program.h"

Program::Program()
{
    Program_Texture = ShaderLoader::CreateProgram(  "Resources/Shaders/Texture.vert", 
                                                    "Resources/Shaders/Texture.frag");
    Program_TextureMix = ShaderLoader::CreateProgram(  "Resources/Shaders/Texture.vert", 
		                                                "Resources/Shaders/TextureMix.frag");
    Program_SpriteSheet = ShaderLoader::CreateProgram( "Resources/Shaders/TextureSpriteSheet.vert", 
		                                                "Resources/Shaders/Texture.frag");
}

Program::~Program()
{
}
