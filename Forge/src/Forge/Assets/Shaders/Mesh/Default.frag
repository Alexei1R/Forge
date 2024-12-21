#version 450 core

layout(location = 0) out vec4 o_Color;

uniform vec4 Color;
uniform vec3 EmissiveColor;
uniform float Metallic;
uniform float Roughness;
uniform float Specular;

// Hardcoded light properties
const vec3 u_LightPosition = vec3(-50.0, -50.0, 50.0); 
const vec3 u_LightColor = vec3(1.0, 1.0, 1.0);     
const float u_AmbientStrength = 0.2;              
const float u_SpecularStrength = 0.7;              

in vec2 v_TexCoord;
in vec3 v_Norm;
in vec3 v_Position;

void main()
{
    // Ambient component
    vec3 ambient = u_AmbientStrength * u_LightColor;

    // Diffuse component
    vec3 norm = normalize(v_Norm);
    vec3 lightDir = normalize(u_LightPosition - v_Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // Specular component
    vec3 viewDir = normalize(-v_Position);  // Assuming camera is at origin
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = u_SpecularStrength * spec * u_LightColor;

    // Combine lighting components
    vec3 result = (ambient + diffuse)* Color.xyz;

    o_Color = vec4(result, 1.0);
}
