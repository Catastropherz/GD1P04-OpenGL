#include "Program.h"

Program::Program()
{
    Program_Texture = ShaderLoader::CreateProgram(  "Resources/Shaders/Texture.vert", 
                                                    "Resources/Shaders/Texture.frag");
    Program_TextureInstanced = ShaderLoader::CreateProgram( "Resources/Shaders/TextureInstanced.vert",
		                                                    "Resources/Shaders/Texture.frag");
    Program_TexLight = ShaderLoader::CreateProgram( "Resources/Shaders/Texture.vert",
		                                            "Resources/Shaders/PointLight.frag");
    Program_TexLightInstanced = ShaderLoader::CreateProgram(    "Resources/Shaders/TextureInstanced.vert",
		                                                        "Resources/Shaders/PointLight.frag");
    Program_TexReflect =  ShaderLoader::CreateProgram(  "Resources/Shaders/Texture.vert",
                                                        "Resources/Shaders/Lighting_Reflect.frag");
    Program_TexReflectInstanced = ShaderLoader::CreateProgram(  "Resources/Shaders/TextureInstanced.vert",
                                                                "Resources/Shaders/Lighting_Reflect.frag");
    Program_TextureUI = ShaderLoader::CreateProgram("Resources/Shaders/TextureUI.vert",
                                                    "Resources/Shaders/Texture.frag");
	Program_TextureTerrain = ShaderLoader::CreateProgram("Resources/Shaders/terrain.vert",
		                                                "Resources/Shaders/terrain.frag"),
    
    Program_PostProcess = ShaderLoader::CreateProgram("Resources/Shaders/ScreenQuad.vert",
                                                      "Resources/Shaders/PostProcess.frag");
    Program_ComputeParticle = ShaderLoader::CreateProgram_C("Resources/Shaders/ComputeParticles.comp");

    Program_RenderParticle = ShaderLoader::CreateProgram("Resources/Shaders/Particle.vert",
                                                         "Resources/Shaders/Particle.frag");
}

Program::~Program()
{
}
