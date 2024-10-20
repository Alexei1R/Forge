#version 330 core

in vec3 nearPoint;
in vec3 farPoint;
in mat4 fragView;
in mat4 fragProj;

out vec4 FragColor;

uniform float nearPlane;
uniform float farPlane;

vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1.0);
    float minimumx = min(derivative.x, 1.0);

    vec4 color = vec4(0.3, 0.3, 0.4, 1.0 - min(line, 1.0));

    // Highlight z-axis
    if (drawAxis && fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx){
                color = vec4(0.0, 0.0, 1.0, color.a); // Blue axis
    }
    // Highlight x-axis
    else if (drawAxis && fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz){
                color = vec4(1.0, 0.0, 0.0, color.a); // Red axis
    }

    return color;
}


float computeDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos, 1.0);
    return (clip_space_pos.z / clip_space_pos.w) * 0.5 + 0.5; // Normalize to [0,1]
}

float computeLinearDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos, 1.0);
    float ndc_depth = clip_space_pos.z / clip_space_pos.w;
    float linearDepth = (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - ndc_depth * (farPlane - nearPlane));
    return linearDepth / farPlane; // Normalize to [0,1]
}

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    if (t < 0.0)
        discard;


    float distance = length(- fragPos3D);

    float fadeStart = 1.0; 
    float fadeEnd = 8.0;   
    float fadeFactor = clamp((fadeEnd - distance) / (fadeEnd - fadeStart), 0.0, 1.0);

    vec4 color = (grid(fragPos3D, 5.0, true) + grid(fragPos3D, 1.0, true));
    color.a *= fadeFactor;

    FragColor = color;
}

