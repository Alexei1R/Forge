#version 450 core


in vec4 v_Color;
in vec2 v_TexCoord;
flat in float v_TexIndex;

layout(location = 0) out vec4 FragColor;

#define MAX_TEXTURE_SLOTS 32

uniform sampler2D u_Textures[MAX_TEXTURE_SLOTS];

// Define constants instead of uniforms
const float u_ScreenPxRange = 4.0;               // Default pixel range
const vec4 u_FgColor = vec4(1.0, 1.0, 1.0, 1.0); // Default foreground color (white)
const vec4 u_BgColor = vec4(0.0, 0.0, 0.0, 0.0); // Default background color (transparent)

// Function to compute the median of three values
float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}



void main() {
    int index = int(v_TexIndex);
    vec4 texSample = texture(u_Textures[index], v_TexCoord);

    // For MSDF, compute the signed distance from the median of RGB channels
    float sd = median(texSample.r, texSample.g, texSample.b);

    // Calculate the screen-space distance
    float screenPxDistance = u_ScreenPxRange * (sd - 0.5);

    // Compute alpha value with smoothstep for anti-aliasing
    float alpha = clamp(screenPxDistance + 0.5, 0.0, 1.0);

    // Mix foreground and background colors based on the alpha
    vec4 color = mix(u_BgColor, u_FgColor, alpha);

    // Apply vertex color if needed
    FragColor = v_Color * color;

}
