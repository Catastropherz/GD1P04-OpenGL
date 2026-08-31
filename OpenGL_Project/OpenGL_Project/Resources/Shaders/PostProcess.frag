#version 460 core

in vec2 FragTexCoords;

uniform sampler2D screenTexture; // Texture from Framebuffer
uniform int effectMode;          // 0 = Normal, 1 = Invert, 2 = Grayscale, 3 = Rain
uniform float iTime;             // Shader playback time for animated effects
uniform vec2 iResolution;        // Viewport resolution

out vec4 FinalColor;

void main()
{
    // Mode 0: Normal Pass
    if (effectMode == 0)
    {
        FinalColor = texture(screenTexture, FragTexCoords);
    }
    // Mode 1: Color Inversion
    else if (effectMode == 1)
    {
        FinalColor = vec4(vec3(1.0f) - texture(screenTexture, FragTexCoords).rgb, 1.0f);
    }
    // Mode 2: Grayscale
    else if (effectMode == 2)
    {
        vec3 col = texture(screenTexture, FragTexCoords).rgb;
        float gray = dot(col, vec3(0.2126f, 0.7152f, 0.0722f));
        FinalColor = vec4(vec3(gray), 1.0f);
    }
    // Mode 3: Raining Effect
    else if (effectMode == 3)
    {
        vec2 uv = FragTexCoords;
        // Simple distortion
        vec2 rainUV = vec2(uv.x * 2.0f, uv.y * 0.1f + iTime * 0.125f);
        float rainDistort = sin(rainUV.x * 20.0f + iTime * 5.0f) * 0.01f;
        
        vec2 distortedUV = uv + vec2(rainDistort, 0.0f);
        FinalColor = texture(screenTexture, distortedUV);
    }
    else
    {
        FinalColor = texture(screenTexture, FragTexCoords);
    }
}