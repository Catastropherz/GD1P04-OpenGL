#version 460 core

in vec3 FragColor;
uniform float CurrentTime;
uniform vec3 SolidColor;

out vec4 FinalColor;

void main()
{
	float mixRatio = abs(sin(CurrentTime));

	vec3 mixedColor = mix(FragColor, SolidColor, mixRatio);
	
	FinalColor = vec4(mixedColor, 1.0f);
}