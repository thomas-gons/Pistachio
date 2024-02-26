#shader vertex
#version 460 core

const int MAX_QUADS = 1000000;

struct QuadInfoNDC {
    // position and size of the quad to be drawn
    vec2 quad_xy;
    vec2 quad_wh;

    // position of the texture to be applied to the quad
    vec2 tex_uv;
};

layout (location = 0) in vec2 vertexPos;
layout (std430, binding = 0) buffer QuadInfoBuffer {
    QuadInfoNDC quadInfo[MAX_QUADS];
    vec2 tex_wh;
};

out vec2 final_tex_uv;

void main() {
    vec2 newPos = vertexPos * quadInfo[gl_VertexID].quad_wh + quadInfo[gl_VertexID].quad_xy;
    gl_Position = vec4(newPos, 0.5, 1.0);
    final_tex_uv = vertexPos * tex_wh + quadInfo[gl_VertexID].tex_uv;
}

#shader fragment
#version 460 core

out vec4 FragColor;

in vec2 final_tex_uv;

// texture sampler
uniform sampler2D textureData;

void main() {
    FragColor = texture(textureData, final_tex_uv);
    if (FragColor == vec4(0.0)) {
        discard;
    }
}