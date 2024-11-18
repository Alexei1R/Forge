#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

layout(std140, binding = 0) uniform CombinedData {
    mat4 u_ViewProjection;
    mat4 u_OrthoProjection;
};

out vec4 v_Color;
out vec2 v_TexCoord;
flat out float v_TexIndex;

uniform bool u_UseScreenSpace;

void main()
{
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;

    mat4 projection = u_UseScreenSpace ? u_OrthoProjection : u_ViewProjection;
    gl_Position = projection * vec4(a_Position, 1.0);
}
