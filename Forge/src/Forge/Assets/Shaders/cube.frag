// cube.frag
#version 330 core

in vec3 FragPos;    // From vertex shader
in vec3 Normal;     // From vertex shader

out vec4 FragColor;

uniform vec3 lightPos;     // Position of the light source
uniform vec3 viewPos;      // Position of the viewer/camera
uniform vec3 lightColor;   // Color of the light
uniform vec3 objectColor;  // Color of the object (we'll set this to magenta)

void main()
{
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(Normal);                      // Normalize the normal vector
    vec3 lightDir = normalize(lightPos - FragPos);      // Direction from fragment to light source
    float diff = max(dot(norm, lightDir), 0.0);         // Calculate the diffuse component
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);        // Direction from fragment to viewer
    vec3 reflectDir = reflect(-lightDir, norm);         // Reflect light direction around normal
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);  // Calculate the specular component
    vec3 specular = specularStrength * spec * lightColor;

    // Combine lighting components
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);                      // Set the fragment color
}
