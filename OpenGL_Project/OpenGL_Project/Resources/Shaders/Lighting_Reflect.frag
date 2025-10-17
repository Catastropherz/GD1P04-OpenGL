#version 460 core

//Input from vertex shader
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

// Uniform Inputs
uniform sampler2D Texture0;
uniform samplerCube Texture_Skybox;
uniform sampler2D Texture0ReflectMap;
uniform float AmbientStrength	= 0.15f;
uniform vec3 AmbientColor		= vec3(1.0f, 1.0f, 1.0f);
uniform vec3 LightColor			= vec3(1.0f, 1.0f, 1.0f);
uniform vec3 LightPos			= vec3(-300.0f, 000.0f, 100.0f);
uniform vec3 CameraPos;
uniform float LightSpecularStrength = 1.0f;
uniform float ObjectShininess			= 32.0f;

// Output
out vec4 FinalColor;

void main()
{
	// Light Direction
	vec3 Norm = normalize(FragNormal);
	vec3 LightDir = normalize(FragPos - LightPos);

	// Diffuse Component
	float DiffusalStrength = max(dot(Norm, -LightDir), 0.0f);
	vec3 Diffuse = DiffusalStrength * LightColor;

	// Ambient Component
	vec3 Ambient = AmbientStrength * AmbientColor;

	// Specular Component
	vec3 ReverseViewDir = normalize(CameraPos - FragPos);
	vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
	float SpecularStrength = pow(max(dot(Norm, HalfwayVector), 0.0f), ObjectShininess);
	vec3 Specular = LightSpecularStrength * SpecularStrength * LightColor;

	// Combine the lighting components
	vec4 Light = vec4(Ambient + Diffuse + Specular, 1.0f);

	// Reflection
	vec3 Normal = normalize(FragNormal);
	vec3 ViewDir = normalize(FragPos - CameraPos);
	vec3 ReflectDir = reflect(ViewDir, Normal);

	vec4 ObjectTexture = texture(Texture0, FragTexCoords);
	vec4 ReflectionTexture = texture(Texture_Skybox, ReflectDir);

	// Calculate reflection factor based on the reflection map
	float ReflectionFactor = 1 - texture(Texture0ReflectMap, FragTexCoords).r;

	// Lighting + Reflect
	FinalColor = Light * mix(ObjectTexture, ReflectionTexture, ReflectionFactor);

}