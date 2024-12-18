#version 450 core

layout(location = 0) in vec2 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;
layout(location = 4) in vec2 a_QuadSize;
layout(location = 5) in float a_Type;

layout(std140, binding = 0) uniform ViewProjection {
    mat4 u_ViewProjection;
};


out vec2 v_TexCoord;
out vec2 v_QuadSize;
out vec4 v_Color;
flat out float v_TexIndex;
flat out float v_Type;

void main()
{
    gl_Position = u_ViewProjection * vec4(a_Position.xy, 0.0, 1.0);
    v_TexCoord = a_TexCoord;
    v_Color = a_Color;
    v_TexIndex = a_TexIndex;
    v_Type = a_Type;
    v_QuadSize = a_QuadSize;
}
