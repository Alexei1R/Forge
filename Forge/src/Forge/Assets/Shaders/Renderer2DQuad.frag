#version 450 core
layout(location = 0) out vec4 FragColor;



in vec4 v_Color;
in vec3 v_Position;

void main() {
    FragColor = v_Color;

}
