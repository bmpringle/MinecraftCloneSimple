#version 330 core

in vec2 uv;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D textureSampler;

uniform vec3 viewPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float opacity;
    float shininess;
};

uniform Material material;

void main()
{
    vec2 modUV = vec2(uv.x, 1-uv.y); 
    /*vec3 lightPos = vec3(0, 100, 0);
    vec3 lightColor = vec3(1, 1, 1);

    // ambient
    vec3 ambient = lightColor * material.ambient * texture(textureSampler, modUV).xyz;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * material.diffuse);
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = lightColor * (spec * material.specular);  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);*/
    FragColor = texture(textureSampler, modUV);
} 