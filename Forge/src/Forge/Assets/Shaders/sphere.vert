#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;   // Optional: For lighting
layout(location = 2) in vec2 aTexCoord; // Optional: For texturing

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;       // Optional: For lighting calculations
out vec3 Normal;        // Optional: For lighting calculations
out vec2 TexCoord;      // Optional: For texturing

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));  // Optional
    Normal = mat3(transpose(inverse(model))) * aNormal;  // Optional
    TexCoord = aTexCoord;  // Optional

    gl_Position = proj * view * model * vec4(aPos, 1.0);
}

