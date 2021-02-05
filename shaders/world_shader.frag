#version 330 core
in vec3 color;
in vec2 uv;

out vec4 FragColor;
uniform sampler2DArray textureSampler;

void main()
{
    vec2 modUV = vec2(uv.x, -uv.y); 
    FragColor = texture(textureSampler, vec3(modUV.xy, 1));
} 