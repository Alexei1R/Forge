#version 330 core

in vec2 v_TexCoord;
in vec4 v_Color;

uniform sampler2D u_FontAtlas;

out vec4 FragColor;

void main()
{
    float sdf = texture(u_FontAtlas, v_TexCoord).r;
    float distance = sdf;

    // Adjust the threshold as needed
    float alpha = smoothstep(0.5 - 0.1, 0.5 + 0.1, distance);

    FragColor = vec4(v_Color.rgb, alpha * v_Color.a);
}

