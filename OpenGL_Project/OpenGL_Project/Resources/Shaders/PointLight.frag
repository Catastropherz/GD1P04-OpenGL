#version 460 core

#define MAX_POINT_LIGHTS 4

struct PointLight
{
	vec3 Position;
	vec3 Color;
	float SpecularStrength;
};

//Input from vertex shader
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

// Uniform Inputs
uniform sampler2D Texture0;
uniform float AmbientStrength	= 0.15f;
uniform vec3 AmbientColor		= vec3(1.0f, 1.0f, 1.0f);
uniform vec3 CameraPos;
uniform float ObjectShininess			= 32.0f;

uniform PointLight PointLightArray[MAX_POINT_LIGHTS];
uniform int PointLightCount;



// Output
out vec4 FinalColor;

vec3 CalculateLight_Point(int index)
{
	// Light Direction
	vec3 Norm = normalize(FragNormal);
	vec3 LightDir = normalize(FragPos - PointLightArray[index].Position);
	
	// Diffuse Component
	float DiffusalStrength = max(dot(Norm, -LightDir), 0.0f);
	vec3 Diffuse = DiffusalStrength * PointLightArray[index].Color;
	
	// Specular Component
	vec3 ReverseViewDir = normalize(CameraPos - FragPos);
	vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
	float SpecularStrength = pow(max(dot(Norm, HalfwayVector), 0.0f), ObjectShininess);
	vec3 Specular = PointLightArray[index].SpecularStrength * SpecularStrength * PointLightArray[index].Color;
	
	return Diffuse + Specular;
}

void main()
{
	// Ambient Component
	vec3 Ambient = AmbientStrength * AmbientColor;
	vec3 TotalLightOutput = vec3(0.0f);
	for (int i = 0; i < PointLightCount; ++i)
	{
		TotalLightOutput += CalculateLight_Point(i);
	}
	TotalLightOutput += Ambient;

	// Calculate the final color
	FinalColor = vec4(TotalLightOutput, 1.0f) * texture(Texture0, FragTexCoords);
}

