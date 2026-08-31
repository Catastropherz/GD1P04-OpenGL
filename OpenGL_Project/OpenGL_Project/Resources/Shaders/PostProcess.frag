#version 460 core

in vec2 FragTexCoords;

uniform sampler2D screenTexture; // Texture from Framebuffer
uniform int effectMode;          // 0 = Normal, 1 = Invert, 2 = Grayscale, 3 = Rain, 4: CRT
uniform float iTime;             // Shader playback time for animated effects
uniform vec2 iResolution;        // Viewport resolution

out vec4 FinalColor;

// ________________________________________________________
//
// CRT SCAN-LINE SHADER (ShaderToy Port)
//
// Curve screen UVs to simulate a curved CRT monitor glass
vec2 CurveUV(vec2 uv)
{
    uv = uv * 2.0 - 1.0;
    vec2 offset = abs(uv.yx) / vec2(6.0, 4.0);
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;
    return uv;
}



//
// MAIN SHADER PIPELINE
//
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
    // Mode 4: CRT
    else if (effectMode == 4)
    {
        // Apply monitor curvature distortion
        vec2 crtUV = CurveUV(FragTexCoords);

        // Black out out-of-bounds pixels outside CRT border
        if (crtUV.x < 0.0 || crtUV.x > 1.0 || crtUV.y < 0.0 || crtUV.y > 1.0)
        {
            FinalColor = vec4(0.0, 0.0, 0.0, 1.0);
            return;
        }

        // Sample texture at curved coordinates
        vec3 col = texture(screenTexture, crtUV).rgb;

        // Scanline Intensity (using resolution height or fallback to 800.0)
        float resY = (iResolution.y > 0.0) ? iResolution.y : 800.0;
        float scanline = sin(crtUV.y * resY * 2.0 + iTime * 4.0) * 0.25;
        col -= scanline;

        // Edge Vignette
        float vignette = crtUV.x * crtUV.y * (1.0 - crtUV.x) * (1.0 - crtUV.y);
        vignette = clamp(pow(16.0 * vignette, 0.35), 0.0, 1.0);
        col *= vignette;

        FinalColor = vec4(col, 1.0);
    }
    else
    {
        FinalColor = texture(screenTexture, FragTexCoords);
    }
}