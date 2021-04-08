#version 330 core

layout (location = 0) in vec4 modelPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 normal;

uniform mat4x4 combinedMatrix;
uniform mat4x4 modelToWorldMatrix;

out vec2 uv;
out vec3 Normal;
out vec3 FragPos;

void main()
{
    gl_Position = combinedMatrix * modelToWorldMatrix * modelPos;
    uv = aUV;
    vec4 vec4Normal = (modelToWorldMatrix * vec4(normal, 1));
    Normal = vec3(vec4Normal.x / vec4Normal.w, vec4Normal.y / vec4Normal.w, vec4Normal.z / vec4Normal.w);

    vec4 FragPosVec4 = (modelToWorldMatrix * modelPos);
    FragPos = vec3(FragPosVec4.x / FragPosVec4.w, FragPosVec4.y / FragPosVec4.w, FragPosVec4.z / FragPosVec4.w);
}