#version 330 core

layout(location = 0) in vec3 aPos;       // Match with BufferLayout in Mesh
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    FragPos = vec3(u_Model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;

    gl_Position = u_ViewProjection * vec4(FragPos, 1.0);
}
