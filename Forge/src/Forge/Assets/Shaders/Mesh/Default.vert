#version 450 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Norm;
layout(location = 2) in vec2 a_TexCoord;

layout(std140, binding = 0) uniform ViewProjection {
    mat4 u_ViewProjection;
};

out vec2 v_TexCoord;
out vec3 v_Norm;
out vec3 v_Position;

void main()
{
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
    v_TexCoord = a_TexCoord;
    v_Norm = a_Norm;
    v_Position = a_Position;
}
