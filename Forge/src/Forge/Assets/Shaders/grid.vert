#version 330 core
layout(location = 0) in vec3 aPos; // Position attribute

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 nearPoint;
out vec3 farPoint;

void main() {
    vec2 ndcPos = aPos.xy;

    // Compute inverse of the view-projection matrix
    mat4 invVP = inverse(u_Projection * u_View);

    vec4 clipSpaceNear = vec4(ndcPos, -1.0, 1.0);
    vec4 clipSpaceFar = vec4(ndcPos, 1.0, 1.0);

    vec4 worldNear = invVP * clipSpaceNear;
    vec4 worldFar = invVP * clipSpaceFar;

    nearPoint = worldNear.xyz / worldNear.w;
    farPoint = worldFar.xyz / worldFar.w;

    gl_Position = vec4(ndcPos, 0.0, 1.0);
}

