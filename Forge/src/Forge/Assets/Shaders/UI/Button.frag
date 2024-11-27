#version 450 core

layout(location = 0) in vec2 v_TexCoord;
layout(location = 0) out vec4 FragColor;

uniform vec4 Color;        // Fill color of the button
uniform float Radius;      // Radius of the rounded corners
uniform vec2 QuadSize;     // Width and height of the quad
uniform float Feather;     // Smooth edge width for antialiasing

float BorderSize = 3;  // Width of the border in pixels
vec4 BorderColor = vec4(vec3(1.0) ,0.5);  // Border color
                
// Signed Distance Function for a rounded rectangle
float roundedBoxSDF(vec2 point, vec2 size, float radius) {
    return length(max(abs(point) - size + vec2(radius), vec2(0.0))) - radius;
}

void main() {
    // Map v_TexCoord from [0,1] to local coordinates centered at (0,0)
    vec2 localCoord = (v_TexCoord - 0.5) * QuadSize;

    // Compute the signed distance to the outer rounded rectangle
    float distance = roundedBoxSDF(localCoord, QuadSize * 0.5, Radius);

    // Antialiasing for the outer edge
    float outerAlpha = 1.0 - smoothstep(0.0, Feather, distance);

    // Antialiasing for the inner edge (border to fill transition)
    float innerAlpha = smoothstep(-BorderSize + Feather, -BorderSize, distance);

    // Blend between border color and fill color
    vec4 color = mix(BorderColor, Color, innerAlpha);

    // Apply the outer alpha to the final color
    FragColor = vec4(color.rgb, color.a * outerAlpha);
}
