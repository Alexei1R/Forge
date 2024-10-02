#version 330 core

// Inputs from vertex shader
in vec3 nearPoint;
in vec3 farPoint;

// Uniforms
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_CameraPos;
uniform float u_Near;
uniform float u_Far;

// Output color
out vec4 FragColor;

// Function to compute depth
float computeDepth(vec3 pos) {
    vec4 clip_space_pos = u_Projection * u_View * vec4(pos, 1.0);
    return (clip_space_pos.z / clip_space_pos.w) * 0.5 + 0.5; // NDC to [0,1]
}

// Grid rendering function
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

void main() {
    // Calculate intersection factor 't' with the ground plane (y = 0)
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);

    // Discard fragments that don't intersect the ground plane
    if (t < 0.0 || t > 1.0) {
        discard;
    }

    // Compute the world-space position on the ground plane
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    // Calculate depth
    gl_FragDepth = computeDepth(fragPos3D);

    // Compute distance from the camera to the fragment position
    float distance = length(- fragPos3D);

    // Calculate fade factor (start fading after 10 meters)
    float fadeStart = 2.0; // Start fading at 10 meters
    float fadeEnd = 10.0;   
    float fadeFactor = clamp((fadeEnd - distance) / (fadeEnd - fadeStart), 0.0, 1.0);

    // Combine grids with different scales for more detail
    vec4 color = (grid(fragPos3D, 5.0, true) + grid(fragPos3D, 1.0, true)) * float(t > 0.0);

    // Apply fading based on fade factor
    color.a *= fadeFactor;

    // Discard fragments that are fully transparent
    if (color.a <= 0.0) {
        discard;
    }

    FragColor = color;
}

