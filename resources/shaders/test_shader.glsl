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

layout (std430, binding = 0) buffer QuadInfoBuffer {
    vec2 tex_wh;
    QuadInfoNDC quadInfo[MAX_QUADS];
};

flat out vec2 final_tex_uv;



const vec2[] quadVertices = vec2[](
    vec2( 0.0, 0.0 ),
    vec2( 0.0, 1.0 ),
    vec2( 1.0, 1.0 ),
    vec2( 0.0, 0.0 ),
    vec2( 1.0, 1.0),
    vec2( 1.0, 0.0)
);

void main() {
    uint vertexIndex = gl_VertexID / 6;
    ivec2 pos = ivec2(quadVertices[gl_VertexID % 6]);

    vec2 newPos = pos * quadInfo[vertexIndex].quad_wh + quadInfo[vertexIndex].quad_xy;
    gl_Position = vec4(newPos, 0.5, 1.0);
    final_tex_uv = pos * tex_wh + quadInfo[vertexIndex].tex_uv;
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