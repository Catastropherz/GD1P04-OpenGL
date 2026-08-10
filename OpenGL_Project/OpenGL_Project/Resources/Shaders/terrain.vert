#version 460 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;
layout (location = 2) in vec3 normal;

out vec2 FragTexCoords;
out vec3 FragNormal;
out vec3 FragPos;

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjectionMat;

void main()
{
    // Transform vertex position to world space
    vec4 worldPos = ModelMat * vec4(position, 1.0);
    FragPos = worldPos.xyz;

    // Pass through texture coordinates
    FragTexCoords = texcoord;
    
    // Transform normal to world space using the normal matrix
    FragNormal = mat3(transpose(inverse(ModelMat))) * normal;

    // Compute clip space position
    gl_Position = ProjectionMat * ViewMat * worldPos;
}