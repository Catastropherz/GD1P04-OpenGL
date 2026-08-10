#version 460 core

in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

// Uniform Samplers for 4 Terrain Layers
uniform sampler2D TextureGrass; // Texture Unit 0
uniform sampler2D TextureDirt;  // Texture Unit 1
uniform sampler2D TextureStone; // Texture Unit 2
uniform sampler2D TextureSnow;  // Texture Unit 3

// Lighting Uniforms
uniform vec3 LightDir   = normalize(vec3(0.5, -1.0, 0.3));
uniform vec3 LightColor = vec3(1.0, 1.0, 1.0);
uniform float Ambient   = 0.2f;

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

    // Directional Diffuse + Ambient Lighting
    vec3 norm = normalize(FragNormal);
    float diff = max(dot(norm, -LightDir), 0.0);
    vec3 lighting = (Ambient + diff) * LightColor;

    FinalColor = vec4(lighting, 1.0) * blendedColor;
}