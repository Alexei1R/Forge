#version 330 core

// Vertex Attributes
layout(location = 0) in vec3 aPos;       // Position attribute
layout(location = 1) in vec3 aNormal;    // Normal attribute (unused)
layout(location = 2) in vec2 aTexCoords; // TexCoord attribute (unused)

// Uniforms
uniform mat4 u_View;
uniform mat4 u_Projection;

// Outputs to fragment shader
out vec3 nearPoint;
out vec3 farPoint;

void main() {
    // Use the position attribute as NDC coordinates
    vec2 ndcPos = aPos.xy;

    // Compute the inverse of the view-projection matrix
    mat4 invVP = inverse(u_Projection * u_View);

    // Define clip space positions at near and far planes
    vec4 clipSpaceNear = vec4(ndcPos, -1.0, 1.0); // z = -1 (near plane in NDC)
    vec4 clipSpaceFar = vec4(ndcPos, 1.0, 1.0);   // z = 1 (far plane in NDC)

    // Unproject to world space
    vec4 worldNear = invVP * clipSpaceNear;
    vec4 worldFar = invVP * clipSpaceFar;

    // Perspective divide
    nearPoint = worldNear.xyz / worldNear.w;
    farPoint = worldFar.xyz / worldFar.w;

    // Set gl_Position to cover the full screen
    gl_Position = vec4(ndcPos, 0.0, 1.0);
}

