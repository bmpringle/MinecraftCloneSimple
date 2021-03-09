#version 330 core
layout (location = 0) in vec3 modelPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aUV;

uniform mat4x4 combinedMatrix;

out vec3 color;
out vec3 uv;

void main()
{
    vec4 proj =  combinedMatrix * vec4(modelPos.xyz, 1);

    gl_Position = vec4(proj.x, proj.y, proj.z, proj.w);
    color = aColor;
    uv = aUV;
}