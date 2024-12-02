#version 450

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

 out vec3 v_Position;
 out vec3 v_Normal;
 out vec2 v_TexCoords;

layout(std140, binding = 0) uniform ViewProjection {
    mat4 u_ViewProjection;
};

void main() {
    v_Normal =  a_Normal;
    v_TexCoords = a_TexCoords;

    gl_Position =  u_ViewProjection*  vec4(a_Position, 1.0);
}

