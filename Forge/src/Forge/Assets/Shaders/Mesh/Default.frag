#version 450 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;
in vec3 v_Norm;
in vec3 v_Position;

void main()
{
    o_Color = vec4(1.0, 0.0, 1.0, 1.0);
}
