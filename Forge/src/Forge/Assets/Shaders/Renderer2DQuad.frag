#version 450 core

in vec4 v_Color;
in vec2 v_TexCoord;
in flat float v_TexIndex;

layout(location = 0) out vec4 FragColor;

#define MAX_TEXTURE_SLOTS 32

uniform sampler2D u_Textures[MAX_TEXTURE_SLOTS];

void main() {
    int index = int(v_TexIndex);
    vec4 texColor = texture(u_Textures[index], v_TexCoord);
    FragColor = v_Color * texColor;
}
