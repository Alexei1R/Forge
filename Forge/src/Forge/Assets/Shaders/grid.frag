#version 330 core

in vec3 nearPoint;
in vec3 farPoint;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_CameraPos;

out vec4 FragColor;

float computeDepth(vec3 pos) {
    vec4 clipSpacePos = u_Projection * u_View * vec4(pos, 1.0);
    return (clipSpacePos.z / clipSpacePos.w) * 0.5 + 0.5;
}

vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);

    vec4 color = vec4(0.3, 0.3, 0.4, 1.0 - min(line, 1.0));

    if (drawAxis && fragPos3D.x > -0.1 && fragPos3D.x < 0.1) {
        color = vec4(1.0, 0.0, 0.0, color.a);
    }
    else if (drawAxis && fragPos3D.z > -0.1 && fragPos3D.z < 0.1) {
        color = vec4(0.0, 0.0, 1.0, color.a);
    }

    return color;
}

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    if (t < 0.0 || t > 1.0) {
        discard;
    }

    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);
    gl_FragDepth = computeDepth(fragPos3D);
    vec4 color = grid(fragPos3D, 5.0, true);

    FragColor = color;
}

