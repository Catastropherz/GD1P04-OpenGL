#include "Program.h"

Program::Program()
{
    Program_Texture = ShaderLoader::CreateProgram(  "Resources/Shaders/Texture.vert", 
                                                    "Resources/Shaders/Texture.frag");
    Program_TextureInstanced = ShaderLoader::CreateProgram( "Resources/Shaders/TextureInstanced.vert",
		                                                    "Resources/Shaders/Texture.frag");
    Program_TexLight = ShaderLoader::CreateProgram( "Resources/Shaders/Texture.vert",
		                                            "Resources/Shaders/Lighting_BlinnPhong.frag");
    Program_TexLightInstanced = ShaderLoader::CreateProgram(    "Resources/Shaders/TextureInstanced.vert",
		                                                        "Resources/Shaders/Lighting_BlinnPhong.frag");
    Program_TexReflect =  ShaderLoader::CreateProgram(  "Resources/Shaders/Texture.vert",
                                                        "Resources/Shaders/Lighting_Reflect.frag");
    Program_TexReflectInstanced = ShaderLoader::CreateProgram(  "Resources/Shaders/TextureInstanced.vert",
                                                                "Resources/Shaders/Lighting_Reflect.frag");
    Program_TextureUI = ShaderLoader::CreateProgram("Resources/Shaders/TextureUI.vert",
                                                    "Resources/Shaders/Texture.frag");

}

Program::~Program()
{
}
