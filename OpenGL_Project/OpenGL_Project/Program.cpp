#include "Program.h"

Program::Program()
{
    Program_FixedTri = ShaderLoader::CreateProgram( "Resources/Shaders/FixedTriangle.vert", 
                                                    "Resources/Shaders/FixedColor.frag");
    Program_PositionOnly = ShaderLoader::CreateProgram( "Resources/Shaders/PositionOnly.vert", 
                                                        "Resources/Shaders/VertexColor.frag");
    Program_ColorFade = ShaderLoader::CreateProgram(    "Resources/Shaders/PositionOnly.vert", 
                                                        "Resources/Shaders/VertexColorFade.frag");
    Program_WorldSpace = ShaderLoader::CreateProgram(   "Resources/Shaders/WorldSpace.vert", 
                                                        "Resources/Shaders/VertexColorFade.frag");
    Program_Assignment1 = ShaderLoader::CreateProgram(  "Resources/Shaders/Assignment1_WorldSpace.vert", 
                                                        "Resources/Shaders/Assignment1_Color.frag");
    Program_Texture = ShaderLoader::CreateProgram(  "Resources/Shaders/Texture.vert", 
                                                    "Resources/Shaders/Texture.frag");
    Program_TextureMix = ShaderLoader::CreateProgram(  "Resources/Shaders/Texture.vert", 
		                                                "Resources/Shaders/TextureMix.frag");
    Program_SpriteSheet = ShaderLoader::CreateProgram( "Resources/Shaders/TextureSpriteSheet.vert", 
		                                                "Resources/Shaders/Texture.frag");
    Program_ClipSpace = ShaderLoader::CreateProgram(   "Resources/Shaders/ClipSpace.vert", 
		                                                "Resources/Shaders/Texture.frag");
    Program_TextureInstanced = ShaderLoader::CreateProgram( "Resources/Shaders/TextureInstanced.vert",
		                                                    "Resources/Shaders/Texture.frag");
    Program_TexLight = ShaderLoader::CreateProgram( "Resources/Shaders/Texture.vert",
		                                            "Resources/Shaders/Lighting_BlinnPhong.frag");
    Program_TexLightInstanced = ShaderLoader::CreateProgram(    "Resources/Shaders/TextureInstanced.vert",
		                                                        "Resources/Shaders/Lighting_BlinnPhong.frag");
}

Program::~Program()
{
}
