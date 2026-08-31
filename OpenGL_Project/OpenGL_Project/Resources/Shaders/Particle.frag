#version 460 core

in float FragLife;

out vec4 FinalColor;

void main()
{
    // Fade alpha based on remaining life
    float alpha = clamp(FragLife / 5.0f, 0.0f, 1.0f);
    FinalColor = vec4(1.0f, 0.8f, 0.2f, alpha); // Yellow-gold particle color
}