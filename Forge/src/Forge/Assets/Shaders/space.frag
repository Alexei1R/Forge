#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 u_ViewPos;
uniform sampler2D colorTexture;  // The texture sampler

struct Light {
    vec3 direction;
    vec3 color;
};

uniform Light u_Light;

void main()
{
    FragColor = vec4(textureColor);  // Maintain texture alpha
}


