#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;

uniform vec3 bounds;
uniform vec3 playerPos;
uniform mat4x4 perspectiveMatrix;
uniform mat4x4 rotationMatrix;

out vec3 color;
out vec2 uv;

void main()
{
    vec4 playerPosRotation = rotationMatrix * vec4((aPos - playerPos).xyz, 1);

    vec4 proj = perspectiveMatrix * vec4(playerPosRotation.x, playerPosRotation.y, playerPosRotation.z, 1);

    gl_Position = vec4(proj.x / bounds.x, proj.y / bounds.y, proj.z / bounds.z, proj.w / bounds.z);
    color = aColor;
    uv = aUV;
}