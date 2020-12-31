#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform vec3 bounds;
uniform vec3 playerPos;
uniform mat4x4 perspectiveMatrix;
uniform mat3x3 rotationMatrix;

out vec3 color;


void main()
{
    vec3 posRotation = rotationMatrix * aPos;

    vec3 playerPosRotation = rotationMatrix * playerPos;

    vec3 posPlayer = vec3(posRotation.x - playerPosRotation.x, posRotation.y - playerPosRotation.y, posRotation.z - playerPosRotation.z);

    vec4 proj = perspectiveMatrix * vec4(posPlayer.x, posPlayer.y, -posPlayer.z, 1);

    gl_Position = vec4(proj.x / bounds.x, proj.y / bounds.y, proj.z / bounds.z, proj.w / bounds.z);
    color = aColor;
}