#version 330 core
layout(location = 0) in vec3 aPos;       // Match with BufferLayout in Mesh
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;



void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
