#version 330 core
layout (location = 0) in vec3 modelPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aUV;
layout (location = 3) in vec3 blockPos;

uniform vec3 bounds;
uniform vec3 playerPos;
uniform mat4x4 perspectiveMatrix;
uniform mat4x4 rotationMatrix;

out vec3 color;
out vec3 uv;

void main()
{
    mat4x4 worldMatrix = mat4x4(1.0f);
    worldMatrix[3] = vec4(vec3(blockPos).xyz, 1);

    mat4x4 viewMatrix = mat4x4(1.0f);
    viewMatrix[3] = vec4(vec3(-playerPos).xyz, 1);
    viewMatrix = rotationMatrix * viewMatrix;

    mat4x4 combinedMatrix = perspectiveMatrix * viewMatrix * worldMatrix;

    vec4 viewCoordinates =  combinedMatrix * vec4(modelPos.xyz, 1);

    vec4 proj = viewCoordinates;

    gl_Position = vec4(proj.x / bounds.x, proj.y / bounds.y, proj.z / bounds.z, proj.w / bounds.z);
    color = aColor;
    uv = aUV;
}