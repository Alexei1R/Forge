#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 proj;

out vec3 nearPoint;
out vec3 farPoint;
out mat4 fragView;
out mat4 fragProj;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 invViewProj = inverse(projection * view);
    vec4 unprojectedPoint = invViewProj * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 p = aPos;

    // Unproject points on the near and far planes
    nearPoint = UnprojectPoint(p.x, p.y, 0.0, view, proj);
    farPoint = UnprojectPoint(p.x, p.y, 1.0, view, proj);

    fragView = view;
    fragProj = proj;

    // Use NDC coordinates directly
    gl_Position = vec4(p, 1.0);
}

