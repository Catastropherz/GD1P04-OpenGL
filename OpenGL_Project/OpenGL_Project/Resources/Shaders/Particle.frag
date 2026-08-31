#version 460 core

in float FragLife;

uniform vec3 ParticleColor;

out vec4 FinalColor;

void main()
{
    // Linear alpha fade over particle lifetime
    float alpha = clamp(FragLife / 2.0f, 0.0f, 1.0f);
    FinalColor = vec4(ParticleColor, alpha);
}