#version 330 core
in vec2 uv;

out vec4 FragColor;
uniform sampler2D textureSampler;

void main()
{
    vec2 modUV = vec2(uv.x, 1-uv.y); 
    FragColor = texture(textureSampler, modUV);
} 