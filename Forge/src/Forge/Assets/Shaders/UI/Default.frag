#version 450 core

#define MAX_TEXTURE_SLOTS 32

in vec2 v_TexCoord;
in vec2 v_QuadSize;

in vec4 v_Color;
in float v_TexIndex;
in float v_Type;

out vec4 FragColor;

uniform int Radius ;
uniform float Feather ;
uniform int BorderSize ;
uniform vec4 BorderColor ;

const float u_ScreenPxRange = 4.0;
const vec4 u_FgColor = vec4(1.0, 1.0, 1.0, 1.0);
vec4 u_BgColor = v_Color;

uniform sampler2D u_Textures[MAX_TEXTURE_SLOTS];

float roundedBoxSDF(vec2 point, vec2 size, float radius) {
    return length(max(abs(point) - size + vec2(radius), vec2(0.0))) - radius;
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

void main() {
    if (v_Type == 1.0) {
        vec2 localCoord = (v_TexCoord - 0.5) * v_QuadSize;
        float distance = roundedBoxSDF(localCoord, v_QuadSize * 0.5, Radius);
        float outerAlpha = 1.0 - smoothstep(0.0, Feather, distance);
        float innerAlpha = smoothstep(-BorderSize + Feather, -BorderSize, distance);
        vec4 color = mix(BorderColor, v_Color, innerAlpha);
        FragColor = vec4(color.rgb, color.a * outerAlpha);
    } else if (v_Type == 2.0) {
        int index = int(v_TexIndex);
        vec4 texSample = texture(u_Textures[index], v_TexCoord);
        float sd = median(texSample.r, texSample.g, texSample.b);
        float screenPxDistance = u_ScreenPxRange * (sd - 0.5);
        float alpha = clamp(screenPxDistance + 0.5, 0.0, 1.0);
        vec4 color = mix(u_BgColor, u_FgColor, alpha);
        FragColor = color;
    } else {
        FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    }
}
