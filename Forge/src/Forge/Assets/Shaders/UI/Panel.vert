
#version 450 core

layout(location = 0) in vec2 a_Position;

layout(std140, binding = 0) uniform ViewProjection {
    mat4 u_ViewProjection;
};


void main()
{

    gl_Position =  u_ViewProjection* vec4(a_Position, 0.0, 1.0);
}

