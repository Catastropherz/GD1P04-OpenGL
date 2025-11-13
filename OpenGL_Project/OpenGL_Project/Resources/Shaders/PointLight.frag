#version 460 core

#define MAX_POINT_LIGHTS 4

struct PointLight
{
	vec3 Position;
	vec3 Color;
	float SpecularStrength;

	float AttenuationConstant;
	float AttenuationLinear;
	float AttenuationExponent ;
};

struct DirectionalLight
{
	vec3 Direction;
	vec3 Color;
	float SpecularStrength;
};

struct SpotLight
{
	vec3 Position;
    vec3 Direction;
    float CutOff;        // cos(inner angle)
    float OuterCutOff;   // cos(outer angle)
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

uniform DirectionalLight Directional;
uniform SpotLight Spotlight;



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
	
	vec3 LightOutput = Diffuse + Specular;

	float Distance = length(PointLightArray[index].Position - FragPos);
	float Attenuation = PointLightArray[index].AttenuationConstant + 
						(PointLightArray[index].AttenuationLinear * Distance) + 
						(PointLightArray[index].AttenuationExponent * Distance * Distance);
	LightOutput /= Attenuation;
	return LightOutput;
}

vec3 CalculateLight_Directional()
{
	// Light Direction
	vec3 Norm = normalize(FragNormal);
	vec3 LightDir = normalize(Directional.Direction);
	
	// Diffuse Component
	float DiffusalStrength = max(dot(Norm, -LightDir), 0.0f);
	vec3 Diffuse = DiffusalStrength * Directional.Color;
	
	// Specular Component
	vec3 ReverseViewDir = normalize(CameraPos - FragPos);
	vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
	float SpecularStrength = pow(max(dot(Norm, HalfwayVector), 0.0f), ObjectShininess);
	vec3 Specular = Directional.SpecularStrength * SpecularStrength * Directional.Color;
	
	vec3 LightOutput = Diffuse + Specular;
	return LightOutput;
}

float CalcSpotlightIntensity(vec3 fragPos, vec3 normal) 
{
    vec3 lightDir = normalize(Spotlight.Position - fragPos);
    float theta = dot(lightDir, normalize(-Spotlight.Direction));
    float epsilon = Spotlight.CutOff - Spotlight.OuterCutOff;
    float intensity = clamp((theta - Spotlight.OuterCutOff) / epsilon, 0.0, 1.0);
    return intensity;
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
	TotalLightOutput += CalculateLight_Directional();

	// Spotlight
	float spotIntensity = CalcSpotlightIntensity(FragPos, FragNormal);
	if (spotIntensity > 0.0) 
	{
		vec3 SpotLightDir = normalize(Spotlight.Position - FragPos);
		float diff = max(dot(FragNormal, SpotLightDir), 0.0);
		vec3 diffuse = diff * Spotlight.Color;

		// Specular
		vec3 viewDir = normalize(CameraPos - FragPos);
		vec3 reflectDir = reflect(-SpotLightDir, FragNormal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), ObjectShininess);
		vec3 specular = Spotlight.SpecularStrength * spec * Spotlight.Color;

		TotalLightOutput += (diffuse + specular) * spotIntensity;
	}


	// Calculate the final color
	FinalColor = vec4(TotalLightOutput, 1.0f) * texture(Texture0, FragTexCoords);
}

