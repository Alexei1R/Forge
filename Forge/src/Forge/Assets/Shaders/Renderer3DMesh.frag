#version 450

layout(location = 0) in vec3 v_Position;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec2 v_TexCoords;

layout(location = 0) out vec4 FragColor;

// Static light parameters
const vec3 LightDirection = normalize(vec3(-0.5, -1.0, -0.3)); // Direction of the light
const vec3 LightColor = vec3(1.0, 1.0, 1.0);                    // White light
const vec3 AmbientColor = vec3(0.1, 0.1, 0.1);                  // Low ambient light

// Static material parameters
const vec4 BaseColor = vec4(1.0, 0.5, 0.3, 1.0);                // Base color of the material (orange-ish)
const float Shininess = 32.0;                                   // Shininess factor for specular lighting
const vec3 CameraPosition = vec3(0.0, 0.0, 5.0);                // Static camera position in world space

void main() {
    // Normalize the interpolated normal
    vec3 normal = normalize(v_Normal);

    // Calculate ambient component
    vec3 ambient = AmbientColor * BaseColor.rgb;

    // Calculate diffuse component
    float diff = max(dot(normal, -LightDirection), 0.0);
    vec3 diffuse = diff * LightColor * BaseColor.rgb;

    // Calculate view direction and reflection for specular component
    vec3 viewDir = normalize(CameraPosition - v_Position);
    vec3 reflectDir = reflect(LightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
    vec3 specular = spec * LightColor;

    // Final color output
    vec3 resultColor = ambient + diffuse + specular;
    FragColor = vec4(resultColor, BaseColor.a);
}
