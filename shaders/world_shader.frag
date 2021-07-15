#version 330 core
in vec3 uv;

out vec4 FragColor;
uniform sampler2DArray textureSampler;

void main()
{
    vec3 modUV = vec3(uv.x, 1-uv.y, uv.z); 
    FragColor = texture(textureSampler, modUV);
} 