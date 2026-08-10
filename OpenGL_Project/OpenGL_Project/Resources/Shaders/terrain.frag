#version 460 core

in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

// Uniform Samplers for 4 Terrain Layers
uniform sampler2D TextureGrass; // Texture Unit 0
uniform sampler2D TextureDirt;  // Texture Unit 1
uniform sampler2D TextureStone; // Texture Unit 2
uniform sampler2D TextureSnow;  // Texture Unit 3

// Lighting Uniforms (provided by LightManager)
uniform float AmbientStrength;
uniform vec3 AmbientColor;

struct DirectionalLight {
    vec3 Direction;
    vec3 Color;
    float SpecularStrength;
};
uniform DirectionalLight Directional;

out vec4 FinalColor;

void main()
{
    vec2 TiledUV = FragTexCoords * 8.0;

    vec4 texGrass = texture(TextureGrass, TiledUV);
    vec4 texDirt  = texture(TextureDirt,  TiledUV);
    vec4 texStone = texture(TextureStone, TiledUV);
    vec4 texSnow  = texture(TextureSnow,  TiledUV);

    float height = FragPos.y;

    float dirtWeight  = smoothstep(5.0,  15.0, height);
    float stoneWeight = smoothstep(20.0, 35.0, height);
    float snowWeight  = smoothstep(40.0, 55.0, height);

    // Lerp layers
    vec4 blendedColor = mix(texGrass, texDirt,  dirtWeight);
    blendedColor      = mix(blendedColor, texStone, stoneWeight);
    blendedColor      = mix(blendedColor, texSnow,  snowWeight);

    // Directional Diffuse + Ambient Lighting (use LightManager uniforms)
    vec3 norm = normalize(FragNormal);
    vec3 lightDir = normalize(Directional.Direction);
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 lighting = (AmbientStrength * AmbientColor) + diff * Directional.Color;

    FinalColor = vec4(lighting, 1.0) * blendedColor;
}