#version 330 core
layout (location = 0) in vec3 modelPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aUV;
layout (location = 3) in vec3 blockPos;

uniform vec3 bounds;
uniform vec3 playerPos;
uniform mat4x4 perspectiveMatrix;
uniform mat3x3 rotationMatrixX;
uniform mat3x3 rotationMatrixY;

out vec3 color;
out vec2 uv;

void main()
{
    vec3 posRotation = rotationMatrixY * (rotationMatrixX * (modelPos + blockPos));

    vec3 playerPosRotation = rotationMatrixY * (rotationMatrixX * playerPos);

    vec3 posPlayer = vec3(posRotation.x - playerPosRotation.x, posRotation.y - playerPosRotation.y, posRotation.z - playerPosRotation.z);

    vec4 proj = perspectiveMatrix * vec4(posPlayer.x, posPlayer.y, -posPlayer.z, 1);

    gl_Position = vec4(proj.x / bounds.x, proj.y / bounds.y, proj.z / bounds.z, proj.w / bounds.z);
    color = aColor;
    uv = aUV;
}