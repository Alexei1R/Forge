#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 u_ViewPos;

struct Light {
    vec3 direction;
    vec3 color;
};

uniform Light u_Light;


void main()
{
    // Ambient lighting
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * u_Light.color;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-u_Light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_Light.color;

    // Specular lighting
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 50);

    vec3 specular = spec * u_Light.color;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}

