#version 330 core
layout (location = 0) in vec4 modelPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 normal;

uniform mat4x4 combinedMatrix;
uniform mat4x4 modelToWorldMatrix;

out vec2 uv;

void main()
{
    gl_Position = combinedMatrix * modelToWorldMatrix * modelPos;
    uv = aUV;
}